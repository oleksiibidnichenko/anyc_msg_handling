#include "msg_handler.hxx"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <vector>

#include "control_message.hxx"
#include "control_message_ack.hxx"
#include "control_message_ack_with_checksum.hxx"
#include "control_message_nack.hxx"
#include "data_message.hxx"

namespace cpp11_app {

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
    std::vector<std::byte> raw_msg(data);
    std::unique_ptr<msg::BaseMsg> msg;

    switch (static_cast<msg::MsgType>(getMsgType())) {
        case msg::MsgType::ControlMsg:
            msg = std::make_unique<msg::ControlMsg<std::vector<std::byte>>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgAck:
            msg = std::make_unique<msg::ControlMsgAck<std::vector<std::byte>>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgNack:
            msg = std::make_unique<msg::ControlMsgNack<std::vector<std::byte>>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgAckCks:
            msg = std::make_unique<msg::ControlMsgAckCks<std::vector<std::byte>>>(std::move(raw_msg));
            break;
        case msg::MsgType::DataMsg:
            msg = std::make_unique<msg::DataMsg<std::vector<std::byte>>>(std::move(raw_msg));
            break;
        default:
            std::cerr << __func__ << ": Wrong msg type: " << std::hex
                << getMsgType() << std::endl;
            return;
    }

    HandleMessage(std::move(msg));
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

void MessageHandler::HandleMessage(std::unique_ptr<msg::BaseMsg> msg)
{
    if (!msg) {
        std::cerr << __func__ << ": No message to handle." << std::endl;
        return;
    }

    // at this point message should be retrieved with std::variant
    switch (msg->GetMsgType()) {
        case msg::MsgType::ControlMsg:
            {
                auto ptr = dynamic_cast<msg::ControlMsg<std::vector<std::byte>>*>(msg.get());
                if (ptr) {
                    std::cout << "Handling Control Message" << std::endl;
                }
            }
            break;
        case msg::MsgType::ControlMsgAck:
            {
                auto ptr = dynamic_cast<msg::ControlMsgAck<std::vector<std::byte>>*>(msg.get());
                if (ptr) {
                    std::cout << "Handling Control Acknowledgement Message" << std::endl;
                }
            }
            break;
        case msg::MsgType::ControlMsgNack:
            {
                auto ptr = dynamic_cast<msg::ControlMsgNack<std::vector<std::byte>>*>(msg.get());
                if (ptr) {
                    std::cout << "Handling Control Negative Acknowledgement Message" << std::endl;
                }
            }
            break;
        case msg::MsgType::ControlMsgAckCks:
            {
                auto ptr = dynamic_cast<msg::ControlMsgAckCks<std::vector<std::byte>>*>(msg.get());
                if (ptr) {
                    std::cout << "Handling Control Acknowledgement with Checksum Message" << std::endl;
                }
            }
            break;
        case msg::MsgType::DataMsg:
            {
                auto ptr = dynamic_cast<msg::DataMsg<std::vector<std::byte>>*>(msg.get());
                if (ptr) {
                    std::cout << "Handling Data Message" << std::endl;
                }
            }
            break;
        default:
            std::cerr << __func__ << ": Wrong msg type: " << std::hex
                << getMsgType() << std::endl;
            return;
    }
}

}  // namespace cpp11_app
