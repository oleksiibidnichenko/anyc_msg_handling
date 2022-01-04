#pragma once

#include <cinttypes>
#if __cplusplus == 201703L
#include <cstddef>
#else
namespace std {
    using byte = unsigned char;
}
#endif

namespace msg {

enum class MsgType : uint16_t {
    ControlMsg = 0x1001,
    ControlMsgAck = 0x1002,
    ControlMsgNack = 0x1003,
    ControlMsgAckCks = 0x1004,
    DataMsg = 0x8001
};

class BaseMsg {
public:
    BaseMsg() = default;
    virtual ~BaseMsg() = default;

    BaseMsg(const BaseMsg&) = delete;
    BaseMsg& operator=(const BaseMsg&) = delete;

    BaseMsg(BaseMsg&&) = default;
    BaseMsg& operator=(BaseMsg&&) = default;

    virtual std::byte* GetRawBuffer() = 0;

    virtual void Clear() = 0;

    virtual size_t Size() const = 0;
    virtual MsgType GetMsgType() const = 0;
    virtual bool isEmpty() const = 0;

protected:
    virtual void Reserve(size_t size) = 0;
};

}  // namespace msg
