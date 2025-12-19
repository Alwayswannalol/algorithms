#include <utility>
constexpr int INSERTION_SORT_QUANT = 16;

template <typename T>
inline void swap(T &a, T &b)
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

template <typename T, typename Compare>
void insertion_sort(T *first, T *last, Compare comp)
{
    if (first == last)
        return;

    for (T *i = first + 1; i != last; ++i)
    {
        T temp = std::move(*i);
        T *j = i;

        while (j > first && comp(temp, *(j - 1)))
        {
            *j = std::move(*(j - 1));
            --j;
        }

        *j = std::move(temp);
    }
}

template <typename T, typename Compare>
T *median_of_three(T *a, T *b, T *c, Compare comp)
{
    if (comp(*a, *b))
    {
        if (comp(*b, *c))
            return b;
        if (comp(*a, *c))
            return c;
        return a;
    }
    else
    {
        if (comp(*a, *c))
            return a;
        if (comp(*b, *c))
            return c;
        return b;
    }
}

template <typename T, typename Compare>
T *partition(T *first, T *last, T *pivot, Compare comp)
{
    swap(*pivot, *(last - 1));
    T *store = first;

    for (T *p = first; p != last - 1; ++p)
    {
        if (comp(*p, *(last - 1)))
        {
            swap(*p, *store);
            ++store;
        }
    }

    swap(*store, *(last - 1));
    return store;
}

template <typename T, typename Compare>
void quicksort(T *first, T *last, Compare comp)
{
    while (last - first > 1)
    {
        if (last - first <= INSERTION_SORT_QUANT)
        {
            insertion_sort(first, last, comp);
            return;
        }

        T *middle = first + (last - first) / 2;
        T *pivot = median_of_three(first, middle, last - 1, comp);
        T *p = partition(first, last, pivot, comp);

        if (p - first < last - p - 1)
        {
            quicksort(first, p, comp);
            first = p + 1;
        }
        else
        {
            quicksort(p + 1, last, comp);
            last = p;
        }
    }
}

template <typename T, typename Compare>
void sort(T *first, T *last, Compare comp)
{
    if (first == last || first + 1 == last)
    {
        return;
    }
    quicksort(first, last, comp);
}
