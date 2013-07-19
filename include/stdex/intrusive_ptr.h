#	pragma once

namespace stdex
{
	template<class T> 
	class intrusive_ptr
	{
	public:
		typedef T element_type;

	public:
		intrusive_ptr()
			: m_ptr(nullptr)
		{
		}

		intrusive_ptr( T * _ptr )
			: m_ptr(_ptr)
		{
			this->incref();
		}

        template<class U>
        intrusive_ptr( U * _ptr )
            : m_ptr(static_cast<T *>(_ptr))
        {
            this->incref();
        }
        
		intrusive_ptr(const intrusive_ptr & _rhs)
			: m_ptr(_rhs.get())
		{
			this->incref();
		}

        template<class U>
        intrusive_ptr(const intrusive_ptr<U> & _rhs)
            : m_ptr(static_cast<T *>(_rhs.get()))
        {
            this->incref();
        }

		~intrusive_ptr()
		{
			this->decref();
		}

		intrusive_ptr & operator = ( const intrusive_ptr & _rhs )
		{
			intrusive_ptr swap_ptr(_rhs);
            swap_ptr.swap( *this );

			return *this;
		}

		intrusive_ptr & operator = ( T * _rhs )
		{
            intrusive_ptr swap_ptr(_rhs);
            swap_ptr.swap( *this );

			return *this;
		}

	public:
		T * get() const
		{
			return m_ptr;
		}

		T * operator -> () const
		{
			return m_ptr;
		}

        //operator T * () const
        //{
        //    return m_ptr;
        //}

		void swap( intrusive_ptr & _rhs )
		{
            T * tmp = m_ptr;
            m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
		}

	protected:
		void incref()
		{
			if( m_ptr != nullptr ) 
			{
				intrusive_ptr_add_ref( m_ptr );
			}
		}

		void decref()
		{
			if( m_ptr != nullptr )
			{
				intrusive_ptr_dec_ref( m_ptr );
			}
		}

    protected:
		T * m_ptr;
	};
    //////////////////////////////////////////////////////////////////////////
    template<class T> 
    inline void swap( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
    {
        _left.swap( _right );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    inline U intrusive_static_cast( const intrusive_ptr<T> & _iptr )
    {
        typedef typename U::element_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = static_cast<U_type *>(t_ptr);

        return U(u_ptr);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    inline U intrusive_dynamic_cast( const intrusive_ptr<T> & _iptr )
    {
        typedef typename U::element_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = dynamic_cast<U_type *>(t_ptr);

        return U(u_ptr);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    inline U * intrusive_get( const intrusive_ptr<T> & _iptr )
    {
        T * t_ptr = _iptr.get();
        U * u_ptr = static_cast<U *>(t_ptr);

        return u_ptr;
    }
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator == ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
	{
		return _left.get() == _right.get();
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator != ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
	{
		return _left.get() != _right.get();
	}
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator == ( const intrusive_ptr<T> & _left, void * )
    {
        T * ptr = _left.get();

        return ptr == nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T> 
    inline bool operator != ( const intrusive_ptr<T> & _left, void * )
    {
        T * ptr = _left.get();

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T> 
    inline bool operator == ( void * , const intrusive_ptr<T> & _right )
    {
        T * ptr = intrusive_get<T *>(_right);

        return ptr == nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T> 
    inline bool operator != ( void * , const intrusive_ptr<T> & _right )
    {
        T * ptr = intrusive_get<T *>(_right);

        return ptr != nullptr;
    }
}