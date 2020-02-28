#pragma once

#include <type_traits>
#include <stdint.h>

#include <assert.h>

namespace stdex
{
    template<size_t TBlockSize, size_t TBlockCount, class TAllocator>
    class pool2
    {
        struct block_t
        {
            uint8_t buff[TBlockSize];
            block_t * next;

#ifndef NDEBUG
            uint32_t uid;
#endif
        };

        class chunk_t
        {
        public:
            chunk_t( chunk_t * _prev )
                : prev( _prev )
#ifndef NDEBUG
                , uid( 0 )
#endif
            {
            }

        public:
            void setUID( uint32_t _uid )
            {
                uid = _uid;
            }

        public:
            block_t * initialize()
            {
                block_t * free = nullptr;

                for( block_t * it = reinterpret_cast<block_t *>(buffer_block_storage + 0),
                    *it_end = reinterpret_cast<block_t *>(buffer_block_storage + TBlockCount);
                    it != it_end;
                    ++it )
                {
                    it->next = free;
                    free = it;

#ifndef NDEBUG
                    it->uid = uid;
#endif
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
            typedef typename std::aligned_storage<sizeof( block_t ), alignof(block_t)>::type block_storage_t;
            block_storage_t buffer_block_storage[TBlockCount];

#ifndef NDEBUG
            uint32_t uid;
#endif
        };

    public:
        pool2()
            : m_chunk( nullptr )
            , m_free( nullptr )
#ifndef NDEBUG
            , m_uid( 0 )
#endif
        {
        }

        ~pool2()
        {
            this->clear();
        }

#ifndef NDEBUG
    public:
        void setUID( uint32_t _uid )
        {
            m_uid = _uid;
        }
#endif

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
            block_t * block = reinterpret_cast<block_t *>(_buff);

#ifndef NDEBUG
            assert( m_uid == block->uid );
#endif

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

#ifndef NDEBUG
            chunk->setUID( m_uid );
#endif

            m_free = chunk->initialize();
            m_chunk = chunk;
        }

    protected:
        chunk_t * m_chunk;
        block_t * m_free;

#ifndef NDEBUG
        uint32_t m_uid;
#endif
    };
}
