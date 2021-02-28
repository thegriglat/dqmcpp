#ifndef IFILTER_HH

/**
 * @brief Base abstract template class for all filter classes
 *
 * @tparam T
 */
template <typename T>
class IFilter
{
public:
    using type = T;
    virtual bool operator()(T i) = 0;
};

/**
 * @brief Template class for combining IFilters with logical AND
 *
 * @tparam A
 * @tparam B
 */
template <class A, class B>
class FilterAnd : public IFilter<typename A::type>
{
public:
    bool operator()(typename A::type i)
    {
        return A()(i) && B()(i);
    }
};

/**
 * @brief Template class for combining IFilters with logical OR
 *
 * @tparam A
 * @tparam B
 */
template <class A, class B>
class FilterOr : public IFilter<typename A::type>
{
public:
    bool operator()(typename A::type i)
    {
        return A()(i) || B()(i);
    }
};

/**
 * @brief Template class for logical NOT
 *
 * @tparam A
 */
template <typename A>
class FilterNot : public IFilter<typename A::type>
{
public:
    bool operator()(typename A::type i)
    {
        return !A()(i);
    }
};

#define IFILTER_HH
#endif