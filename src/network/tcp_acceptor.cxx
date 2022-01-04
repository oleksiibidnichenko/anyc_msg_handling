#include "tcp_acceptor.hxx"

#include <iostream>

//--------------------------------------------------------------
// TcpServerConnection

namespace net {

constexpr int TcpAcceptor::sTimerDuration;

TcpAcceptor::TcpAcceptor(boost::asio::io_context &io,
                         std::unique_ptr<TcpServerConnection> connection)
    : mListeningSocket(io),
      mConnection(std::move(connection)),
      mConnTimer(io)
{
}

void TcpAcceptor::start(const boost::asio::ip::tcp::endpoint &ep_to_bind)
{
    if (mListeningSocket.is_open())
    {
        throw std::logic_error("TcpAcceptor: duplicated start()");
    }

    if (!mConnection)
    {
        throw std::logic_error("TcpAcceptor is not initialized");
    }

    mListeningSocket.open(boost::asio::ip::tcp::v4());
    mListeningSocket.set_option(boost::asio::ip::tcp::acceptor::reuse_address(
            true));
    mListeningSocket.bind(ep_to_bind);
    mListeningSocket.listen(1);
    requestAccept();
}

void TcpAcceptor::requestAccept()
{
    // Schedule next acceptance in proactive pattern.
    mConnection->resetConnection();

    mListeningSocket.async_accept(mConnection->getSocket(),
                                  [this] (const boost::system::error_code& ec)
                                  {
                                      handleAccept(ec);
                                  });
}

void TcpAcceptor::handleAccept(const boost::system::error_code &ec)
{
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }

    if (ec) {
        std::cerr << __func__ << ec.category().name() << ':' << ec.value()
            << std::endl;
        requestAccept();
        return;
    }

    mConnection->readMsgHeader();
    mConnTimer.expires_from_now(boost::posix_time::seconds(sTimerDuration));
    mConnTimer.async_wait([this] (const boost::system::error_code &ec)
            {
                handleTimeout(ec);
            });
}

void TcpAcceptor::handleTimeout(const boost::system::error_code &ec)
{
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }

    if (ec) {
        std::cerr << __func__ << ec.category().name() << ':' << ec.value()
            << std::endl;
        return;
    }

    if (mConnection->isActive()) {
        mConnTimer.expires_from_now(boost::posix_time::seconds(sTimerDuration));
        mConnTimer.async_wait([this] (const boost::system::error_code &ec)
                {
                    handleTimeout(ec);
                });
    } else {
        requestAccept();
    }
}

}  // namespace net
