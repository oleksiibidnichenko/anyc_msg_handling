#include "tcp_server_connection.hxx"

#include <iostream>

#include "common_limit.hxx"

//--------------------------------------------------------------
// TcpServerConnection

namespace net {

TcpServerConnection::TcpServerConnection(boost::asio::io_context &io_context)
    : mSocket(io_context)
{
    mInputBuffer.reserve(limit::max_msg_size);
}

TcpServerConnection::~TcpServerConnection()
{
    resetConnection();
}

void TcpServerConnection::resetConnection()
{
    if (mSocket.is_open()) {
        mSocket.close();
        onConnTerm();
    }
}

void TcpServerConnection::readMsgHeader()
{
    mInputBuffer.resize(limit::min_header_size);
    boost::asio::async_read(mSocket, boost::asio::buffer(mInputBuffer),
            boost::asio::transfer_exactly(limit::min_header_size),
            [this] (const boost::system::error_code& ec, size_t len)
            {
                onReadHeader(ec, len);
            });
}

void TcpServerConnection::readMsgBody(size_t len)
{
    mInputBuffer.resize(limit::min_header_size + len);
    boost::asio::async_read(mSocket,
        boost::asio::buffer(&mInputBuffer[limit::min_header_size], len),
        boost::asio::transfer_exactly(len),
        [this] (const boost::system::error_code& ec, size_t len)
        {
            onReadBody(ec, len);
        });
}

void TcpServerConnection::readMsgIgnore(size_t len)
{
    if (!len) {
        readMsgHeader();
    } else {
        // LIMITATION we hope nobody will try to send gigabytes of data and one
        // async_read will be enough.
        mInputBuffer.resize(limit::min_header_size + len);
        boost::asio::async_read(mSocket,
            boost::asio::buffer(&mInputBuffer[limit::min_header_size], len),
            boost::asio::transfer_exactly(len),
            [this] (const boost::system::error_code& ec, size_t len)
            {
                onReadWrongData(ec, len);
            });
    }
}

void TcpServerConnection::onReadHeader(const boost::system::error_code& ec,
        size_t len)
{
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }

    if (ec) {
        std::cerr << __func__ << " " << ec.category().name() << ':'
            << ec.value() << std::endl;
        resetConnection();
        return;
    }

    if (len != limit::min_header_size) {
        std::cerr << __func__ << ": Missing data! Expected: "
            << limit::min_header_size << " Actual: " << len << std::endl;
        return;
    }

    if (isValidMsgType()) {
        readMsgBody(getBodySize());
    } else {
        auto msg_size = getBodySize();
        std::cerr << __func__
                  << " : Invalid message type, ignoring message leftover ("
                  << msg_size << "bytes)" << std::endl;

        readMsgIgnore(msg_size);
    }
}

void TcpServerConnection::onReadBody(const boost::system::error_code& ec,
        size_t len)
{
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }

    if (ec) {
        std::cerr << __func__ << " " << ec.category().name() << ':'
            << ec.value() << std::endl;
        resetConnection();
        return;
    }

    if (getBodySize() == len) {
        cbReceive(mInputBuffer);
    } else {
        std::cerr << __func__ << ": Missing data! Expected: " << getBodySize()
            << " Actual: " << len << std::endl;
    }

    readMsgHeader();
}

void TcpServerConnection::onReadWrongData(const boost::system::error_code& ec,
        size_t len)
{
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }

    static_cast<void>(len);

    if (ec) {
        std::cerr << __func__ << " " << ec.category().name() << ':'
            << ec.value() << std::endl;
        resetConnection();
        return;
    }

    readMsgHeader();
}

}  // namespace net
