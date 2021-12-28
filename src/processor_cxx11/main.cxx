#include "../basic_async_model/basic_server.hxx"

#include <iostream>

class Cxx11ServerConnection : public TcpServerConnection {
public:
    explicit Cxx11ServerConnection(boost::asio::io_context &io)
        : TcpServerConnection(io) {
    }
protected:
    void cbReceive(const boost::system::error_code&, std::size_t) override;
};

void Cxx11ServerConnection::cbReceive(const boost::system::error_code& ec, std::size_t length)
{
    if (!ec) {
        std::cout << "cbReceive: got " << length << " bytes" << std::endl;
    }
    else {
        std::cout << "cbReceive: error" << std::endl;
    }
}

class Cxx11Acceptor : public TcpAcceptor {
public:
    Cxx11Acceptor(boost::asio::io_context &, int);
protected:
    TcpServerConnection *createConnectionObject(
            boost::asio::io_context &io) override
    {
        return new Cxx11ServerConnection(io);
    };
};

Cxx11Acceptor::Cxx11Acceptor(boost::asio::io_context &io, int port)
    : TcpAcceptor(io)
{
    boost::asio::ip::tcp::endpoint ep_listen(boost::asio::ip::address_v4(), port);
    start(ep_listen);
}

boost::asio::io_context io_context{};

static void signal_cb(const boost::system::error_code& error, int /*signal_number*/)
{
    if (!error)
    {
        // A signal occurred.
        io_context.stop();
    }
}

int main()
{
    // Setup TCP listener
    static const int LISTENING_PORT = 32101;
    boost::system::error_code ec;
    Cxx11Acceptor acceptor(io_context, LISTENING_PORT);

    // Setup signal listener
    boost::asio::signal_set signal_listener(io_context, SIGINT, SIGTERM);
    signal_listener.async_wait(signal_cb);

    std::cout << "Entering event loop" << std::endl;
    io_context.run(ec);
    std::cout << "Exited event loop";
    if (ec) {
        std::cout << ", with error";
    }
    std::cout << std::endl;
    return bool(ec);
}
