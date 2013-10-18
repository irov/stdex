#	pragma once

#	include "stdex/intrusive_slug_list.h"

namespace stdex
{
	template<class T>
	class intrusive_slug
		: public intrusive_slug_linked<T>
	{
    public:
        typedef typename intrusive_slug_linked<T>::linked_type slug_linked_type;
	public:
		inline intrusive_slug( intrusive_slug_list<T> & _list )
			: intrusive_slug_linked<T>(EILT_SLUG)
			, m_end(*_list.end())
		{
			_list.push_front( this );

			m_eof = _list.empty();
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

		inline T * operator -> () const
		{
            slug_linked_type * linked = this->current();

			return static_cast<T *>(linked);
		}

		inline T * operator * () const
		{
            slug_linked_type * linked = this->current();

			return static_cast<T *>(linked);
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