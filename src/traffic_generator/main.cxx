#include <boost/asio.hpp>
#include <boost/format.hpp>

#include <getopt.h>
#include <iostream>
#include <stdexcept>
#include <vector>

using boost::asio::ip::tcp;

struct send_spec_t {
    unsigned type;
    unsigned size;
    unsigned fraction;
};

std::vector<send_spec_t> send_spec;

std::vector<std::byte> full_body;

void composeMessageQueue()
{
    const std::byte filler_byte{0x55}; // TODO variate
    unsigned all_fractions = 0;
    for (const send_spec_t& one_spec : send_spec) {
        std::byte head[6];
        uint16_t mtype = htons(one_spec.type);
        uint32_t msize = htonl(one_spec.size);
        memcpy(&head[0], &mtype, 2);
        memcpy(&head[2], &msize, 4);
        for (unsigned repeat = 0; repeat < one_spec.fraction; ++repeat) {
            for (unsigned i = 0; i < 6; ++i) {
                full_body.push_back(head[i]);
            }
            for (unsigned i = 0; i < one_spec.size; ++i) {
                full_body.push_back(filler_byte);
            }
        }
        all_fractions += one_spec.fraction;
    }
    if (all_fractions != 100) {
        throw std::runtime_error("Error: fraction sum is not 100");
    }
}

void parseMessageSpec(const char *spec_text)
{
    constexpr size_t MAX_FULL_LENGTH = 65542; // 6 for header, 65536 for payload
    // Expect: <type>:<size>:<fraction>. We don't pedantically check the syntax.
    const char *spec_text_orig = spec_text;
    static const char COLON[] = ":";
    send_spec_t one_spec;
    one_spec.type = strtoul(spec_text, nullptr, 16);
    spec_text += strcspn(spec_text, COLON);
    spec_text += strspn(spec_text, COLON);
    one_spec.size = strtoul(spec_text, nullptr, 10);
    spec_text += strcspn(spec_text, COLON);
    spec_text += strspn(spec_text, COLON);
    one_spec.fraction = strtoul(spec_text, nullptr, 10);
    if (one_spec.size <= 0 || one_spec.size >= MAX_FULL_LENGTH) {
        auto formatted = boost::format("Wrong size: %d in message spec: %s")
                % one_spec.size % spec_text_orig;
        throw std::runtime_error(formatted.str());
    }
    if (one_spec.fraction == 0 || one_spec.fraction > 100) {
        auto formatted = boost::format("Wrong fraction: %d in message spec: %s")
                         % one_spec.fraction % spec_text_orig;
        throw std::runtime_error(formatted.str());
    }
    send_spec.push_back(one_spec);
}

void sendFullBody(tcp::socket& target_socket)
{
    size_t pos = 0;
    size_t rest = full_body.size();
    while (pos < rest) {
        pos += target_socket.write_some(boost::asio::buffer(&full_body[pos], rest));
    }
}

int main(int argc, char **argv)
{
    int opt;
    unsigned send_seconds = 10;
    while ((opt = getopt(argc, argv, "m:t:")) != -1) {
        switch (opt) {
            case 'm':
                parseMessageSpec(optarg);
                break;
            case 't':
                send_seconds = strtoul(optarg, nullptr, 0);
                break;
        }
    }
    std::cout << "I am traffic generator..." << std::endl;
    composeMessageQueue();

    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
            resolver.resolve("localhost", "32101");
    tcp::socket client(io_context);
    boost::asio::connect(client, endpoints);
    // Sending cycle.
    // We assume a single cycle is principally less than 1 second.
    auto ts_started = std::chrono::steady_clock::now();
    size_t sent_portions = 0;
    for(;;) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> dt = now - ts_started;
        if (dt.count() > (double)send_seconds) {
            break;
        }
        sendFullBody(client);
        ++sent_portions;
    }
    size_t sent_messages = sent_portions * 100;
    double rate = (double)sent_messages / send_seconds;
    std::cout << "Send rate: " << rate << " messages per second" << std::endl;
    double ns_per_message = 1.0e9 / rate;
    std::cout << ns_per_message << " ns spent for one message" << std::endl;
    return 0;
}
