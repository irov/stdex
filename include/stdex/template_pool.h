#pragma once

#include "stdex/pool.h"

#include <new>

namespace stdex
{
    template<class TBlockType, uint32_t TBlockCount, class TAllocator>
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

            return std::launder( t );
        }

        void destroyT( TBlockType * _t )
        {
            _t->~TBlockType();

            m_pool.free_block( _t );
        }

    public:
        void clear()
        {
            m_pool.clear();
        }

    public:
        bool empty() const
        {
            bool result = m_pool.empty();

            return result;
        }

    protected:
        typedef pool<TBlockType, TBlockCount, TAllocator> pool_t;
        pool_t m_pool;
    };
}
