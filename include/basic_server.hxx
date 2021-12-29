#pragma once

#include <boost/asio.hpp>

class TcpServerConnection;
class TcpAcceptor;

class TcpServerConnection {
public:
    const size_t INPUT_BUFFER_SIZE = 4096;
    explicit TcpServerConnection(boost::asio::io_context &io_context);
    virtual ~TcpServerConnection();
protected:
    std::vector<char> mInputBuffer;
    boost::asio::ip::tcp::socket& getSocket()
    {
        return mSocket;
    }
    virtual void cbReceive(const boost::system::error_code& ec, std::size_t length) = 0;
private:
    friend class TcpAcceptor;
    boost::asio::ip::tcp::socket mSocket;
    bool mReceiveRequested = false;
    bool mIsStopped = false;
    void requestReceive();
    void onReceive(const boost::system::error_code& ec, std::size_t length);
};

class TcpAcceptor {
public:
    explicit TcpAcceptor(boost::asio::io_context &);
    TcpAcceptor(const TcpAcceptor&) = delete;
    TcpAcceptor(TcpAcceptor&&) = delete;
    TcpAcceptor& operator=(const TcpAcceptor&) = delete;
    TcpAcceptor& operator=(TcpAcceptor&&) = delete;
    ~TcpAcceptor();
    void start(const boost::asio::ip::tcp::endpoint &);
protected:
    virtual TcpServerConnection *createConnectionObject(
            boost::asio::io_context &io) = 0;
    std::vector<std::unique_ptr<TcpServerConnection>> mConnectionHistory;
private:
    boost::asio::io_context& mIO;
    boost::asio::ip::tcp::acceptor mListeningSocket;
    bool mAcceptRequested = false;
    std::unique_ptr<TcpServerConnection> mNextConnection;
    void requestAccept();
    void handleAccept(const boost::system::error_code &error);
};
