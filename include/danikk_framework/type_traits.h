#pragma once

namespace danikk_framework
{
	template <class T>
	struct remove_const
	{
		typedef T type;
	};

	template <class T>
	struct remove_const<const T>
	{
		typedef T type;
	};
}






#define create_ns_checker(ns) \
namespace ns { \
    template <typename T> \
    constexpr std::true_type ns##FindmeNsADLHelper(T&&); \
} \
namespace ns##_type_traits { \
    class ns##SecondBestMatchType {}; \
    class ns##BestExactMatchType : public ns##SecondBestMatchType {}; \
    namespace helpers { \
        template <typename T> \
        auto TestNs(ns##_type_traits::ns##BestExactMatchType) \
               -> decltype(ns##FindmeNsADLHelper(std::declval<T>())); \
        template <typename T> \
        auto TestNs(ns##_type_traits::ns##SecondBestMatchType) \
               -> std::false_type; \
    } \
    template <typename T> \
    constexpr bool ns##IsFindmeNs() { \
        return decltype(helpers::TestNs<std::decay_t<T>> \
                           (ns##BestExactMatchType{}))::value; \
    } \
}

#define is_in_ns(Type, ns) \
(ns##_type_traits::ns##IsFindmeNs<Type>())

create_ns_checker(std)
