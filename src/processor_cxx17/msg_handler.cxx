#include "msg_handler.hxx"

#include "control_message.hxx"
#include "control_message_ack.hxx"
#include "control_message_ack_with_checksum.hxx"
#include "control_message_nack.hxx"
#include "data_message.hxx"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <variant>
#include <vector>

namespace cpp17_app {

MessageHandler::MessageHandler(boost::asio::io_context &io_context):
    net::TcpServerConnection(io_context)
{
}

void MessageHandler::cbReceive(const std::vector<std::byte>& data)
{
    if (!data.size()) {
        std::cerr << __func__ << ": buffer is empty" << std::endl;
        return;
    }

    // this place should be optimized with PMR
    container_t raw_msg(data);
    std::unique_ptr<MessageVariant> msg_variant;

    switch (static_cast<msg::MsgType>(getMsgType())) {
        case msg::MsgType::ControlMsg:
            msg_variant = std::make_unique<MessageVariant>(msg::ControlMsg<container_t>(std::move(raw_msg)));
            break;
        case msg::MsgType::ControlMsgAck:
            msg_variant = std::make_unique<MessageVariant>(msg::ControlMsg<container_t>(std::move(raw_msg)));
            msg_variant->emplace<msg::ControlMsgAck<container_t>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgNack:
            msg_variant = std::make_unique<MessageVariant>(msg::ControlMsgNack<container_t>(std::move(raw_msg)));
            break;
        case msg::MsgType::ControlMsgAckCks:
            msg_variant = std::make_unique<MessageVariant>(msg::ControlMsgAckCks<container_t>(std::move(raw_msg)));
            break;
        case msg::MsgType::DataMsg:
            msg_variant = std::make_unique<MessageVariant>(msg::DataMsg<container_t>(std::move(raw_msg)));
            break;
        default:
            std::cerr << __func__ << ": Wrong msg type: " << std::hex
                << getMsgType() << std::endl;
            return;
    }

    HandleMessage(std::move(msg_variant));
}

bool MessageHandler::isValidMsgType() const
{
    switch (static_cast<msg::MsgType>(getMsgType())) {
        case msg::MsgType::ControlMsg:
        case msg::MsgType::ControlMsgAck:
        case msg::MsgType::ControlMsgNack:
        case msg::MsgType::ControlMsgAckCks:
        case msg::MsgType::DataMsg:
            return true;
        default:
            return false;
    }
}

uint32_t MessageHandler::getBodySize() const noexcept
{
    if (mInputBuffer.size() < (sMsgTypeLen + sMsgSizeLen)) {
        return 0;
    }

    uint32_t msg_size = 0;

    std::memcpy(static_cast<void*>(&msg_size),
            static_cast<const void*>(mInputBuffer.data() + sMsgTypeLen),
            sizeof(uint32_t));

    return ntohl(msg_size);
}

uint16_t MessageHandler::getMsgType() const
{
    if (mInputBuffer.size() < sMsgTypeLen) {
        return 0;
    }

    uint16_t msg_type = 0;

    std::memcpy(static_cast<void*>(&msg_type),
            static_cast<const void*>(mInputBuffer.data()), sizeof(uint16_t));

    return ntohs(msg_type);
}

void MessageHandler::HandleMessage(std::unique_ptr<MessageHandler::MessageVariant> msg)
{
    if (!msg) {
        std::cerr << __func__ << ": No message to handle." << std::endl;
        return;
    }

    // at this point message should be retrieved with std::variant
    msg::MsgType msg_type;
    std::visit([&](auto& arg) { msg_type = arg.GetMsgType(); }, *msg);
    switch (msg_type) {
        case msg::MsgType::ControlMsg:
            {
                auto& value = std::get<msg::ControlMsg<container_t>>(*msg);
                (void)value; // Silence "unused value"
                std::cout << "Handling Control Message" << std::endl;
            }
            break;
        case msg::MsgType::ControlMsgAck:
            {
                auto& value = std::get<msg::ControlMsgAck<container_t>>(*msg);
                (void)value;
                std::cout << "Handling Control Acknowledgement Message" << std::endl;
            }
            break;
        case msg::MsgType::ControlMsgNack:
            {
                auto& value = std::get<msg::ControlMsgNack<container_t>>(*msg);
                (void)value;
                std::cout << "Handling Control NonAcknowledgement Message" << std::endl;
            }
            break;
        case msg::MsgType::ControlMsgAckCks:
            {
                auto& value = std::get<msg::ControlMsgAckCks<container_t>>(*msg);
                (void)value;
                std::cout << "Handling Control Ack-Cks Message" << std::endl;
            }
            break;
        case msg::MsgType::DataMsg:
            {
                auto& value = std::get<msg::DataMsg<container_t>>(*msg);
                (void)value;
                std::cout << "Handling Data Message" << std::endl;
            }
            break;
        default:
            std::cerr << __func__ << ": Wrong msg type: " << std::hex
                << getMsgType() << std::endl;
            return;
    }
}

}  // namespace cpp17_app
