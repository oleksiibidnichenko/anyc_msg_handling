#include "msg_handler.hxx"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <net/if.h>
#include <optional>
#include <vector>

#include "base_message.hxx"

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
    std::optional<Message> msg;

    switch (static_cast<msg::MsgType>(getMsgType())) {
        case msg::MsgType::ControlMsg:
            msg = msg::ControlMsg<Container>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgAck:
            msg = msg::ControlMsgAck<Container>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgNack:
            msg = msg::ControlMsgNack<Container>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgAckCks:
            msg = msg::ControlMsgAckCks<Container>(std::move(raw_msg));
            break;
        case msg::MsgType::DataMsg:
            msg = msg::DataMsg<Container>(std::move(raw_msg));
            break;
        default:
            std::cerr << __func__ << ": Wrong msg type: " << std::hex
                << getMsgType() << std::endl;
            return;
    }

    HandleMessage(std::move(msg.value()));
}

void MessageHandler::HandleMessage(Message msg)
{
    std::visit([](auto& arg) {
                switch (arg.GetMsgType()) {
                    case msg::MsgType::ControlMsg:
                        std::cout << "Handling Control Message" << std::endl;
                        break;
                    case msg::MsgType::ControlMsgAck:
                        std::cout << "Handling Control Acknowledgement Message"
                                  << std::endl;
                        break;
                    case msg::MsgType::ControlMsgNack:
                        std::cout << "Handling Control Negative " <<
                                     "Acknowledgement Message" << std::endl;
                        break;
                    case msg::MsgType::ControlMsgAckCks:
                        std::cout << "Handling Control Acknowledgement "
                                  <<"with Checksum Message" << std::endl;
                        break;
                    case msg::MsgType::DataMsg:
                        std::cout << "Handling Data Message" << std::endl;
                        break;
                    default:
                        throw std::logic_error("Wrong msg type");
                }
            }, msg);
}

}  // namespace app
