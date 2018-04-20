#   pragma once

#	include "stdex/allocator.h"

namespace stdex
{
    template<class T>
    class heap_array
    {
	public:
		typedef size_t size_type;
		typedef T value_type;

    public:
        heap_array()
			: m_buff(nullptr)
			, m_current(nullptr)
			, m_last(nullptr)
        {

        }

		~heap_array()
		{
			stdex_free( m_buff, typeid(T).name() );
		}

	public:
		void reserve( size_t _size )
		{
			this->allocate_buff_( _size );
						
			m_last = m_buff + _size;
			m_current = m_buff;
		}

		void resize( size_t _size )
		{
			this->allocate_buff_( _size );
						
			m_last = m_buff + _size;
			m_current = m_last;
		}

	protected:
		void allocate_buff_( size_t _size )
		{
			size_t memory_size = sizeof(T) * _size;
			void * memory = stdex_malloc( memory_size, typeid(T).name() );
			m_buff = static_cast<T *>(memory);
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

		T * emplace_count( size_t _count )
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
            return m_current == m_last;
        }

        size_t size() const
        {
            return m_current - m_buff;
        }

		size_t capacity() const
		{
			return m_last - m_buff;
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
        T * m_buff;
        T * m_current;
		T * m_last;
    };
}