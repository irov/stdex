#pragma once

#include "stdex/typename.h"

#include <type_traits>
#include <cstdint>

namespace stdex
{
    template<class TBlockType, uint32_t TBlockCount>
    struct pool_template_chunk
    {
        typedef pool_template_chunk<TBlockType, TBlockCount> chunk_t;

        struct pool_block
        {
            TBlockType buff;
            pool_block * next;
        };

        pool_block * initialize()
        {
            pool_block * free = nullptr;

            for( pool_block * it = reinterpret_cast<pool_block *>(buffer_block_storage + 0),
                *it_end = reinterpret_cast<pool_block *>(buffer_block_storage + TBlockCount);
                it != it_end;
                ++it )
            {
                it->next = free;
                free = it;
            }

            return free;
        }

        chunk_t * removePrev()
        {
            chunk_t * old = prev;
            prev = nullptr;

            return old;
        }

        chunk_t * getPrev() const
        {
            return prev;
        }

        chunk_t * prev;
        typedef typename std::aligned_storage<sizeof( pool_block ), alignof(pool_block)>::type pool_block_storage;
        pool_block_storage buffer_block_storage[TBlockCount];
    };

    template<class TBlockType, uint32_t TBlockCount, class TAllocator>
    class pool
    {
        typedef pool_template_chunk<TBlockType, TBlockCount> chunk_t;
        typedef typename chunk_t::pool_block block_t;

    public:
        pool()
            : m_chunk( nullptr )
            , m_free( nullptr )
            , m_blockCount( 0 )
            , m_chunkCount( 0 )
        {
        }

        ~pool()
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

            ++m_blockCount;

            void * impl = reinterpret_cast<void *>(free);

            return impl;
        }

        void free_block( void * _buff )
        {
            block_t * block = reinterpret_cast<block_t *>(_buff);

            block->next = m_free;
            m_free = block;

            --m_blockCount;

            if( m_blockCount == 0 && m_chunkCount > 1 )
            {
                this->collapse();
            }
        }

    public:
        uint32_t getBlockCount() const
        {
            return m_blockCount;
        }

        size_t getBlockSize() const
        {
            return sizeof( TBlockType );
        }

        uint32_t getChunkCount() const
        {
            return m_chunkCount;
        }

        uint32_t getBlockTotal() const
        {
            return TBlockCount;
        }

    public:
        bool empty() const
        {
            return m_blockCount == 0;
        }

    public:
        void clear()
        {
            chunk_t * chunk = m_chunk;

            while( chunk != nullptr )
            {
                chunk_t * prev = chunk->getPrev();

                TAllocator::free( chunk, type_name<TBlockType>::value );

                chunk = prev;
            }

            m_chunk = nullptr;
            m_free = nullptr;

            m_blockCount = 0;
            m_chunkCount = 0;
        }

        void collapse()
        {
            chunk_t * chunk = m_chunk->removePrev();

            while( chunk != nullptr )
            {
                chunk_t * prev = chunk->getPrev();

                TAllocator::free( chunk, type_name<TBlockType>::value );

                chunk = prev;
            }

            m_free = m_chunk->initialize();

            m_blockCount = 0;
            m_chunkCount = 1;
        }

    protected:
        void addChunk_()
        {
            const size_t sizeof_chunk_t = sizeof( chunk_t );
            void * mem_chunk = TAllocator::malloc( sizeof_chunk_t, type_name<TBlockType>::value );

            chunk_t * chunk = static_cast<chunk_t *>(mem_chunk);

            chunk->prev = m_chunk;

            m_free = chunk->initialize();
            m_chunk = chunk;

            ++m_chunkCount;
        }

    protected:
        chunk_t * m_chunk;
        block_t * m_free;

        uint32_t m_blockCount;
        uint32_t m_chunkCount;
    };
}
