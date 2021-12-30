#include "common_msg_handler.hxx"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <net/if.h>

#include "control_message.hxx"
#include "control_message_ack.hxx"
#include "control_message_ack_with_checksum.hxx"
#include "control_message_nack.hxx"
#include "data_message.hxx"

namespace app {

CommonMessageHandler::CommonMessageHandler(boost::asio::io_context &io_context):
    net::TcpServerConnection(io_context)
{
}

bool CommonMessageHandler::isValidMsgType() const
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

uint32_t CommonMessageHandler::getBodySize() const noexcept
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

void CommonMessageHandler::onConnTerm() noexcept
{
    // do nothing by default
}

uint16_t CommonMessageHandler::getMsgType() const
{
    if (mInputBuffer.size() < sMsgTypeLen) {
        return 0;
    }

    uint16_t msg_type = 0;

    std::memcpy(static_cast<void*>(&msg_type),
            static_cast<const void*>(mInputBuffer.data()), sizeof(uint16_t));

    return ntohs(msg_type);
}

}  // namespace app
