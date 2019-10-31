#pragma once

#include "stdex/stl_vector.h"
#include "stdex/stl_allocator.h"

#include <functional>

namespace stdex
{
    namespace helper
    {
        //////////////////////////////////////////////////////////////////////////
        template<class C, class Key, class L>
        inline size_t binary_vector_binary_lower_bound( const C & _container, const Key & _key, size_t _begin, size_t _count )
        {
            while( _count > 0 )
            {
                size_t it = _begin;
                size_t step = _count / 2;
                it += step;

                const typename C::value_type & buffer_it = _container[it];

                const Key & key_it = buffer_it.key;

                if( L()(key_it, _key) == true )
                {
                    _begin = ++it;
                    _count -= step + 1;
                }
                else
                {
                    _count = step;
                }
            }

            return _begin;
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class Key, class L>
        inline size_t binary_vector_binary_search( const C & _container, const Key & _key, size_t _begin, size_t _end )
        {
            while( _begin < _end )
            {
                size_t middle = (_begin + _end) / 2;

                const typename C::value_type & buffer_middle = _container[middle];

                const Key & middle_key = buffer_middle.key;

                if( L()(middle_key, _key) == true )
                {
                    _begin = middle + 1;
                }
                else
                {
                    _end = middle;
                }
            }

            if( _begin == _end )
            {
                const typename C::value_type & buffer_begin = _container[_begin];

                const Key & begin_key = buffer_begin.key;

                if( L()(_key, begin_key) == false && L()(begin_key, _key) == false )
                {
                    return _begin;
                }
            }

            return (size_t)-1;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    template<class Key, class T, class L = std::less<Key> >
    class binary_vector
    {
    public:
        typedef stdex::stl_allocator<size_t> free_type_allocator;
        typedef stdex::stl_allocator<T> strore_type_allocator;

        typedef std::vector<size_t, free_type_allocator> free_type;
        typedef std::vector<T, strore_type_allocator> strore_type;

    public:
        struct binary_value_store_type
        {
            Key key;
            size_t index;
        };

        typedef stdex::stl_allocator<binary_value_store_type> binary_value_store_type_allocator;

        typedef std::vector<binary_value_store_type, binary_value_store_type_allocator> buffer_type;

    public:
        typedef typename buffer_type::iterator iterator;
        typedef typename buffer_type::const_iterator const_iterator;

    public:
        typedef struct
        {
            iterator first;
            bool second;
        } insert_type;

    protected:
        iterator binary_vector_binary_search( const Key & _key )
        {
            size_t count = m_buffer.size();

            if( count == 0 )
            {
                return m_buffer.end();
            }

            size_t index = helper::binary_vector_binary_search<buffer_type, Key, L>( m_buffer, _key, 0, count - 1 );

            if( index == (size_t)-1 )
            {
                return m_buffer.end();
            }

            iterator it = m_buffer.begin();
            std::advance( it, index );

            return it;
        }

        const_iterator binary_vector_binary_search( const Key & _key ) const
        {
            size_t count = m_buffer.size();

            if( count == 0 )
            {
                return m_buffer.end();
            }

            size_t index = helper::binary_vector_binary_search<buffer_type, Key, L>( m_buffer, _key, 0, count - 1 );

            if( index == (size_t)-1 )
            {
                return m_buffer.end();
            }

            const_iterator it = m_buffer.begin();
            std::advance( it, index );

            return it;
        }

    protected:
        iterator binary_vector_binary_lower_bound( const Key & _key )
        {
            size_t count = m_buffer.size();

            if( count == 0 )
            {
                return m_buffer.end();
            }

            size_t index = helper::binary_vector_binary_lower_bound<buffer_type, Key, L>( m_buffer, _key, 0, count );

            iterator it = m_buffer.begin();
            std::advance( it, index );

            return it;
        }

        const_iterator binary_vector_binary_lower_bound( const Key & _key ) const
        {
            size_t count = m_buffer.size();

            if( count == 0 )
            {
                return m_buffer.end();
            }

            size_t index = helper::binary_vector_binary_lower_bound<buffer_type, Key, L>( m_buffer, _key, 0, count );

            const_iterator it = m_buffer.begin();
            std::advance( it, index );

            return it;
        }

    public:
        binary_vector()
        {
        }

    public:
        void reserve( size_t _size )
        {
            m_store.reserve( _size );
            m_buffer.reserve( _size );
        }

        size_t size() const
        {
            size_t size = m_buffer.size();

            return size;
        }

        void clear()
        {
            m_store.clear();
            m_buffer.clear();
            m_free.clear();
        }

        bool empty() const
        {
            bool empty = m_buffer.empty();

            return empty;
        }

    public:
        iterator begin()
        {
            iterator it = m_buffer.begin();

            return it;
        }

        iterator end()
        {
            iterator it = m_buffer.end();

            return it;
        }

        const_iterator begin() const
        {
            const_iterator it = m_buffer.begin();

            return it;
        }

        const_iterator end() const
        {
            const_iterator it = m_buffer.end();

            return it;
        }

    public:
        void set_value( iterator _it, const T & _value )
        {
            size_t index = _it->index;

            m_store[index] = _value;
        }

        T & get_value( iterator _it ) const
        {
            size_t index = _it->index;

            T & value = m_store[index];

            return value;
        }

        T & get_value( const_iterator _it ) const
        {
            size_t index = _it->index;

            T & value = m_store[index];

            return value;
        }

        const Key & get_key( const_iterator _it ) const
        {
            return _it->key;
        }

    protected:
        size_t story_value_( const T & _value )
        {
            size_t index;

            if( m_free.empty() == true )
            {
                index = m_store.size();
                m_store.push_back( _value );
            }
            else
            {
                index = m_free.back();
                m_free.pop_back();

                m_store[index] = _value;
            }

            return index;
        }

    public:
        insert_type insert( const Key & _key, const T & _value )
        {
            iterator it_lower_bound = this->binary_vector_binary_lower_bound( _key );

            if( it_lower_bound != m_buffer.end() )
            {
                const Key & lower_bound_key = it_lower_bound->key;
                if( L()(_key, lower_bound_key) == false )
                {
                    insert_type ret = {it_lower_bound, false};

                    return ret;
                }
            }

            size_t index = this->story_value_( _value );

            binary_value_store_type bvst = {_key, index};
            iterator it_insert = m_buffer.insert( it_lower_bound, bvst );

            insert_type ret = {it_insert, true};

            return ret;
        }

        iterator erase( iterator _it )
        {
            size_t index = _it->index;
            m_free.push_back( index );
            m_store[index] = T();

            iterator new_it = m_buffer.erase( _it );

            return new_it;
        }

        bool erase( const Key & _key )
        {
            iterator it_found = this->find( _key );

            if( it_found == this->end() )
            {
                return false;
            }

            this->erase( it_found );

            return true;
        }

    public:
        bool exist( const Key & _key ) const
        {
            const_iterator it_found = this->find( _key );

            bool result = (it_found != this->end());

            return result;
        }

        bool has( const Key & _key, T ** _value ) const
        {
            const_iterator it_found = this->find( _key );

            if( it_found == this->end() )
            {
                return false;
            }

            if( _value != nullptr )
            {
                T & value = this->get_value( it_found );

                *_value = &value;
            }

            return true;
        }

        bool has( const Key & _key, const T ** _value ) const
        {
            const_iterator it_found = this->find( _key );

            if( it_found == this->end() )
            {
                return false;
            }

            if( _value != nullptr )
            {
                const T & value = this->get_value( it_found );

                *_value = &value;
            }

            return true;
        }

        bool has_copy( const Key & _key, T & _value ) const
        {
            const_iterator it_found = this->find( _key );

            if( it_found == this->end() )
            {
                return false;
            }

            const T & value = this->get_value( it_found );

            _value = value;

            return true;
        }

    public:
        iterator find( const Key & _key )
        {
            iterator it_lower_bound = this->binary_vector_binary_search( _key );

            iterator it_end = this->end();

            if( it_lower_bound == it_end )
            {
                return it_end;
            }

            return it_lower_bound;
        }

        const_iterator find( const Key & _key ) const
        {
            const_iterator it_lower_bound = this->binary_vector_binary_search( _key );

            const_iterator it_end = this->end();

            if( it_lower_bound == it_end )
            {
                return it_end;
            }

            return it_lower_bound;
        }

        const buffer_type & get_buffer_() const
        {
            return m_buffer;
        }

    protected:
        buffer_type m_buffer;
        mutable strore_type m_store;
        free_type m_free;
    };

    template<class Key, class T, class L>
    class binary_vector<Key, T *, L>
    {
    public:
        struct binary_value_store_type
        {
            Key key;
            T * value;
        };

        typedef stdex::stl_allocator<binary_value_store_type> binary_value_store_type_allocator;

        typedef std::vector<binary_value_store_type, binary_value_store_type_allocator> buffer_type;

        typedef typename buffer_type::iterator iterator;
        typedef typename buffer_type::const_iterator const_iterator;

        typedef struct
        {
            iterator first;
            bool second;
        } insert_type;

    protected:
        iterator binary_vector_binary_search( const Key & _key )
        {
            size_t count = m_buffer.size();

            if( count == 0 )
            {
                return m_buffer.end();
            }

            size_t index = helper::binary_vector_binary_search<buffer_type, Key, L>( m_buffer, _key, 0, count - 1 );

            if( index == (size_t)-1 )
            {
                return m_buffer.end();
            }

            iterator it = m_buffer.begin();
            std::advance( it, index );

            return it;
        }

        const_iterator binary_vector_binary_search( const Key & _key ) const
        {
            size_t count = m_buffer.size();

            if( count == 0 )
            {
                return m_buffer.end();
            }

            size_t index = helper::binary_vector_binary_search<buffer_type, Key, L>( m_buffer, _key, 0, count - 1 );

            if( index == (size_t)-1 )
            {
                return m_buffer.end();
            }

            const_iterator it = m_buffer.begin();
            std::advance( it, index );

            return it;
        }

    protected:
        iterator binary_vector_binary_lower_bound( const Key & _key )
        {
            size_t count = m_buffer.size();

            if( count == 0 )
            {
                return m_buffer.end();
            }

            size_t index = helper::binary_vector_binary_lower_bound<buffer_type, Key, L>( m_buffer, _key, 0, count );

            iterator it = m_buffer.begin();
            std::advance( it, index );

            return it;
        }

        const_iterator binary_vector_binary_lower_bound( const Key & _key ) const
        {
            size_t count = m_buffer.size();

            if( count == 0 )
            {
                return m_buffer.end();
            }

            size_t index = helper::binary_vector_binary_lower_bound<buffer_type, Key, L>( m_buffer, _key, 0, count );

            const_iterator it = m_buffer.begin();
            std::advance( it, index );

            return it;
        }

    public:
        binary_vector()
        {
        }

    public:
        void reserve( size_t _size )
        {
            m_buffer.reserve( _size );
        }

        size_t size() const
        {
            size_t size = m_buffer.size();

            return size;
        }

        bool empty() const
        {
            bool empty = m_buffer.empty();

            return empty;
        }

        void clear()
        {
            m_buffer.clear();
        }

    public:
        T * get_value( iterator _it ) const
        {
            return _it->value;
        }

        T * get_value( const_iterator _it ) const
        {
            return _it->value;
        }

        void set_value( iterator _it, T * _value )
        {
            _it->value = _value;
        }

        const Key & get_key( const_iterator it ) const
        {
            return it->key;
        }

    public:
        iterator begin()
        {
            iterator it = m_buffer.begin();

            return it;
        }

        iterator end()
        {
            iterator it = m_buffer.end();

            return it;
        }

        const_iterator begin() const
        {
            const_iterator it = m_buffer.begin();

            return it;
        }

        const_iterator end() const
        {
            const_iterator it = m_buffer.end();

            return it;
        }

    public:
        insert_type insert( const Key & _key, T * _value )
        {
            iterator it_lower_bound = this->binary_vector_binary_lower_bound( _key );

            if( it_lower_bound != m_buffer.end() )
            {
                if( L()(_key, it_lower_bound->key) == false )
                {
                    insert_type ret = {it_lower_bound, false};

                    return ret;
                }
            }

            binary_value_store_type bvst = {_key, _value};
            iterator it_insert = m_buffer.insert( it_lower_bound, bvst );

            insert_type ret = {it_insert, true};

            return ret;
        }

        iterator erase( iterator _erase )
        {
            iterator new_it = m_buffer.erase( _erase );

            return new_it;
        }

        bool erase( const Key & _key )
        {
            iterator it_found = this->find( _key );

            if( it_found == this->end() )
            {
                return false;
            }

            this->erase( it_found );

            return true;
        }

    public:
        bool exist( const Key & _key ) const
        {
            const_iterator it_found = this->find( _key );

            bool result = (it_found != this->end());

            return result;
        }

        bool has( const Key & _key, T ** _it ) const
        {
            const_iterator it_found = this->find( _key );

            if( it_found == this->end() )
            {
                return false;
            }

            if( _it != nullptr )
            {
                *_it = it_found->value;
            }

            return true;
        }

        bool has( const Key & _key, const T ** _it ) const
        {
            const_iterator it_found = this->find( _key );

            if( it_found == this->end() )
            {
                return false;
            }

            if( _it != nullptr )
            {
                *_it = it_found->value;
            }

            return true;
        }

    public:
        iterator find( const Key & _key )
        {
            iterator it_lower_bound = this->binary_vector_binary_search( _key );

            iterator it_end = this->end();

            if( it_lower_bound == it_end )
            {
                return it_end;
            }

            return it_lower_bound;
        }

        const_iterator find( const Key & _key ) const
        {
            const_iterator it_lower_bound = this->binary_vector_binary_search( _key );

            const_iterator it_end = this->end();

            if( it_lower_bound == it_end )
            {
                return it_end;
            }

            return it_lower_bound;
        }

    protected:
        buffer_type m_buffer;
    };
}