#	pragma once

namespace stdex
{
	template<size_t TBlockSize, size_t TBlockCount>
	class pool_template_chunk
	{
    public:
        typedef pool_template_chunk<TBlockSize, TBlockCount> chunk_type;

	public:
		struct pool_block
		{
			char buff[TBlockSize];
            chunk_type * chunk;
			pool_block * next;
		};

	public:
		pool_template_chunk()
            : m_prev(nullptr)
            , m_next(nullptr)
            , m_freeBlock(nullptr)
            , m_countBlock(TBlockCount)
		{
            pool_block * block = nullptr;

			for( pool_block * it = buffer_block, 
				*it_end = buffer_block + TBlockCount; 
				it != it_end; 
			++it )
			{
                it->chunk = this;
				it->next = block;
				block = it;
			}

            m_freeBlock = block;
		}

        pool_block * alloc()
        {
            pool_block * block = m_freeBlock;
            m_freeBlock = m_freeBlock->next;

            --m_countBlock;

            return block;
        }

        void free( pool_block * _block )
        {
            _block->next = m_freeBlock;
            m_freeBlock = _block;

            ++m_countBlock;
        }

    public:
        void unlink()
        {
            if( m_prev != nullptr )
            {
                m_prev->m_next = m_next;
            }

            if( m_next != nullptr )
            {
                m_next->m_prev = m_prev;
            }

            m_prev = nullptr;
            m_next = nullptr;
        }

        void push_front( chunk_type * _chunk )
        {
            m_prev = nullptr;
            m_next = _chunk;

            _chunk->m_prev = this;
        }

    public:
        chunk_type * getNext() const
        {
            return m_next;
        }

        size_t getCountBlock() const
        {
            return m_countBlock;
        }

    protected:
		pool_block buffer_block[TBlockCount];

    protected:
        chunk_type * m_prev;
        chunk_type * m_next;

        pool_block * m_freeBlock;

        size_t m_countBlock;
	};

	template<size_t TBlockSize, size_t TBlockCount>
	class pool
	{
		typedef pool_template_chunk<TBlockSize, TBlockCount> chunk_type;
		typedef typename chunk_type::pool_block block_type;

    public:
        pool()
            : m_freeChunk(nullptr)
            , m_fullChunk(nullptr)
            , m_emptyChunk(nullptr)
            , m_countBlock(0)
            , m_countBlockMax(0)
            , m_countChunk(0)
        {
        }

        ~pool()
        {
            chunk_type * chunk = m_freeChunk;

            while( chunk != nullptr )
            {
                chunk_type * next = chunk->getNext();

                delete chunk;

                chunk = next;
            }

            chunk = m_fullChunk;

            while( chunk != nullptr )
            {
                chunk_type * next = chunk->getNext();

                delete chunk;

                chunk = next;
            }

            chunk = m_emptyChunk;

            while( chunk != nullptr )
            {
                chunk_type * next = chunk->getNext();

                delete chunk;

                chunk = next;
            }
        }

	public:
		void * alloc()
		{
			if( m_freeChunk == nullptr )
			{
                this->addChunk_();
			}

			block_type * free = m_freeChunk->alloc();

            ++m_countBlock;

            this->updateAllockChunks_();
            
            void * impl = static_cast<void *>(free);

			return impl;
		}

		void free( void * _buff )
		{
			block_type * block = reinterpret_cast<block_type*>(_buff);
            
            chunk_type * chunk = block->chunk;
			chunk->free( block );
                        
            --m_countBlock;

            this->updateFreeChunks_( chunk );
		}

        bool empty() const
        {
            return m_countBlock == 0;
        }

	protected:
        void updateAllockChunks_()
        {
            size_t chunkCountBlock = m_freeChunk->getCountBlock();

            if( chunkCountBlock != 0 )
            {
                return;
            }
             
            chunk_type * free = m_freeChunk;
            m_freeChunk = m_freeChunk->getNext();

            free->unlink();

            if( m_fullChunk != nullptr )
            {
                free->push_front( m_fullChunk );
            }

            m_fullChunk = free;
        }

        void updateFreeChunks_( chunk_type * _chunk )
        {
            const size_t chunkCount = TBlockCount;

            size_t chunkCountBlock = _chunk->getCountBlock();

            if( chunkCountBlock == chunkCount )
            {
                if( m_freeChunk == _chunk )
                {
                    m_freeChunk = m_freeChunk->getNext();
                }
                else if( m_fullChunk == _chunk )
                {
                    m_fullChunk = m_fullChunk->getNext();
                }

                _chunk->unlink();

                if( m_emptyChunk != nullptr )
                {
                    _chunk->push_front( m_emptyChunk );
                }

                m_emptyChunk = _chunk;
            }

            this->updateEmptyChunk();
        }

        void updateEmptyChunk()
        {
            if( m_emptyChunk == nullptr )
            {
                return;
            }

            const size_t chunkCount2 = TBlockCount * 2;

            if( m_countBlockMax - m_countBlock > chunkCount2 )
            {
                chunk_type * free = m_emptyChunk;
                m_emptyChunk = m_emptyChunk->getNext();

                free->unlink();

                delete free;

                --m_countChunk;
                m_countBlockMax -= TBlockCount;
            }
        }

		void addChunk_()
		{
            chunk_type * chunk = nullptr;

            if( m_emptyChunk != nullptr )
            {
                chunk = m_emptyChunk;
                m_emptyChunk = m_emptyChunk->getNext();

                chunk->unlink();
            }
            else
            {
			    chunk = new chunk_type();

                ++m_countChunk;

                const size_t chunkCount = TBlockCount;

                m_countBlockMax += chunkCount;
            }

            if( m_freeChunk != nullptr )
            {
                chunk->push_front( m_freeChunk );
            }

            m_freeChunk = chunk;
		}

    public:
        size_t getCountBlock() const
        {
            return m_countBlock;
        }

        size_t getCountBlockMax() const
        {
            return m_countBlockMax;
        }

        size_t getBlockSize() const
        {
            return TBlockSize;
        }

        size_t getBlockCount() const
        {
            return TBlockCount;
        }

	protected:
		chunk_type * m_freeChunk;
        chunk_type * m_fullChunk;
        chunk_type * m_emptyChunk;

        size_t m_countBlock;
        size_t m_countBlockMax;
        size_t m_countChunk;
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
        typedef pool<sizeof(T), TBlockCount> pool_type;
        pool_type m_pool;
    };
}
