#include "msg_handler.hxx"
#include "tcp_acceptor.hxx"

#include <iostream>

namespace {

boost::asio::io_context io_context{};

void signal_cb(const boost::system::error_code& error, int /*signal_number*/)
{
    if (!error)
    {
        io_context.stop();
    }
}

} //  namespace

int main()
{
    // Setup signal listener
    boost::asio::signal_set signal_listener(io_context, SIGINT, SIGTERM);
    signal_listener.async_wait(signal_cb);

    // Setup TCP listener
    constexpr uint16_t listening_port = 32101;
    net::TcpAcceptor acceptor(io_context,
            std::make_unique<dummy_app::MessageHandler>(io_context));

    acceptor.start(boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address_v4(), listening_port));

    // Start up the network
    boost::system::error_code ec;

    io_context.run(ec);

    if (ec) {
        std::cerr << __func__ << ec.category().name() << ':' << ec.value()
            << std::endl;
        return 1;
    }

    return 0;
}
