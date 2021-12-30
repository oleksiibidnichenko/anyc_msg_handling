#pragma once

#include <utility>
#include "control_message_ack.hxx"

namespace msg {

template <typename Container>
class ControlMsgAckCks final: public ControlMsgAck<Container> {
public:
    explicit ControlMsgAckCks(Container&& stg):
        ControlMsgAck<Container>(std::forward<Container>(stg)) {};

    ControlMsgAckCks(const ControlMsgAckCks&) = delete;
    ControlMsgAckCks& operator=(const ControlMsgAckCks&) = delete;

    ControlMsgAckCks(ControlMsgAckCks&&) = default;
    ControlMsgAckCks& operator=(ControlMsgAckCks&&) = default;

    MsgType GetMsgType() const override
    {
        return MsgType::ControlMsgAckCks;
    }
};

}  // namespace msg
