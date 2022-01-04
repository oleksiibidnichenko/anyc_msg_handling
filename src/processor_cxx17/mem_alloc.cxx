#include "mem_alloc.hxx"

namespace mem {

MemoryResHandler::MemoryResHandler() :
    mMemRes(mMemory.data(), mMemory.size()),
    mMemPool({sNoOfBlksPerAlloc, sMaxBlockSize}, &mMemRes)
{}

std::pmr::vector<std::byte> MemoryResHandler::getVector(
        const std::vector<std::byte>& buf)
{
    return std::pmr::vector<std::byte>(std::begin(buf), std::end(buf),
            &mMemPool);
}

}  // namespace mem
