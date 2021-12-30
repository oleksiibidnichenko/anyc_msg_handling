#pragma once

#include <memory>
#include <boost/asio.hpp>

#include "tcp_server_connection.hxx"

namespace dummy_app {

class MessageHandler: public net::TcpServerConnection {
public:
    explicit MessageHandler(boost::asio::io_context &io_context);
    ~MessageHandler() = default;

    MessageHandler(const MessageHandler&) = delete;
    MessageHandler& operator=(const MessageHandler&) = delete;

    MessageHandler(MessageHandler&&) = delete;
    MessageHandler& operator=(MessageHandler&&) = delete;

protected:
    void cbReceive(const std::vector<std::byte>& data) override;

    bool isValidMsgType() const override;
    uint32_t getBodySize() const noexcept override;
private:
    uint16_t getMsgType() const;

    static constexpr size_t sMsgTypeLen = 2;
    static constexpr size_t sMsgSizeLen = 4;
};

}  // namespace dummy_app
