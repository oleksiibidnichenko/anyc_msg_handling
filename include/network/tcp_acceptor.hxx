#pragma once

#include <cstddef>
#include <boost/asio.hpp>

#include "tcp_server_connection.hxx"

namespace net {

class TcpAcceptor final {
public:
    TcpAcceptor(boost::asio::io_context &,
                std::unique_ptr<TcpServerConnection>);
    ~TcpAcceptor() = default;

    TcpAcceptor(const TcpAcceptor&) = delete;
    TcpAcceptor& operator=(const TcpAcceptor&) = delete;

    TcpAcceptor(TcpAcceptor&&) = delete;
    TcpAcceptor& operator=(TcpAcceptor&&) = delete;

    void start(const boost::asio::ip::tcp::endpoint &);

private:
    void requestAccept();
    void handleAccept(const boost::system::error_code &error);

    void handleTimeout(const boost::system::error_code &error);

    static constexpr int sTimerDuration = 1;

    boost::asio::ip::tcp::acceptor mListeningSocket;

    std::unique_ptr<TcpServerConnection> mConnection;

    boost::asio::deadline_timer mConnTimer;
};

}  // namespace net
