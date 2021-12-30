#pragma once

#include <variant>
#include <boost/asio.hpp>

#include "common_msg_handler.hxx"

#include "control_message.hxx"
#include "control_message_ack.hxx"
#include "control_message_ack_with_checksum.hxx"
#include "control_message_nack.hxx"
#include "data_message.hxx"

namespace app {

class MessageHandler final: public CommonMessageHandler {
public:
    explicit MessageHandler(boost::asio::io_context &io_context);
    ~MessageHandler() = default;

    MessageHandler(const MessageHandler&) = delete;
    MessageHandler& operator=(const MessageHandler&) = delete;

    MessageHandler(MessageHandler&&) = delete;
    MessageHandler& operator=(MessageHandler&&) = delete;

protected:
    void cbReceive(const std::vector<std::byte>& data) override;

private:
    using Container = std::vector<std::byte>;
    using Message = std::variant<msg::ControlMsg<Container>,
                                 msg::ControlMsgAck<Container>,
                                 msg::ControlMsgNack<Container>,
                                 msg::ControlMsgAckCks<Container>,
                                 msg::DataMsg<Container>>;

    void HandleMessage(Message msg);
};

}  // namespace app
