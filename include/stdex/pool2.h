#pragma once

#include <stdint.h>

namespace stdex
{
    template<size_t TBlockSize, size_t TBlockCount, class TAllocator>
    class pool2
    {
        struct block_t
        {
            uint8_t buff[TBlockSize];
            block_t * next;
        };

        class chunk_t
        {
        public:
            chunk_t( chunk_t * _prev )
                : prev( _prev )
            {
            }

        public:
            block_t * initialize()
            {
                block_t * free = nullptr;

                for( block_t * it = buffer_block,
                    *it_end = buffer_block + TBlockCount;
                    it != it_end;
                    ++it )
                {
                    it->next = free;
                    free = it;
                }

                return free;
            }

        public:
            chunk_t * getPrev() const
            {
                return prev;
            }

        protected:
            chunk_t * prev;
            block_t buffer_block[TBlockCount];
        };
        
    public:
        pool2()
            : m_chunk( nullptr )
            , m_free( nullptr )
        {
        }

        ~pool2()
        {
            this->clear();
        }

    public:
        void * alloc_block()
        {
            if( m_free == nullptr )
            {
                this->addChunk_();
            }

            block_t * free = m_free;
            m_free = m_free->next;

            void * impl = reinterpret_cast<void *>(free);

            return impl;
        }

        void free_block( void * _buff )
        {
            block_t * block = reinterpret_cast<block_t*>(_buff);

            block->next = m_free;
            m_free = block;
        }

    public:
        void clear()
        {
            chunk_t * chunk = m_chunk;

            while( chunk != nullptr )
            {
                chunk_t * prev = chunk->getPrev();

                TAllocator::s_free( chunk, "pool2" );

                chunk = prev;
            }

            m_chunk = nullptr;
            m_free = nullptr;
        }

    protected:
        void addChunk_()
        {
            const size_t sizeof_chunk_t = sizeof( chunk_t );
            void * mem_chunk = TAllocator::s_malloc( sizeof_chunk_t, "pool2" );
            new (mem_chunk)chunk_t( m_chunk );

            chunk_t * chunk = static_cast<chunk_t *>(mem_chunk);

            m_free = chunk->initialize();
            m_chunk = chunk;
        }

    protected:
        chunk_t * m_chunk;
        block_t * m_free;
    };
}
