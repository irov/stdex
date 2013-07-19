#	pragma once

namespace stdex
{
    template<size_t TBlockSize, size_t TBlockCount>
    class pool_template_chunk
    {
    public:
        typedef pool_template_chunk<TBlockSize, TBlockCount> chunk_t;

    public:
        struct pool_block
        {
            char buff[TBlockSize];
            pool_block * next;
        };

    public:
        pool_template_chunk( pool_block ** _free, chunk_t * _prev )
            : prev(_prev)
        {
            for( pool_block * it = buffer_block, 
                *it_end = buffer_block + TBlockCount; 
                it != it_end; 
            ++it )
            {
                it->next = *_free;
                *_free = it;
            }
        }

    public:
        chunk_t * getPrev() const
        {
            return prev;
        }

    protected:
        pool_block buffer_block[TBlockCount];
        chunk_t * prev;
    };

    template<size_t TBlockSize, size_t TBlockCount>
    class pool
    {
        typedef pool_template_chunk<TBlockSize, TBlockCount> chunk_t;
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
            chunk_t * chunk = m_chunk;

            while( chunk != nullptr )
            {
                chunk_t * prev = chunk->getPrev();

                delete chunk;

                chunk = prev;
            }
        }

    public:
        void * alloc()
        {
            if( m_free == nullptr )
            {
                this->addChunk_();
            }

            block_t * free = m_free;
            m_free = m_free->next;

            ++m_blockCount;

            void * impl = static_cast<void *>(free);

            return impl;
        }

        void free( void * _buff )
        {
            block_t * block = reinterpret_cast<block_t*>(_buff);

            block->next = m_free;

            m_free = block;
            --m_blockCount;
        }

        size_t getBlockCount() const
        {
            return m_blockCount;
        }

        size_t getBlockSize() const
        {
            return TBlockSize;
        }

        size_t getChunkCount() const
        {
            return m_chunkCount;
        }

        size_t getBlockTotal() const
        {
            return TBlockCount;
        }

    protected:
        void addChunk_()
        {
            chunk_t * chunk = new chunk_t(&m_free, m_chunk);

            m_chunk = chunk;

            ++m_chunkCount;
        }

    protected:
        chunk_t * m_chunk;
        block_t * m_free;

        size_t m_blockCount;
        size_t m_chunkCount;
    };

    template<class T, size_t TBlockCount>
    class template_pool
    {
    public:
        template_pool()
        {
        }

    public:
        T * createT()
        {
            void * impl = m_pool.alloc();

            T * t = new (impl) T;

            return t;
        }

        void destroyT( T * _t )
        {
            _t->~T();

            m_pool.free( _t );
        }

    public:
        bool empty() const
        {
            bool result = m_pool.empty();

            return result;
        }

    protected:
        typedef pool<sizeof(T), TBlockCount> pool_t;
        pool_t m_pool;
    };
}
