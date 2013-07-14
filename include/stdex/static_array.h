#   pragma once

namespace stdex
{
    template<class T, size_t I>
    class static_array
    {
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

        iterator advance( size_t _index )
        {
            return m_buff + _index;
        }

        const_iterator advance( size_t _index ) const
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
            const T * end = m_buff + I;
            return m_current == end;
        }

        size_t size() const
        {
            return m_current - m_buff;
        }

    public:
        T & operator [] ( size_t _index )
        {
            return m_buff[_index];
        }

        const T & operator [] ( size_t _index ) const
        {
            return m_buff[_index];
        }

    protected:
        T m_buff[I];
        T * m_current;
    };
}