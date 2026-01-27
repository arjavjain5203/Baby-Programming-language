#pragma once
#include <cstddef>
#include <cstdlib>

class ArenaAllocation
{
    public:
        inline explicit ArenaAllocation(size_t byte)
            : m_block_size(byte)
        {
            m_buffer = malloc(m_block_size);
            m_current_ptr = static_cast<std::byte*>(m_buffer);
        }

        template <typename T>
        inline T* alloc()
        {
            void*offset=m_current_ptr;
            m_current_ptr += sizeof(T);

            return static_cast<T*>(offset);
        }


        inline ArenaAllocation(const ArenaAllocation&) = delete;
        inline ArenaAllocation& operator=(const ArenaAllocation&) = delete;

        inline ~ArenaAllocation()
        {
            free(m_buffer);
        }


    private:
        void* m_buffer;
        size_t m_block_size;
        std::byte* m_current_ptr;
};