#pragma once

#include <cstddef>
#include <boost/asio.hpp>

namespace net {

class TcpServerConnection {
public:
    explicit TcpServerConnection(boost::asio::io_context &io_context);
    virtual ~TcpServerConnection();

    TcpServerConnection(const TcpServerConnection&) = delete;
    TcpServerConnection& operator=(const TcpServerConnection&) = delete;

    TcpServerConnection(TcpServerConnection&&) = delete;
    TcpServerConnection& operator=(TcpServerConnection&&) = delete;

    boost::asio::ip::tcp::socket& getSocket()
    {
        return mSocket;
    }

    bool isActive() const
    {
        return mSocket.is_open();
    }

    void resetConnection();

    void readMsgHeader();

protected:
    virtual void cbReceive(const std::vector<std::byte>& data) = 0;

    virtual bool isValidMsgType() const = 0;
    virtual uint32_t getBodySize() const noexcept = 0;

    std::vector<std::byte> mInputBuffer;

private:
    static constexpr size_t sInputBufferSize = 65536;
    static constexpr size_t sMinHeaderSize = 6;

    void readMsgBody(size_t len);
    void readMsgIgnore(size_t len);

    void onReadHeader(const boost::system::error_code& ec, size_t len);
    void onReadBody(const boost::system::error_code& ec, size_t len);
    void onReadWrongData(const boost::system::error_code& ec, size_t len);

    boost::asio::ip::tcp::socket mSocket;
};

}  // namespace net
