#   pragma once

namespace stdex
{
    template<class T>
    class auto_array
    {
    public:
        typedef T * iterator;
        typedef const T * const_iterator;

    public:
        auto_array()
            : m_begin(nullptr)
            , m_end(nullptr)
        {
        }

        auto_array( const auto_array & _array )
            : m_begin(_array.m_begin)
            , m_end(_array.m_end)
        {
            _array.m_begin = nullptr;
            _array.m_end = nullptr;
        }

        ~auto_array()
        {
            delete [] m_begin;
        }

    public:
        iterator begin()
        {
            return m_begin;
        }

        iterator end()
        {
            return m_end;
        }

    public:
        const_iterator begin() const
        {
            return m_begin;
        }

        const_iterator end() const
        {
            return m_end;
        }

    public:
        size_t size() const
        {
            return m_end - m_begin;
        }

        bool empty() const
        {
            return m_end == m_begin;
        }

    public:
        T * operator [] ( size_t _index )
        {
            return m_begin[_index];
        }

    public:
        void reserve( size_t _size )
        {
            size_t current_size = this->size();
            if( current_size >= _size )
            {
                return;
            }

            m_begin = new T[_size];
            m_end = m_begin;
        }

    public:
        void push_back( const T & _value )
        {
            *m_end = _value;

            ++m_end;
        }

        T & emplace_back()
        {
            T * emplace = m_end;
            ++m_end;

            return *emplace;
        }

    protected:
        T * m_begin;
        T * m_end;
    };
}