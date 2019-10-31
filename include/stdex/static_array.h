#pragma once

#include <stdint.h>

namespace stdex
{
    template<class T, uint32_t Size>
    class static_array
    {
    public:
        typedef uint32_t size_type;
        typedef T value_type;

    public:
        static_array()
        {
            m_current = m_buff;
        }

    public:
        T * buff()
        {
            return m_buff;
        }

        const T * buff() const
        {
            return m_buff;
        }

        T & back()
        {
            return *(m_current - 1);
        }

        const T & back() const
        {
            return *(m_current - 1);
        }

    public:
        typedef T * iterator;
        typedef const T * const_iterator;

    public:
        iterator begin()
        {
            return m_buff;
        }

        const_iterator begin() const
        {
            return m_buff;
        }

        iterator end()
        {
            return m_current;
        }

        const_iterator end() const
        {
            return m_current;
        }

        iterator advance( size_type _index )
        {
            return m_buff + _index;
        }

        const_iterator advance( size_type _index ) const
        {
            return m_buff + _index;
        }

    public:
        T & emplace()
        {
            T * element = m_current;
            ++m_current;

            return *element;
        }

        T * emplace_count( uint32_t _count )
        {
            T * element = m_current;
            m_current += _count;

            return element;
        }

        void clear()
        {
            m_current = m_buff;
        }

        bool empty() const
        {
            return m_current == m_buff;
        }

        bool full() const
        {
            const T * end = m_buff + Size;

            return m_current == end;
        }

        size_type size() const
        {
            size_type countElement = m_current - m_buff;

            return countElement;
        }

        size_type capacity() const
        {
            return Size;
        }

    public:
        T & operator [] ( size_type _index )
        {
            return m_buff[_index];
        }

        const T & operator [] ( size_type _index ) const
        {
            return m_buff[_index];
        }

    protected:
        T m_buff[Size];
        T * m_current;
    };
}