#include "msg_handler.hxx"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <vector>

#include "base_message.hxx"
#include "control_message.hxx"
#include "control_message_ack.hxx"
#include "control_message_ack_with_checksum.hxx"
#include "control_message_nack.hxx"
#include "data_message.hxx"

namespace dummy_app {

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

    std::cout << "received a sequence of " << data.size() << " bytes"
        << std::endl;
}

bool MessageHandler::isValidMsgType() const
{
    return true;
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

}  // namespace dummy_app
