#include <type_traits>
#include <unordered_map>
#include <iterator>

template<typename T> concept Integer = std::is_integral_v<T>;

template<Integer T, T default_v = 0> class InfiniteMatrix
{
public:
    using size_type = std::size_t;

    class row_t
    {
    public:

        class node_t
        {
        public:
            T& operator=(const T& v)
            {
                element = v;
            }
        private:
            T element;
        };

        using nested_row_t = std::unordered_map<size_type,T>;

        T& operator[](const T& key)
        {
            auto el = m_row.find(key);
            if(el == m_row.end())
            {
                auto res = m_row.emplace(std::make_pair(key, default_v));
                //if(!res.second)
                //    throw?
                el = res.first;
            }
            return el->second;
        }

        row_t& operator=(const row_t&) = delete;
        row_t& operator=(row_t&&) = delete;

        size_type size() const noexcept
        {
            return m_row.size();
        }

    private:
        nested_row_t m_row;
    };

    row_t& operator[](const T& key)
    {
        return m_matrix[key];
    }

    size_type size() const noexcept
    {
        size_type sz = 0;
        for(const auto& it:m_matrix)
            sz += it.second.size();
        return sz;
    }


//    struct Iterator
//    {
//        using value_type = T;
//        using pointer = T*;
//        using reference = T&;

//        Iterator() noexcept = default;

//        Iterator(pointer p) noexcept:
//            ptr{p}
//        {}

//        Iterator(const Iterator& l) noexcept:
//            ptr{l.ptr}
//        {}

//        ~Iterator() noexcept = default;

//        Iterator& operator=(const Iterator& l) noexcept
//        {
//            ptr = l.ptr;
//            return *this;
//        }

//        Iterator& operator++() noexcept
//        {
//            ++ptr;
//            return *this;
//        }

//        Iterator& operator--() noexcept
//        {
//            --ptr;
//            return *this;
//        }

//        reference operator*() const noexcept
//        {
//            return *ptr;
//        }

//        pointer operator->() const noexcept
//        {
//            return ptr;
//        }

//        friend bool operator==(const Iterator& l, const Iterator& r) noexcept
//        {
//            return l.ptr == r.ptr;
//        }

//        friend bool operator!=(const Iterator& l, const Iterator& r) noexcept
//        {
//            return l.ptr != r.ptr;
//        }

//    private:
//        T* ptr = nullptr;
//    };

//    iterator begin() noexcept
//    {
//        return {m_start};
//    }

//    const_iterator begin() const noexcept
//    {
//        return {m_start};
//    }

//    iterator end() noexcept
//    {
//        return {m_end};
//    }

//    const_iterator end() const noexcept
//    {
//        return {m_end};
//    }

private:
    using data_t = std::unordered_map<size_type, row_t>;
    data_t m_matrix;
};
