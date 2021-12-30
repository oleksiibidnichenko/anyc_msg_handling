#include "msg_handler.hxx"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <net/if.h>
#include <vector>

#include "base_message.hxx"

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

    Container raw_msg = mMemRes.getVector(data);
    Message msg;

    switch (static_cast<msg::MsgType>(getMsgType())) {
        case msg::MsgType::ControlMsg:
            msg.emplace<msg::ControlMsg<Container>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgAck:
            msg.emplace<msg::ControlMsgAck<Container>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgNack:
            msg.emplace<msg::ControlMsgNack<Container>>(std::move(raw_msg));
            break;
        case msg::MsgType::ControlMsgAckCks:
            msg.emplace<msg::ControlMsgAckCks<Container>>(std::move(raw_msg));
            break;
        case msg::MsgType::DataMsg:
            msg.emplace<msg::DataMsg<Container>>(std::move(raw_msg));
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

void MessageHandler::HandleMessage(Message msg)
{
    ++mCtr;
    std::visit(VariantVisit<Container>{}, msg);
}

}  // namespace app
