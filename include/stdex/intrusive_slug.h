#	pragma once

namespace stdex
{
	template<class T>
	class intrusive_slug
		: public T::linked_type
	{
    public:
		typedef typename T::value_type value_type;
		typedef typename T::linked_type slug_linked_type;

	public:
		inline intrusive_slug( T & _list )
			: T::linked_type(EILT_SLUG)			
		{
			m_eof = _list.empty();

			if( m_eof == false )
			{
				typename T::iterator it = _list.pure_begin_();
				slug_linked_type * linked = it.get();
				linked->link_before( this );

				m_end = *_list.end();
			}
		}

	public:
		void next_shuffle()
		{
			slug_linked_type * pos = this->right();

			pos = this->adapt_( pos );

			pos = pos->right();

			pos = this->adapt_( pos );

			this->unlink();
			pos->link_before( this );

			m_eof = (pos == m_end);
		}

		inline bool eof() const
		{
			return m_eof;
		}

		inline value_type operator -> () const
		{
            slug_linked_type * linked = this->current();

			return static_cast<value_type>(linked);
		}

		inline value_type operator * () const
		{
            slug_linked_type * linked = this->current();

			return static_cast<value_type>(linked);
		}

	protected:
		inline slug_linked_type * current() const
		{
			slug_linked_type * pos = this->right();

			slug_linked_type * current = this->adapt_( pos );

			return current;
		}

		inline slug_linked_type * adapt_( slug_linked_type * _pos ) const
		{
			slug_linked_type * pos = _pos;

			while( pos->getIntrusiveTag() == EILT_SLUG )
			{
				pos = pos->right();
			}

			return pos;
		}
		
	protected:
		const slug_linked_type * m_end;
		bool m_eof;
	};
}