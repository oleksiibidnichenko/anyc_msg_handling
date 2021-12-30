#include "tcp_server_connection.hxx"

#include <iostream>

//--------------------------------------------------------------
// TcpServerConnection

namespace net {

TcpServerConnection::TcpServerConnection(boost::asio::io_context &io_context)
    : mSocket(io_context)
{
    mInputBuffer.resize(sMinHeaderSize);
}

TcpServerConnection::~TcpServerConnection()
{
    resetConnection();
}

void TcpServerConnection::resetConnection()
{
    if (mSocket.is_open()) {
        mSocket.close();
    }
}

void TcpServerConnection::readMsgHeader()
{
    boost::asio::async_read(mSocket, boost::asio::buffer(mInputBuffer),
            boost::asio::transfer_exactly(sMinHeaderSize),
            [this] (const boost::system::error_code& ec, size_t len)
            {
                onReadHeader(ec, len);
            });
}

void TcpServerConnection::readMsgBody(size_t len)
{
    mInputBuffer.resize(sMinHeaderSize + len);
    boost::asio::async_read(mSocket,
        boost::asio::buffer(&mInputBuffer[sMinHeaderSize], len),
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
        mInputBuffer.resize(sMinHeaderSize + len);
        boost::asio::async_read(mSocket,
            boost::asio::buffer(&mInputBuffer[sMinHeaderSize], len),
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

    if (len != sMinHeaderSize) {
        std::cerr << __func__ << ": Missing data! Expected: "
            << sMinHeaderSize << " Actual: " << len << std::endl;
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
