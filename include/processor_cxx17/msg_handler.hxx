#pragma once

#include "base_message.hxx"
#include "tcp_server_connection.hxx"

#include "control_message.hxx"
#include "control_message_ack.hxx"
#include "control_message_ack_with_checksum.hxx"
#include "control_message_nack.hxx"
#include "data_message.hxx"

#include <memory>
#include <boost/asio.hpp>
#include <variant>

namespace cpp17_app {

class MessageHandler: public net::TcpServerConnection {
public:
    explicit MessageHandler(boost::asio::io_context &io_context);
    ~MessageHandler() = default;

    MessageHandler(const MessageHandler&) = delete;
    MessageHandler& operator=(const MessageHandler&) = delete;

    MessageHandler(MessageHandler&&) = delete;
    MessageHandler& operator=(MessageHandler&&) = delete;

    using container_t = std::vector<std::byte>;

    using MessageVariant = std::variant<
            msg::ControlMsg<container_t>,
            msg::ControlMsgAck<container_t>,
            msg::ControlMsgNack<container_t>,
            msg::ControlMsgAckCks<container_t>,
            msg::DataMsg<container_t>
    >;

protected:
    void cbReceive(const std::vector<std::byte>& data) override;

    bool isValidMsgType() const override;
    uint32_t getBodySize() const noexcept override;
private:
    uint16_t getMsgType() const;

    void HandleMessage(std::unique_ptr<MessageVariant> msg);

    static constexpr size_t sMsgTypeLen = 2;
    static constexpr size_t sMsgSizeLen = 4;
};

}  // namespace cpp17_app
