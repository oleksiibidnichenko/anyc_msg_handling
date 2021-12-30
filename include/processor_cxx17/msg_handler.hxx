#pragma once

#include <boost/asio.hpp>

#include "common_msg_handler.hxx"

#include "mem_alloc.hxx"
#include "variant_visit.hxx"

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

    void onConnTerm() noexcept override;

private:
    using Container = std::pmr::vector<std::byte>;
    using Message = std::variant<std::monostate,
                                 msg::ControlMsg<Container>,
                                 msg::ControlMsgAck<Container>,
                                 msg::ControlMsgNack<Container>,
                                 msg::ControlMsgAckCks<Container>,
                                 msg::DataMsg<Container>>;

    void HandleMessage(Message msg);

    mem::MemoryResHandler mMemRes;
    size_t mCtr;
};

}  // namespace app
