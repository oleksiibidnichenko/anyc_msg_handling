#pragma once

#include <utility>
#include "base_message.hxx"

namespace msg {

template <typename Container, size_t MaxSize = 64>
class ControlMsg: public BaseMsg {
public:
    ControlMsg(Container&& stg): m_stg(std::forward<Container>(stg))
    {
        Reserve(MaxSize);
    }

    ControlMsg(const ControlMsg&) = delete;
    ControlMsg& operator=(const ControlMsg&) = delete;

    ControlMsg(ControlMsg&&) = default;
    ControlMsg& operator=(ControlMsg&&) = default;

    std::byte* GetRawBuffer() override
    {
        return m_stg.data();
    }

    void Clear() override
    {
        m_stg.clear();
    }

    size_t Size() const override
    {
        return m_stg.size();
    }

    MsgType GetMsgType() const override
    {
        return MsgType::ControlMsg;
    }

    bool isEmpty() const override
    {
        return m_stg.empty();
    }

protected:
    void Reserve(size_t size) override
    {
        m_stg.reserve(size);
    }

private:
    Container m_stg;
};

}  // namespace msg
