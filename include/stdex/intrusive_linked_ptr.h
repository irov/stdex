#	pragma once

#   include "stdex/intrusive_ptr_base.h"
#   include "stdex/intrusive_ptr.h"

namespace stdex
{
    template<class Tag>
	class intrusive_linked_ptr
        : public intrusive_ptr_base
	{
    public:
		typedef intrusive_linked_ptr<Tag> linked_type;
        typedef intrusive_ptr<linked_type> linked_type_ptr;

	public:
		inline intrusive_linked_ptr()
			: m_right(nullptr)
			, m_left(nullptr)
		{
		}

		inline ~intrusive_linked_ptr()
		{
			this->unlink();
		}

    private:
        intrusive_linked_ptr & operator = ( const intrusive_linked_ptr & _linked )
        {
            (void)_linked;

            return *this;
        }

	public:
		inline bool unique() const
		{
			return (m_right == nullptr) && (m_left == nullptr);
		}

		inline linked_type * left() const
		{
			return m_left;
		}

		inline linked_type * right() const
		{
			return m_right;
		}
        
		inline void link_after( linked_type * _other )
		{
			_other->m_right = m_right;
			_other->m_left = (this);

			if( m_right != nullptr )
			{
				m_right->m_left = _other;
			}

			m_right = _other;
		}

		inline void link_before( linked_type * _other )
		{
			_other->m_left = m_left;
			_other->m_right = (this);

			if( m_left != nullptr )
			{
				m_left->m_right = _other;
			}

			m_left = _other;
		}

		inline void unlink()
		{
			if( m_right != nullptr )
			{
				m_right->m_left = m_left;
			}

			if( m_left != nullptr )
			{
				m_left->m_right = m_right;
			}
			
			m_left = nullptr;
			m_right = nullptr;
		}

		inline linked_type * leftcast() const
		{
			linked_type * it = m_left;

			while( it->m_left != nullptr )
			{
				it = it->m_left;
			}

			return it;
		}

		inline linked_type * rightcast() const
		{
			linked_type * it = m_right;

			while( it->m_right != nullptr )
			{
				it = it->m_right;
			}

			return it;
		}

		void linkall( linked_type * _other )
		{
			linked_type * other_right = _other->m_right;

			if( m_left != nullptr )
			{
				linked_type * left = this->leftcast();

				left->m_left = _other;
				_other->m_right = left;
			}
			else
			{
				m_left = _other;
				_other->m_right = (this);
			}

			if( m_right != nullptr )
			{
				if( other_right != nullptr )
				{
					linked_type * right = this->rightcast();

					other_right->m_left = right;
					right->m_right = other_right;
				}
			}
			else
			{
				if( other_right != nullptr )
				{
					other_right->m_left = (this);
					m_right = other_right;
				}
			}
		}

		template<class F>
		void foreach( F _f ) const
		{
			foreach_self<F>( _f );
			foreach_other<F>( _f );
		}

		template<class F>
		void foreach_self( F _f ) const
		{
			const linked_type * nc_this = (this);
			_f( const_cast<linked_type *>(nc_this) );
		}

		template<class F>
		void foreach_other( F _pred ) const
		{
			linked_type * it_right = m_right;
			while( it_right != nullptr )
			{
				_pred( it_right );
				it_right = it_right->m_right;
			}

			linked_type * it_left = m_left;
			while( it_left != nullptr )
			{
				_pred( (it_left) );
				it_left = it_left->m_left;
			}
		}

		template<class F>
		const linked_type * find( F _pred ) const
		{
			linked_type * node_found = this->find_self( _pred );

			if( node_found != nullptr )
			{
				return node_found;
			}

            const linked_type * other = find_other( _pred );

			return other;
		}

		template<class F>
		const linked_type * find_self( F _pred ) const
		{
			if( _pred(this) == true )
			{
				return this;
			}

			return nullptr;
		}

		template<class F>
		const linked_type * find_other( F _pred ) const
		{
			linked_type * it_right = m_right;
			while( it_right != nullptr )
			{
				if( _pred( it_right ) == true )
				{
					return this;
				}

				it_right = it_right->m_right;
			}

			linked_type * it_left = m_left;
			while( it_left )
			{
				if( _pred( it_left ) == true )
				{
					return this;
				}

				it_left = it_left->m_left;
			}

			return nullptr;
		}

    public:
		mutable linked_type_ptr m_right;
		mutable linked_type_ptr m_left;
    };
}