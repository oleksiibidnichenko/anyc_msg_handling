#pragma once

#include <utility>
#include "control_message.hxx"

namespace msg {

template <typename Container, size_t MaxSize = 16>
class ControlMsgAck: public ControlMsg<Container, MaxSize> {
public:
    ControlMsgAck(Container&& stg):
        ControlMsg<Container, MaxSize>(std::forward<Container>(stg)) {};

    ControlMsgAck(const ControlMsgAck&) = delete;
    ControlMsgAck& operator=(const ControlMsgAck&) = delete;

    ControlMsgAck(ControlMsgAck&&) = default;
    ControlMsgAck& operator=(ControlMsgAck&&) = default;

    MsgType GetMsgType() const override
    {
        return MsgType::ControlMsgAck;
    }
};

}  // namespace msg
