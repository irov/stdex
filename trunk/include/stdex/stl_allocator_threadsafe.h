#	pragma once

#	include <stdex/allocator.h>

#	include <new>
#	include <limits>
#	include <memory>

namespace stdex
{
	template<typename T>
	class stl_allocator_threadsafe
	{
	public: 
		typedef T value_type;
		typedef T * pointer;
		typedef const T * const_pointer;
		typedef T & reference;
		typedef const T & const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

	public: 
		template<typename U>
		struct rebind 
		{
			typedef stl_allocator_threadsafe<U> other;
		};

	public: 
		inline stl_allocator_threadsafe()
		{
		}

		inline stl_allocator_threadsafe( const stl_allocator_threadsafe & )
		{
		}

		template<typename U>
		inline stl_allocator_threadsafe( const stl_allocator_threadsafe<U> & )
		{
		}

		inline ~stl_allocator_threadsafe()
		{
		}
						
	public:
		inline pointer address( reference _ref ) 
		{ 
			return &_ref; 
		}

		inline const_pointer address( const_reference _ref ) 
		{ 
			return &_ref; 
		}

		inline pointer allocate( size_type _count, typename std::allocator<void>::const_pointer p = nullptr )
		{ 
			(void)p;

			void * memory = stdex_malloc_threadsafe( _count * sizeof(T) );

			return reinterpret_cast<pointer>(memory);
		}

		inline void deallocate( pointer _ptr, size_type _count ) 
		{ 
			(void) _count;

			stdex_free_threadsafe( _ptr );
		}

		inline size_type max_size() const
		{ 
			return (std::numeric_limits<size_type>::max)() / sizeof(T);
		}

		inline void construct( pointer _ptr, const_reference _t )
		{ 
			new (_ptr) T(_t);
		}

		inline void destroy( pointer _ptr )
		{ 
			(void)_ptr;

			_ptr->~T();
		}

		inline bool operator == (stl_allocator_threadsafe const &) const
		{ 
			return true; 
		}

		inline bool operator != (stl_allocator_threadsafe const &) const
		{ 
			return false;
		}
	};
}