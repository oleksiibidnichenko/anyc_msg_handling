#pragma once

#include <utility>
#include "control_message.hxx"

namespace msg {

template <typename Container, size_t MaxSize = 12>
class ControlMsgNack final: public ControlMsg<Container, MaxSize> {
public:
    explicit ControlMsgNack(Container&& stg):
        ControlMsg<Container, MaxSize>(std::forward<Container>(stg)) {};

    ControlMsgNack(const ControlMsgNack&) = delete;
    ControlMsgNack& operator=(const ControlMsgNack&) = delete;

    ControlMsgNack(ControlMsgNack&&) = default;
    ControlMsgNack& operator=(ControlMsgNack&&) = default;

    MsgType GetMsgType() const override
    {
        return MsgType::ControlMsgNack;
    }
};

}  // namespace msg
