#	pragma once

#	include "allocator.h"

#	include <stdint.h>
#	include <new>

namespace stdex
{
    template<class TBlockType, size_t TBlockCount>
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

			for( pool_block * it = buffer_block, 
				*it_end = buffer_block + TBlockCount; 
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
        pool_block buffer_block[TBlockCount];        
    };

	class stdex_pool_allocator_default
	{		
	public:
		static void * s_malloc( size_t _size )
		{
			return stdex_malloc( _size );
		}

		static void * s_realloc( void * _mem, size_t _size )
		{
			return stdex_realloc( _mem, _size );
		}
		
		static void s_free( void * _ptr )
		{
			return stdex_free( _ptr );
		}
	};

	template<class TBlockType, size_t TBlockCount, class TAllocator = stdex_pool_allocator_default>
    class pool
    {
        typedef pool_template_chunk<TBlockType, TBlockCount> chunk_t;
        typedef typename chunk_t::pool_block block_t;

    public:
        pool()
            : m_chunk(nullptr)
            , m_free(nullptr)
            , m_blockCount(0)
            , m_chunkCount(0)
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
            block_t * block = reinterpret_cast<block_t*>(_buff);

            block->next = m_free;
            m_free = block;

			--m_blockCount;
			
			if( m_blockCount == 0 && m_chunkCount > 1 )
			{
				this->collapse();
			}
        }

	public:
        size_t getBlockCount() const
        {
            return m_blockCount;
        }

        size_t getBlockSize() const
        {
            return sizeof( TBlockType );
        }

        size_t getChunkCount() const
        {
            return m_chunkCount;
        }

        size_t getBlockTotal() const
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

				TAllocator::s_free( chunk );

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

				TAllocator::s_free( chunk );

				chunk = prev;
			}

			m_free = m_chunk->initialize();

			m_blockCount = 0;
			m_chunkCount = 1;
		}

    protected:
        void addChunk_()
        {
			void * mem_chunk = TAllocator::s_malloc( sizeof( chunk_t ) );

			chunk_t * chunk = static_cast<chunk_t *>(mem_chunk);

            chunk->prev = nullptr;

			m_free = chunk->initialize();
            m_chunk = chunk;

            ++m_chunkCount;
        }

    protected:
        chunk_t * m_chunk;
        block_t * m_free;

        size_t m_blockCount;
        size_t m_chunkCount;
    };

    template<class TBlockType, size_t TBlockCount>
    class template_pool
    {
    public:
        template_pool()
        {
        }

        ~template_pool()
        {
        }

    public:
        TBlockType * createT()
        {
            void * impl = m_pool.alloc_block();

            TBlockType * t = new (impl) TBlockType();

            return t;
        }

        void destroyT( TBlockType * _t )
        {
            _t->~TBlockType();

            m_pool.free_block( _t );
        }

    public:
        bool empty() const
        {
            bool result = m_pool.empty();

            return result;
        }

    protected:
        typedef pool<TBlockType, TBlockCount> pool_t;
        pool_t m_pool;
    };
}
