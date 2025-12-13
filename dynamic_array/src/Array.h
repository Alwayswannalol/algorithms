#include <cstdlib>
#include <cassert>
#include <stdexcept>
#include <utility>

template <typename T>
class Array final
{
    T *data_;
    int capacity_;
    int size_;

    static constexpr int start_capacity = 16;
    static constexpr int growth_factor = 2;

    void reallocate(int new_capacity)
    {
        T *new_data = static_cast<T *>(malloc(new_capacity * sizeof(T)));
        if (!new_data)
        {
            throw std::bad_alloc();
        }

        for (int i = 0; i < size_; i++)
        {
            try
            {
                new (new_data + i) T(std::move_if_noexcept(data_[i]));
            }
            catch (...)
            {
                for (int j = 0; j < i; ++j)
                {
                    new_data[j].~T();
                }
                free(new_data);
                throw;
            }
            data_[i].~T();
        }
        free(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void ensure_capacity(int required_capacity)
    {
        if (required_capacity > capacity_)
        {
            int new_capacity = std::max(
                static_cast<int>(capacity_ * growth_factor),
                required_capacity);
            reallocate(new_capacity);
        }
    }

    void swap(Array &other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

public:
    Array() : size_(0), capacity_(start_capacity)
    {
        data_ = static_cast<T *>(malloc(capacity_ * sizeof(T)));
        if (!data_)
        {
            throw std::bad_alloc();
        }
    }

    explicit Array(int capacity) : size_(0), capacity_(capacity)
    {
        if (capacity_ <= 0)
        {
            capacity_ = start_capacity;
        }
        data_ = static_cast<T *>(malloc(capacity_ * sizeof(T)));
        if (!data_)
        {
            throw std::bad_alloc();
        }
    }

    Array(const Array &other)
        : size_(other.size_),
          capacity_(other.capacity_)
    {
        data_ = static_cast<T *>(malloc(capacity_ * sizeof(T)));
        if (!data_)
        {
            throw std::bad_alloc();
        }

        for (int i = 0; i < size_; ++i)
        {
            try
            {
                new (data_ + i) T(other.data_[i]);
            }
            catch (...)
            {
                for (int j = 0; j < i; ++j)
                {
                    data_[j].~T();
                }
                free(data_);
                throw;
            }
        }
    }

    Array(Array &&other) noexcept
        : data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~Array() noexcept
    {
        if (data_)
        {
            for (int i = 0; i < size_; i++)
            {
                data_[i].~T();
            }
            free(data_);
        }
    }

    Array &operator=(const Array &other)
    {
        if (this != &other)
        {
            Array temp(other);
            swap(temp);
        }
        return *this;
    }

    Array &operator=(Array &&other) noexcept
    {
        if (this != &other)
        {
            for (int i = 0; i < size_; ++i)
            {
                data_[i].~T();
            }
            size_ = 0;
            free(data_);

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    int insert(const T &value)
    {
        ensure_capacity(size_ + 1);
        new (data_ + size_) T(value);
        return size_++;
    }

    int insert(int index, const T &value)
    {
        assert(index >= 0 && index <= size_);

        ensure_capacity(size_ + 1);

        if (size_ > 0)
        {
            for (int i = size_; i > index; --i)
            {
                new (data_ + i) T(std::move_if_noexcept(data_[i - 1]));
                data_[i - 1].~T();
            }
        }

        new (data_ + index) T(value);
        ++size_;
        return index;
    }

    void remove(int index)
    {
        assert(index >= 0 && index < size_);

        for (int i = index; i < size_ - 1; ++i)
        {
            data_[i].~T();
            new (data_ + i) T(std::move_if_noexcept(data_[i + 1]));
            data_[i + 1].~T();
        }
        --size_;
    }

    const T &operator[](int index) const noexcept
    {
        assert(index >= 0 && index < size_);
        return data_[index];
    }

    T &operator[](int index) noexcept
    {
        assert(index >= 0 && index < size_);
        return data_[index];
    }

    int size() const noexcept
    {
        return size_;
    }

public:
    class Iterator
    {
    private:
        T *ptr_;
        T *start_;
        T *end_;
        bool reverse_;

    public:
        Iterator(T *start, int size, T *current, bool reverse)
            : ptr_(current),
              start_(start),
              end_(start + size),
              reverse_(reverse)
        {
            if (reverse && size > 0)
            {
                ptr_ = start + size - 1;
            }
        }

        T &get() { return *ptr_; }
        const T &get() const { return *ptr_; }

        void set(const T &value) { *ptr_ = value; }

        void next()
        {
            if (reverse_)
            {
                --ptr_;
            }
            else
            {
                ++ptr_;
            }
        }

        bool hasNext() const
        {
            if (reverse_)
            {
                return ptr_ > start_;
            }
            else
            {
                return ptr_ < end_ - 1;
            }
        }

        Iterator &operator++()
        {
            next();
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            next();
            return temp;
        }

        T &operator*() { return *ptr_; }
        const T &operator*() const { return *ptr_; }

        T *operator->() { return ptr_; }
        const T *operator->() const { return ptr_; }

        bool operator==(const Iterator &other) const
        {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const Iterator &other) const
        {
            return ptr_ != other.ptr_;
        }
    };

    class ConstIterator
    {
    private:
        const T *ptr_;
        const T *start_;
        const T *end_;
        bool reverse_;

    public:
        ConstIterator(const T *start, int size, const T *current, bool reverse)
            : ptr_(current),
              start_(start),
              end_(start + size),
              reverse_(reverse)
        {
            if (reverse && current == start && size > 0)
            {
                ptr_ = start + size - 1;
            }
        }

        const T &get() const { return *ptr_; }

        void next()
        {
            if (reverse_)
            {
                --ptr_;
            }
            else
            {
                ++ptr_;
            }
        }

        bool hasNext() const
        {
            if (reverse_)
            {
                return ptr_ > start_;
            }
            else
            {
                return ptr_ < end_ - 1;
            }
        }

        ConstIterator &operator++()
        {
            next();
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator temp = *this;
            next();
            return temp;
        }

        const T &operator*() const { return *ptr_; }
        const T *operator->() const { return ptr_; }

        bool operator==(const ConstIterator &other) const
        {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const ConstIterator &other) const
        {
            return ptr_ != other.ptr_;
        }
    };

    Iterator iterator()
    {
        return Iterator(data_, size_, data_, false);
    }

    Iterator reverseIterator()
    {
        if (size_ == 0)
        {
            return Iterator(data_, size_, data_, true);
        }
        return Iterator(data_, size_, data_ + size_ - 1, true);
    }

    ConstIterator iterator() const
    {
        return ConstIterator(data_, size_, data_, false);
    }

    ConstIterator reverseIterator() const
    {
        if (size_ == 0)
        {
            return ConstIterator(data_, size_, data_, true);
        }
        return ConstIterator(data_, size_, data_ + size_ - 1, true);
    }

    Iterator begin() { return Iterator(data_, size_, data_, false); }
    Iterator end() { return Iterator(data_, size_, data_ + size_, false); }

    ConstIterator begin() const { return ConstIterator(data_, size_, data_, false); }
    ConstIterator end() const { return ConstIterator(data_, size_, data_ + size_, false); }

    ConstIterator cbegin() const { return begin(); }
    ConstIterator cend() const { return end(); }

    T* begin_ptr() { return data_; }
    T* end_ptr() { return data_ + size_; }
};