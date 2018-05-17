#pragma once

#include <stddef.h>

namespace stdex
{
    namespace helper
    {
        template<class I, class F>
        I intrusive_find_if( I _begin, I _end, F _pred )
        {
            for( I
                it = _begin,
                it_end = _end;
                it != it_end;
                ++it )
            {
                if( _pred( *it ) == true )
                {
                    return it;
                }
            }

            return _end;
        }

        template<class I, class T>
        I intrusive_find( I _begin, I _end, T _node )
        {
            I it_find( _node );

            for( I
                it = _begin,
                it_end = _end;
                it != it_end;
                ++it )
            {
                if( it == it_find )
                {
                    return it;
                }
            }

            return _end;
        }

        template<class I, class T>
        bool intrusive_has( I _begin, I _end, T _node )
        {
            I it_find( _node );

            for( I
                it = _begin,
                it_end = _end;
                it != it_end;
                ++it )
            {
                if( it == it_find )
                {
                    return true;
                }
            }

            return false;
        }

        template<class I>
        I intrusive_advance( I _it, I _end, size_t _count )
        {
            for( size_t i = 0; i != _count; ++i )
            {
                if( _it != _end )
                {
                    ++_it;
                }
                else
                {
                    return _end;
                }
            }

            return _it;
        }

        template<class I>
        size_t intrusive_distance( I _begin, I _end )
        {
            size_t count = 0;
            for( ; _begin != _end; ++_begin )
            {
                ++count;
            }

            return count;
        }

        template<class T, class P>
        void intrusive_sort( T & _container, P _predicate )
        {
            typedef typename T::linked_type linked_type;

            typename T::iterator it_begin = _container.begin();
            typename T::iterator it_end = _container.end();

            for( typename T::iterator it = it_begin; it != it_end; )
            {
                typename T::iterator it_next = it;
                ++it_next;

                while( it != _container.begin() )
                {
                    typename T::iterator it_prev = it;
                    --it_prev;

                    if( _predicate( *it_prev, *it ) == false )
                    {
                        linked_type * it_linked = *it;
                        linked_type * it_prev_linked = *it_prev;

                        it_linked->unlink();
                        it_prev_linked->link_before( it_linked );
                    }
                    else
                    {
                        break;
                    }
                }

                it = it_next;
            }
        }

        template<class C, class P>
        void intrusive_sort_stable( C & _container, P _predicate )
        {
            typedef typename C::iterator container_iterator_type;
            typedef typename C::value_type_ptr container_value_type_ptr;
            typedef typename C::linked_type linked_type;
            typedef linked_type * linked_type_ptr;

            container_iterator_type it_begin = _container.begin();
            container_iterator_type it_end = _container.end();

            for( container_iterator_type it = it_begin; it != it_end; )
            {
                container_iterator_type it_next = it;
                ++it_next;

                while( it != _container.begin() )
                {
                    container_iterator_type it_prev = it;
                    --it_prev;

                    container_value_type_ptr element_ptr = *it;
                    container_value_type_ptr prev_element_ptr = *it_prev;

                    if( _predicate( prev_element_ptr, element_ptr ) == false && _predicate( element_ptr, prev_element_ptr ) == true )
                    {
                        linked_type_ptr element_linked = element_ptr.template getT<linked_type_ptr>();
                        linked_type_ptr prev_element_linked = element_ptr.template getT<linked_type_ptr>();

                        element_linked->unlink();
                        prev_element_linked->link_before( element_linked );
                    }
                    else
                    {
                        break;
                    }
                }

                it = it_next;
            }
        }
    }
}