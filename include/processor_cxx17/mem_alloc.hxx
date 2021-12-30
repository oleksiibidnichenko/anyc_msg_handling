#pragma once

#include <cstddef>
#include <memory_resource>
#include <vector>

namespace mem {

class MemoryResHandler final {
public:
    MemoryResHandler() = default;
    ~MemoryResHandler() = default;

    MemoryResHandler(const MemoryResHandler&) = delete;
    MemoryResHandler& operator=(const MemoryResHandler&) = delete;

    MemoryResHandler(MemoryResHandler&&) = delete;
    MemoryResHandler& operator=(MemoryResHandler&&) = delete;

    std::pmr::vector<std::byte> getVector(const std::vector<std::byte>& buf);

private:
    std::pmr::unsynchronized_pool_resource mMemPool;
};

}  // namespace mem
