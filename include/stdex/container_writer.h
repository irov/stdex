#pragma once

namespace stdex
{
    template<class C>
    class container_writer
    {
    public:
        typedef typename C::value_type value_type;

    public:
        container_writer( C & _container )
            : m_container( _container )
        {
        }

        ~container_writer()
        {
        }

    protected:
        container_writer & operator = ( const container_writer & )
        {
            return *this;
        }

    public:
        template<class T>
        inline void write( const T & _t )
        {
            *this >> _t;
        }

        template<class T>
        inline void writePOD( const T & _t )
        {
            const value_type * buff = reinterpret_cast<const value_type *>(&_t);
            this->writeBuffer( buff, sizeof( T ) );
        }

        template<class T>
        inline void writePODs( const T * _t, size_t _count )
        {
            const value_type * buff = reinterpret_cast<const value_type *>(_t);
            this->writeBuffer( buff, _count * sizeof( T ) );
        }

        inline void writeBuffer( const value_type * _begin, size_t _size )
        {
            m_container.insert( m_container.end(), _begin, _begin + _size );
        }

    protected:
        C & m_container;
    };

    template<class C>
    container_writer<C> make_container_writer( C & _container )
    {
        return container_writer<C>( _container );
    }

    template<class C, class T>
    void operator << ( container_writer<C> & aw, const T & _value )
    {
        aw.writePOD( _value );
    }
}