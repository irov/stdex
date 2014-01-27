#	pragma once

namespace stdex
{
	template<class T>
	class intrusive_slug
		: public T::linked_type
	{
    public:
		typedef typename T::value_type value_type;
		typedef typename T::linked_type linked_type;

	public:
		inline intrusive_slug( T & _list )
			: T::linked_type(EILT_SLUG)
			, m_end(nullptr)
		{
			m_eof = _list.empty();

			if( m_eof == false )
			{
				typename T::iterator it = _list.pure_begin_();
				linked_type * linked = it.get();
				linked->link_before( this );

				m_end = *_list.end();
			}
		}

	public:
		void next_shuffle()
		{
			linked_type * pos = this->right();

			linked_type * pos_adapt = this->adapt_( pos );

			if( pos_adapt == m_end )
			{
				m_eof = true;
				return;
			}

			linked_type * pos_adapt_right = pos_adapt->right();

			linked_type * pos_adapt_right_adapt = this->adapt_( pos_adapt_right );

			if( pos_adapt_right_adapt == m_end )
			{
				m_eof = true;
				return;
			}

			this->unlink();
			pos_adapt_right_adapt->link_before( this );
		}

		inline bool eof() const
		{
			return m_eof;
		}

		inline value_type operator -> () const
		{
            linked_type * linked = this->current();

			return static_cast<value_type>(linked);
		}

		inline value_type operator * () const
		{
            linked_type * linked = this->current();

			return static_cast<value_type>(linked);
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
			linked_type * pos = _pos;

			while( pos->getIntrusiveTag() == EILT_SLUG )
			{
				pos = pos->right();
			}

			return pos;
		}
		
	protected:
		bool m_eof;
		const linked_type * m_end;
		
	};
}