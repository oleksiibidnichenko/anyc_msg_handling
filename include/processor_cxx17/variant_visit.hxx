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
        std::cerr << "Handling Control Message."
                  << " First byte" << std::hex
                  << static_cast<uint8_t>(*msg.GetRawBuffer())
                  << std::endl;
    }

    void operator()(msg::ControlMsgAck<Container>& msg)
    {
        std::cerr << "Handling Control Acknowledgement Message."
                  << " First byte" << std::hex
                  << static_cast<uint8_t>(*msg.GetRawBuffer())
                  << std::endl;
    }

    void operator()(msg::ControlMsgNack<Container>& msg)
    {
        std::cerr << "Handling Control Negative Acknowledgement Message."
                  << " First byte" << std::hex
                  << static_cast<uint8_t>(*msg.GetRawBuffer())
                  << std::endl;
    }

    void operator()(msg::ControlMsgAckCks<Container>& msg)
    {
        std::cerr << "Handling Control Acknowledgement with Checksum"
                  << " Message. First byte" << std::hex
                  << static_cast<uint8_t>(*msg.GetRawBuffer())
                  << std::endl;
    }

    void operator()(msg::DataMsg<Container>& msg)
    {
        std::cerr << "Handling Data Message."
                  << " First byte" << std::hex
                  << static_cast<uint8_t>(*msg.GetRawBuffer())
                  << std::endl;
    }
};

}  // namespace app
