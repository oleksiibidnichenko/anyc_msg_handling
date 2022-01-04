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

#ifdef NDEBUG
    static_cast<void>(data);
#else
    std::cout << "received a sequence of " << data.size() << " bytes"
        << std::endl;
#endif
}

}  // namespace app
