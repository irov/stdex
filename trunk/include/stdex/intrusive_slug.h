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
		intrusive_slug( T & _list )
			: T::linked_type(EILT_SLUG)
			, m_list(_list)
		{
			m_list.increfSlug();

			m_eof = m_list.empty();

			if( m_eof == false )
			{
				linked_type * linked = m_list.head();
				linked->link_after( this );
			}
		}

		~intrusive_slug()
		{
			m_list.decrefSlug();
		}

	public:
		void next_shuffle()
		{
			linked_type * pos_right = this->right();

			linked_type * head = m_list.head();

			linked_type * pos_right_adapt_right_adapt = nullptr;

			size_t countSlugs = m_list.countSlugs();

			if( countSlugs == 1 )
			{
				if( pos_right == head )
				{
					m_eof = true;
					return;
				}

				pos_right_adapt_right_adapt = pos_right->right();
			}
			else
			{
				linked_type * pos_right_adapt = this->adapt_( pos_right );

				if( pos_right_adapt == head )
				{
					m_eof = true;
					return;
				}

				linked_type * pos_right_adapt_right = pos_right_adapt->right();

				pos_right_adapt_right_adapt = this->adapt_( pos_right_adapt_right );
			}

			if( pos_right_adapt_right_adapt == head )
			{
				m_eof = true;
				return;
			}

			this->unlink();
			pos_right_adapt_right_adapt->link_before( this );
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

			size_t countSlugs = m_list.countSlugs();
			if( countSlugs != 1 )
			{				
				pos = this->adapt_( pos );
			}

			return pos;
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
		T & m_list;
		bool m_eof;
	};
}