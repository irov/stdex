#	pragma once

#	include "stdex/intrusive_slug_list.h"

namespace stdex
{
	template<class T>
	class intrusive_slug
		: public intrusive_slug_linked<T>
	{
	public:
		inline intrusive_slug( intrusive_slug_list<T> & _list )
			: intrusive_slug_linked<T>(EILT_SLUG)
			, m_end(*_list.end())
		{
			_list.push_front( this );
		}

	public:
		void next_shuffle()
		{
			linked_type * pos = this->right();

			pos = this->adapt_( pos );

			pos = pos->right();

			pos = this->adapt_( pos );

			this->unlink();
			pos->link_before( this );
		}

		inline bool eof() const
		{
			linked_type * pos = this->current();

			return pos == m_end;
		}

		inline T * operator -> () const
		{
            linked_type * linked = this->current();

			return static_cast<T *>(linked);
		}

		inline T * operator * () const
		{
            linked_type * linked = this->current();

			return static_cast<T *>(linked);
		}

	protected:
		inline linked_type * current() const
		{
			linked_type * pos = this->right();

			linked_type * current = this->adapt_( pos );

			return current;
		}

		inline linked_type * adapt_( linked_type * _pos ) const
		{
			while( _pos->getIntrusiveTag() == EILT_SLUG )
			{
				_pos = _pos->right();
			}

			return _pos;
		}
		
	protected:
		const linked_type * m_end;
	};
}