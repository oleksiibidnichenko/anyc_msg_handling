#pragma once

#include <boost/asio.hpp>

#include "tcp_server_connection.hxx"

namespace app {

class CommonMessageHandler: public net::TcpServerConnection {
public:
    explicit CommonMessageHandler(boost::asio::io_context &io_context);
    ~CommonMessageHandler() = default;

    CommonMessageHandler(const CommonMessageHandler&) = delete;
    CommonMessageHandler& operator=(const CommonMessageHandler&) = delete;

    CommonMessageHandler(CommonMessageHandler&&) = delete;
    CommonMessageHandler& operator=(CommonMessageHandler&&) = delete;

protected:
    bool isValidMsgType() const override;
    uint32_t getBodySize() const noexcept override;

    uint16_t getMsgType() const;

private:
    static constexpr size_t sMsgTypeLen = 2;
    static constexpr size_t sMsgSizeLen = 4;
};

}  // namespace app
