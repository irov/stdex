#	pragma once

namespace stdex
{
    template<class T>
    class holder_ptr
    {
    public:
        inline static T * get()
        {
            return m_hostage;
        }

        inline static void keep( T * _hostage )
        {
            m_hostage = _hostage;
        }

        inline static T * release()
        {
            T * freed = m_hostage;
            m_hostage = nullptr;

            return freed;
        }

        template<class K>
        inline static K release_t()
        {
            T * t = holder_ptr<T>::release();

            K k = static_cast<K>(t);

            return k;
        }

    protected:
        static T * m_hostage;
    };

    template<class T>
    T * holder_ptr<T>::m_hostage = nullptr;
}