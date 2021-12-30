#pragma once

#include <utility>
#include "control_message.hxx"

namespace msg {

template <typename Container, size_t MaxSize = 65536>
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
