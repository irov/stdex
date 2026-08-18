#pragma once

namespace stdex
{
    enum EIntrusiveSlugLinkedTag
    {
        EILT_ELEMENT,
        EILT_END,
        EILT_SLUG
    };

    namespace detail
    {
        struct intrusive_slug_no_adapt_t
        {
        };
    }
}
