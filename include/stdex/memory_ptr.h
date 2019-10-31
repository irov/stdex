#pragma once

namespace stdex
{
    template<class T>
    class memory_ptr_allocator_nothread
    {
    public:
        static T * malloc( size_t _count )
        {
            void * mem = stdex_malloc( sizeof( T ) * _count );

            return static_cast<T *>(mem);
        }

        static T * realloc( void * _mem, size_t _count )
        {
            void * mem = stdex_realloc( _mem, sizeof( T ) * _count );

            return static_cast<T *>(mem);
        }

        static void free( void * _mem )
        {
            stdex_free( _mem );
        }
    };

    template<class T>
    class memory_ptr_allocator_threadsafe
    {
    public:
        static T * malloc( size_t _count )
        {
            void * mem = stdex_malloc_threadsafe( sizeof( T ) * _count );

            return static_cast<T *>(mem);
        }

        static void free( void * _mem )
        {
            stdex_free_threadsafe( _mem );
        }
    };

    template<class T, class Allocator>
    class memory_ptr
    {
    public:
        memory_ptr()
            : m_ptr( nullptr )
        {
        }

        ~memory_ptr()
        {
            Allocator::free( m_ptr );
            m_ptr = nullptr;
        }

        explicit memory_ptr( size_t _count )
            : m_ptr( Allocator::malloc( _count ) )
        {
        }

    private:
        memory_ptr( const memory_ptr & _ptr )
        {
            (void)_ptr;
        }

    public:
        void resize( size_t _count )
        {
            m_ptr = Allocator::malloc( _count );
        }

    public:
        T * buff() const
        {
            return m_ptr;
        }

    public:
        T & operator [] ( uint32_t _index )
        {
            return m_ptr[_index];
        }

        const T & operator [] ( uint32_t _index ) const
        {
            return m_ptr[_index];
        }

    protected:
        T * m_ptr;
    };
}