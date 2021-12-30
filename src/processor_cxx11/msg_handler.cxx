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
    CommonMessageHandler(io_context),
    mCtr(0)
{
}

void MessageHandler::cbReceive(const std::vector<std::byte>& data)
{
    if (!data.size()) {
        std::cerr << __func__ << ": buffer is empty" << std::endl;
        return;
    }

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

void MessageHandler::onConnTerm() noexcept
{
    std::cout << "This session managed to handle with " << mCtr << " messages."
              << std::endl;
    mCtr = 0;
}

void MessageHandler::HandleMessage(std::unique_ptr<msg::BaseMsg> msg)
{
    ++mCtr;
    switch (msg->GetMsgType()) {
        case msg::MsgType::ControlMsg:
            {
                auto ptr = dynamic_cast<msg::ControlMsg<Container>*>(msg.get());
                if (ptr) {
                    std::cerr << "Handling Control Message."
                              << " First byte" << std::hex
                              << static_cast<uint8_t>(*ptr->GetRawBuffer())
                              << std::endl;
                }
            }
            break;
        case msg::MsgType::ControlMsgAck:
            {
                auto ptr = dynamic_cast<msg::ControlMsgAck<Container>*>(msg.get());
                if (ptr) {
                    std::cerr << "Handling Control Acknowledgement Message."
                              << " First byte" << std::hex
                              << static_cast<uint8_t>(*ptr->GetRawBuffer())
                              << std::endl;
                }
            }
            break;
        case msg::MsgType::ControlMsgNack:
            {
                auto ptr = dynamic_cast<msg::ControlMsgNack<Container>*>(msg.get());
                if (ptr) {
                    std::cerr << "Handling Control Negative Acknowledgement Message."
                              << " First byte" << std::hex
                              << static_cast<uint8_t>(*ptr->GetRawBuffer())
                              << std::endl;
                }
            }
            break;
        case msg::MsgType::ControlMsgAckCks:
            {
                auto ptr = dynamic_cast<msg::ControlMsgAckCks<Container>*>(msg.get());
                if (ptr) {
                    std::cerr << "Handling Control Acknowledgement with Checksum Message."
                              << " First byte" << std::hex
                              << static_cast<uint8_t>(*ptr->GetRawBuffer())
                              << std::endl;
                }
            }
            break;
        case msg::MsgType::DataMsg:
            {
                auto ptr = dynamic_cast<msg::DataMsg<Container>*>(msg.get());
                if (ptr) {
                    std::cerr << "Handling Data Message."
                              << " First byte" << std::hex
                              << static_cast<uint8_t>(*ptr->GetRawBuffer())
                              << std::endl;
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
