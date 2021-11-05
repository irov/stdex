#pragma once

#include <memory.h>

namespace stdex
{
    template<class C>
    class container_reader
    {
    public:
        typedef typename C::value_type value_type;

    public:
        container_reader( const C & _container )
            : m_container( _container )
            , m_carriage( 0 )
        {
        }

        ~container_reader()
        {
        }

    protected:
        container_reader & operator = ( const container_reader & )
        {
            return *this;
        }

    public:
        template<class T>
        inline void read( T & _t )
        {
            *this >> _t;
        }

        template<class T>
        inline void readPOD( T & _t )
        {
            value_type * buff = reinterpret_cast<value_type *>(&_t);
            this->readBuffer( buff, sizeof( T ) );
        }

        template<class T>
        inline void readPODs( T * _t, size_t _count )
        {
            value_type * buff = reinterpret_cast<value_type *>(_t);
            this->readBuffer( buff, _count * sizeof( T ) );
        }

        inline void readBuffer( void * _begin, size_t _size )
        {
            const value_type * data = m_container.data();

            ::memcpy( _begin, data + m_carriage, _size );

            m_carriage += _size;
        }

    protected:
        const C & m_container;
        uint32_t m_carriage;
    };

    template<class C>
    container_reader<C> make_container_reader( const C & _container )
    {
        return container_reader<C>( _container );
    }

    template<class C, class T>
    void operator << ( container_reader<C> & aw, T & _value )
    {
        aw.readPOD( _value );
    }
}