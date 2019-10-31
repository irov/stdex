#pragma once

#include <stdint.h>

namespace stdex
{
    template<class T, class Equal, uint32_t Size, uint32_t ProbeStep>
    class static_hash_array
    {
    public:
        typedef uint32_t hash_type;
        typedef uint32_t size_type;
        typedef K key_type;
        typedef T value_type;

    public:
        static_hash_array()
            : m_count( 0 )
        {
        }

        ~static_hash_array()
        {
        }

    public:
        void setDummy( const T & _dummy )
        {
            m_dummy = _dummy;
        }

    public:
        void clear()
        {
            for( uint32_t index = 0; index != Size; ++index )
            {
                m_buff[index] = nullptr;
            }

            m_count = 0;
        }

        bool empty() const
        {
            return m_count == 0;
        }

        bool full() const
        {
            return m_count == Size;
        }

        size_type size() const
        {
            return m_count;
        }

        size_type capacity() const
        {
            return Size;
        }

    public:
        void insert( hash_type _hash, const T & _t )
        {
            uint32_t slot = _hash % Size;

            for( uint32_t probe = slot; probe != Size; probe = (probe + ProbeStep) % Size )
            {
                T & element = m_buff[probe];

                if( element == nullptr && element != m_dummy )
                {
                    break;
                }

                element = _t;

                ++m_count;

                break;
            }
        }

        bool has( hash_type _hash ) const
        {
            uint32_t slot = _hash % Size;

            for( uint32_t probe = slot; probe != Size; probe = (probe + ProbeStep) % Size )
            {
                const T & element = m_buff[probe];

                if( element == nullptr )
                {
                    break;
                }

                if( Equal()(element, _t) == false )
                {
                    continue;
                }

                return true;
            }

            return false;
        }

        const T & get( hash_type _hash ) const
        {
            uint32_t slot = _hash % Size;

            for( uint32_t probe = slot; probe != Size; probe = (probe + ProbeStep) % Size )
            {
                const T & element = m_buff[probe];

                if( element == nullptr )
                {
                    break;
                }

                if( Equal()(element, _t) == false )
                {
                    continue;
                }

                return element;
            }

            return m_dummy;
        }

        void remove( hash_type _hash, const T & _t )
        {
            uint32_t slot = _hash % Size;

            for( uint32_t probe = slot; probe != Size; probe = (probe + ProbeStep) % Size )
            {
                T & element = m_buff[probe];

                if( element == nullptr )
                {
                    break;
                }

                if( Equal()(element, _t) == false )
                {
                    continue;
                }

                element = m_dummy;

                --m_count;

                break;
            }
        }

    protected:
        T m_dummy;
        T m_buff[Size];
        size_type m_count;
    };
}