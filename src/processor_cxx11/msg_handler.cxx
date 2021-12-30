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

namespace app {

MessageHandler::MessageHandler(boost::asio::io_context &io_context):
    CommonMessageHandler(io_context)
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
            msg = std::make_unique<msg::ControlMsg<Container>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgAck:
            msg = std::make_unique<msg::ControlMsgAck<Container>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgNack:
            msg = std::make_unique<msg::ControlMsgNack<Container>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgAckCks:
            msg = std::make_unique<msg::ControlMsgAckCks<Container>>(std::move(raw_msg));
            break;
        case msg::MsgType::DataMsg:
            msg = std::make_unique<msg::DataMsg<Container>>(std::move(raw_msg));
            break;
        default:
            std::cerr << __func__ << ": Wrong msg type: " << std::hex
                << getMsgType() << std::endl;
            return;
    }

    HandleMessage(std::move(msg));
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
                auto ptr = dynamic_cast<msg::ControlMsg<Container>*>(msg.get());
                if (ptr) {
                    std::cout << "Handling Control Message" << std::endl;
                }
            }
            break;
        case msg::MsgType::ControlMsgAck:
            {
                auto ptr = dynamic_cast<msg::ControlMsgAck<Container>*>(msg.get());
                if (ptr) {
                    std::cout << "Handling Control Acknowledgement Message" << std::endl;
                }
            }
            break;
        case msg::MsgType::ControlMsgNack:
            {
                auto ptr = dynamic_cast<msg::ControlMsgNack<Container>*>(msg.get());
                if (ptr) {
                    std::cout << "Handling Control Negative Acknowledgement Message" << std::endl;
                }
            }
            break;
        case msg::MsgType::ControlMsgAckCks:
            {
                auto ptr = dynamic_cast<msg::ControlMsgAckCks<Container>*>(msg.get());
                if (ptr) {
                    std::cout << "Handling Control Acknowledgement with Checksum Message" << std::endl;
                }
            }
            break;
        case msg::MsgType::DataMsg:
            {
                auto ptr = dynamic_cast<msg::DataMsg<Container>*>(msg.get());
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

}  // namespace app
