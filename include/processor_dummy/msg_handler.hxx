#pragma once

#include <memory>
#include <boost/asio.hpp>

#include "common_msg_handler.hxx"

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
};

}  // namespace app
