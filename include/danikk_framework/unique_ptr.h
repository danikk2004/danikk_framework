#pragma once

#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	template <class dataT> class UniquePtr
    {
    private:
        dataT* m_ptr;

        UniquePtr(UniquePtr&) = delete;
        UniquePtr(UniquePtr&&) = delete;
    public:
        UniquePtr()
        {
            m_ptr = NULL;
        }

        UniquePtr(dataT* ptr)
        {
            m_ptr = ptr;
        }

        UniquePtr& operator = (dataT* newPtr)
        {
            if(m_ptr != NULL)
            {
                free(m_ptr);
            }
            m_ptr = newPtr;
            return *this;
        }

        ~UniquePtr()
        {
            if(m_ptr != NULL)
            {
                free(m_ptr);
            }
        }

        operator dataT*()
        {
            return m_ptr;
        }
    };
}
