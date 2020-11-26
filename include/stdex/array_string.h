#pragma once

#include "stdex/memorycopy.h"

#include <cstring>

namespace stdex
{
    template<uint32_t Size>
    class array_string
    {
    public:
        typedef uint32_t size_type;

    public:
        array_string()
            : m_pos( 0 )
        {
        }

        ~array_string()
        {
        }

    public:
        const char * c_str() const
        {
            return m_buffer;
        }

        size_type size() const
        {
            return m_pos;
        }

        bool empty() const
        {
            return m_pos == 0;
        }

    public:
        void clear()
        {
            m_pos = 0;
        }

    public:
        void append( const char * _value, size_type _size )
        {
            if( m_pos + _size >= Size )
            {
                return;
            }

            memorycopy( m_buffer, (size_t)m_pos, _value, (size_t)_size );

            m_pos += _size;
            m_buffer[m_pos] = '\0';
        }

        void append( char * _value )
        {
            size_type size = (size_type)strlen( _value );

            this->append( _value, size );
        }

        void append( const char * _value )
        {
            size_type size = (size_type)strlen( _value );

            this->append( _value, size );
        }

        void append( char _ch )
        {
            if( m_pos + 1 >= Size )
            {
                return;
            }

            m_buffer[m_pos] = _ch;
            m_pos += 1;
            m_buffer[m_pos] = '\0';
        }

        template<class T>
        void append( const T & _value )
        {
            const char * value_str = _value.c_str();
            typename T::size_type value_size = _value.size();

            this->append( value_str, (size_type)value_size );
        }

    public:
        void assign( const char * _value, size_type _size )
        {
            if( _size >= Size )
            {
                return;
            }

            memorycopy( m_buffer, (size_t)0, _value, (size_t)_size );

            m_pos = _size;
            m_buffer[m_pos] = 0;
        }

        void assign( char * _value )
        {
            size_type size = (size_type)strlen( _value );

            this->assign( _value, size );
        }

        void assign( const char * _value )
        {
            size_type size = (size_type)strlen( _value );

            this->assign( _value, size );
        }

        void assign( char _ch )
        {
            if( 1 >= Size )
            {
                return;
            }

            m_buffer[0] = _ch;
            m_pos += 1;
            m_buffer[m_pos] = '\0';
        }

        template<class T>
        void assign( const T & _value )
        {
            const char * value_str = _value.c_str();
            typename T::size_type value_size = _value.size();

            this->assign( value_str, (size_type)value_size );
        }

    public:
        void replace_last( const char * _value )
        {
            size_t size = strlen( _value );

            if( size > (size_t)m_pos )
            {
                return;
            }

            memorycopy( m_buffer, (size_t)m_pos - size, _value, size );
        }

        void cut_before_last_of( char _ch )
        {
            char * ch_pos = strrchr( m_buffer, _ch );

            if( ch_pos == nullptr )
            {
                return;
            }

            ptrdiff_t d = ch_pos - m_buffer;

            m_pos = (size_type)d;
        }

    public:
        void operator = ( const char * _value )
        {
            this->clear();
            this->append( _value );
        }

        void operator += ( char * _value )
        {
            this->append( _value );
        }

        void operator += ( const char * _value )
        {
            this->append( _value );
        }

        void operator += ( char _value )
        {
            this->append( _value );
        }

        template<class T>
        void operator = ( const T & _value )
        {
            this->clear();
            this->append( _value );
        }

        template<class T>
        void operator += ( const T & _value )
        {
            this->append( _value );
        }

    protected:
        char m_buffer[Size];
        size_type m_pos;
    };
}