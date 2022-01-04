#pragma once

#include <array>
#include <cstddef>
#include <memory_resource>
#include <vector>

#include "common_limit.hxx"

namespace mem {

class MemoryResHandler final {
public:
    MemoryResHandler();
    ~MemoryResHandler() = default;

    MemoryResHandler(const MemoryResHandler&) = delete;
    MemoryResHandler& operator=(const MemoryResHandler&) = delete;

    MemoryResHandler(MemoryResHandler&&) = delete;
    MemoryResHandler& operator=(MemoryResHandler&&) = delete;

    std::pmr::vector<std::byte> getVector(const std::vector<std::byte>& buf);

private:
    // 256 kilobytes
    static constexpr size_t sMemPool = 262144;
    static constexpr size_t sNoOfBlksPerAlloc = 1;
    // equals to a max msg size 65536 + 1000 bytes for internal container needs
    static constexpr size_t sMaxBlockSize = limit::max_msg_size + 1000;

    std::array<std::byte, sMemPool> mMemory;
    std::pmr::monotonic_buffer_resource mMemRes;
    std::pmr::unsynchronized_pool_resource mMemPool;
};

}  // namespace mem
