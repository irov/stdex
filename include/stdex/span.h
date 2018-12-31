#pragma once

#include <stdint.h>

namespace stdex
{
    namespace span
    {
        template<class It>
        class range_iterator_t
        {
        public:
            range_iterator_t( It _begin, It _end )
                : m_begin( _begin )
                , m_end( _end )
            {}

        public:
            inline It begin() const
            {
                return m_begin;
            }

            inline It end() const
            {
                return m_end;
            }

        protected:
            It m_begin;
            It m_end;
        };
        //////////////////////////////////////////////////////////////////////////
        template<class C>
        range_iterator_t<typename C::const_iterator> range( const C & _container, int32_t _from )
        {
            typename C::size_type size = _container.size();

            uint32_t diff_from = (_from + size) % size;

            typename C::const_iterator it_begin = _container.begin();

            return range_iterator_t<typename C::const_iterator>( it_begin + diff_from, _container.end() );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C>
        range_iterator_t<typename C::const_iterator> range( const C & _container, int32_t _from, int32_t _to )
        {
            typename C::size_type size = _container.size();

            typename C::size_type diff_from = (_from + size) % size;
            typename C::size_type diff_to = (_to + size) % size;

            typename C::const_iterator it_begin = _container.begin();

            if( diff_to <= diff_from )
            {
                return range_iterator_t<typename C::const_iterator>( it_begin, it_begin );
            }

            return range_iterator_t<typename C::const_iterator>( it_begin + diff_from, it_begin + diff_to );
        }
    }
}