#pragma once

#include <iostream>
#include <variant>

#include "control_message.hxx"
#include "control_message_ack.hxx"
#include "control_message_ack_with_checksum.hxx"
#include "control_message_nack.hxx"
#include "data_message.hxx"

namespace app {

template <typename Container>
struct VariantVisit final {
    void operator()(std::monostate&)
    {
        // do nothing
    }

    void operator()(msg::ControlMsg<Container>& msg)
    {
#ifdef NDEBUG
    static_cast<void>(msg);
#else
    std::cout << "Handling Control Message."
              << " First byte" << std::hex
              << static_cast<uint8_t>(*msg.GetRawBuffer())
              << std::endl;
#endif
    }

    void operator()(msg::ControlMsgAck<Container>& msg)
    {
#ifdef NDEBUG
    static_cast<void>(msg);
#else
    std::cout << "Handling Control Acknowledgement Message."
              << " First byte" << std::hex
              << static_cast<uint8_t>(*msg.GetRawBuffer())
              << std::endl;
#endif
    }

    void operator()(msg::ControlMsgNack<Container>& msg)
    {
#ifdef NDEBUG
    static_cast<void>(msg);
#else
    std::cout << "Handling Control Negative Acknowledgement Message."
              << " First byte" << std::hex
              << static_cast<uint8_t>(*msg.GetRawBuffer())
              << std::endl;
#endif
    }

    void operator()(msg::ControlMsgAckCks<Container>& msg)
    {
#ifdef NDEBUG
    static_cast<void>(msg);
#else
    std::cout << "Handling Control Acknowledgement with Checksum"
              << " Message. First byte" << std::hex
              << static_cast<uint8_t>(*msg.GetRawBuffer())
              << std::endl;
#endif
    }

    void operator()(msg::DataMsg<Container>& msg)
    {
#ifdef NDEBUG
    static_cast<void>(msg);
#else
    std::cout << "Handling Data Message."
              << " First byte" << std::hex
              << static_cast<uint8_t>(*msg.GetRawBuffer())
              << std::endl;
#endif
    }
};

}  // namespace app
