#pragma once

#include "stdex/intrusive_slug_linked_tag.h"

#include <cstdint>

namespace stdex
{
    namespace helper
    {
        template<class I>
        bool intrusive_slug_empty( I _begin, I _end )
        {
            return _begin == _end;
        }

        template<class L>
        bool intrusive_slug_empty( const L * _begin, const L * _end )
        {
            const L * it = _begin;

            while( it != _end )
            {
                if( it->getIntrusiveTag() != EILT_SLUG )
                {
                    return false;
                }

                it = it->right_get();
            }

            return true;
        }

        template<class I>
        uint32_t intrusive_slug_distance( I _begin, I _end )
        {
            uint32_t count = 0;

            for( ; _begin != _end; ++_begin )
            {
                ++count;
            }

            return count;
        }

        template<class L>
        uint32_t intrusive_slug_distance( const L * _begin, const L * _end )
        {
            uint32_t count = 0;

            for( const L * it = _begin; it != _end; it = it->right_get() )
            {
                if( it->getIntrusiveTag() == EILT_ELEMENT )
                {
                    ++count;
                }
            }

            return count;
        }
    }
}
