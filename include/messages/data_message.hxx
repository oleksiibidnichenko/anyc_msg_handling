#pragma once

#include <utility>
#include "common_limit.hxx"
#include "control_message.hxx"

namespace msg {

template <typename Container, size_t MaxSize = limit::max_msg_size>
class DataMsg final : public ControlMsg<Container, MaxSize> {
public:
    explicit DataMsg(Container&& stg):
        ControlMsg<Container, MaxSize>(std::forward<Container>(stg)) {};

    DataMsg(const DataMsg&) = delete;
    DataMsg& operator=(const DataMsg&) = delete;

    DataMsg(DataMsg&&) = default;
    DataMsg& operator=(DataMsg&&) = default;

    MsgType GetMsgType() const override
    {
        return MsgType::DataMsg;
    }

    void Reserve(size_t size) override
    {
        ControlMsg<Container, MaxSize>::Reserve(size);
    }
};

}  // namespace msg
