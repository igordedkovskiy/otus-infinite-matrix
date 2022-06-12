#include <type_traits>
#include <unordered_map>
#include <iterator>
#include <utility>

template<typename T> struct TD;

template<typename T> concept Integer = std::is_integral_v<T>;

template<Integer T, std::size_t N, T default_v> class InfiniteMatrix
{
public:
    using size_type = std::size_t;

    InfiniteMatrix() noexcept = default;
    InfiniteMatrix(const InfiniteMatrix&) noexcept = default;
    InfiniteMatrix(InfiniteMatrix&&) noexcept = default;
    InfiniteMatrix& operator=(const InfiniteMatrix&) noexcept = default;
    InfiniteMatrix& operator=(InfiniteMatrix&&) noexcept = default;
    ~InfiniteMatrix() noexcept = default;

    consteval T default_value() const noexcept
    {
        return default_v;
    }

    /// \brief \a row_t is a wrapper, needed to overload operator[]
    template<std::size_t S, bool Dummy = true> class row_t
    {
    public:
        using nested_row_t = std::unordered_map<size_type, row_t<S-1>>;

        struct Iterator
        {
            using row_iterator = typename nested_row_t::iterator;
            using el_iterator = typename nested_row_t::mapped_type::iterator;
            using value_type = typename nested_row_t::value_type;
            using pointer = typename nested_row_t::pointer;
            using element_type = decltype(std::tuple_cat(std::tie(std::declval<row_iterator>()->first),
                                                                  *std::declval<el_iterator>()));
            using reference = element_type;

            Iterator() noexcept = delete;
            Iterator(const Iterator&) noexcept = default;
            Iterator(Iterator&&) noexcept = default;
            ~Iterator() noexcept = default;

            Iterator& operator=(const Iterator& l) noexcept
            {
                m_data = l.m_data;
                m_row_it = l.m_row_it;
                m_el_it = l.m_el_it;
                return *this;
            }
            Iterator& operator=(Iterator&& l) noexcept
            {
                m_data = l.m_data;
                m_row_it = std::move(l.m_row_it);
                m_el_it = std::move(l.m_el_it);
                return *this;
            }

            Iterator(nested_row_t& data, const row_iterator& row, const el_iterator& el) noexcept:
                m_data{data},
                m_row_it{row},
                m_el_it{el}
            {}

            Iterator& operator++() noexcept
            {
                ++m_el_it;
                if(m_el_it == std::end(m_row_it->second))
                {
                    ++m_row_it;
                    if(m_row_it != std::end(m_data))
                        m_el_it = std::begin(m_row_it->second);
                }
                return *this;
            }

            reference operator*() noexcept
            {
                return std::tuple_cat(std::tie(m_row_it->first), *m_el_it);
            }

//            pointer operator->() const noexcept
//            {
//                return &(m_el_it->second);
//            }

            friend bool operator==(const Iterator& l, const Iterator& r) noexcept
            {
                return (l.m_data == r.m_data) && (l.m_row_it == r.m_row_it) && (l.m_el_it == r.m_el_it);
            }

            friend bool operator!=(const Iterator& l, const Iterator& r) noexcept
            {
                return (l.m_data != r.m_data) || (l.m_row_it != r.m_row_it) || (l.m_el_it != r.m_el_it);
            }

        private:
            nested_row_t& m_data;
            row_iterator  m_row_it;
            el_iterator   m_el_it;
        };

        using iterator = Iterator;
        using const_iterator = const Iterator;

        row_t() noexcept = default;
        row_t(const row_t&) noexcept = default;
        row_t(row_t&&) noexcept = default;
        row_t& operator=(const row_t&) noexcept = default;
        row_t& operator=(row_t&&) noexcept = default;
        ~row_t() noexcept = default;


        iterator begin() noexcept
        {
            return {m_row, std::begin(m_row), std::begin(std::begin(m_row)->second)};
        }

        const_iterator begin() const noexcept
        {
            return {m_row, std::begin(m_row), std::begin(std::begin(m_row)->second)};
        }

        iterator end() noexcept
        {
            const auto row = std::next(std::begin(m_row), m_row.size()-1);
            return {m_row, std::end(m_row), std::end(row->second)};
        }

        const_iterator end() const noexcept
        {
            const auto row = std::next(std::begin(m_row), m_row.size()-1);
            return {m_row, std::end(m_row), std::end(row->second)};
        }


        friend bool operator==(const row_t& l, const row_t& r)
        {
            return l.m_row == r.m_row;
        }

        /// \note operator[] is overloaded to insert correct default cell value, which is impossible
        ///       to achieve with nested_row_t::operator[]
        row_t<S-1>& operator[](const size_type& key)
        {
            return m_row[key];
        }

        size_type size() const noexcept
        {
            size_type sz = 0;
            for(const auto& it:m_row)
                sz += it.second.size();
            return sz;
        }

    private:
        nested_row_t m_row;
    };

    template<bool Dummy> class row_t<1, Dummy>
    {
    public:
        using nested_row_t = std::unordered_map<size_type, T>;

        struct Iterator
        {
            using el_iterator = typename nested_row_t::iterator;
            using pointer = T*;
            using element_type = std::tuple<size_type, T&>;
            using reference = element_type;

            Iterator() noexcept = delete;
            Iterator(const Iterator&) noexcept = default;
            Iterator(Iterator&&) noexcept = default;
            Iterator& operator=(const Iterator&) noexcept = default;
            Iterator& operator=(Iterator&&) noexcept = default;
            ~Iterator() noexcept = default;

            Iterator(const el_iterator& el) noexcept:
                m_el_it{el}
            {}

            Iterator& operator++() noexcept
            {
                ++m_el_it;
                return *this;
            }

            reference operator*() noexcept
            {
                return {m_el_it->first, m_el_it->second};
            }

            pointer operator->() const noexcept
            {
                return &(m_el_it->second);
            }

            friend bool operator==(const Iterator& l, const Iterator& r) noexcept
            {
                return l.m_el_it == r.m_el_it;
            }

            friend bool operator!=(const Iterator& l, const Iterator& r) noexcept
            {
                return l.m_el_it != r.m_el_it;
            }
        private:
            el_iterator   m_el_it;
        };

        using iterator = Iterator;
        using const_iterator = const Iterator;

        row_t() noexcept = default;
        row_t(const row_t&) noexcept = default;
        row_t(row_t&&) noexcept = default;
        row_t& operator=(const row_t&) noexcept = default;
        row_t& operator=(row_t&&) noexcept = default;
        ~row_t() noexcept = default;


        iterator begin() noexcept
        {
            return {std::begin(m_row)};
        }

        const_iterator begin() const noexcept
        {
            return {std::begin(m_row)};
        }

        iterator end() noexcept
        {
            return {std::end(m_row)};
        }

        const_iterator end() const noexcept
        {
            return {std::end(m_row)};
        }


        friend bool operator==(const row_t& l, const row_t& r)
        {
            return l.m_row == r.m_row;
        }

        /// \note operator[] is overloaded to insert correct default cell value, which is impossible
        ///       to achieve with nested_row_t::operator[]
        T& operator[](const size_type& key)
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

        size_type size() const noexcept
        {
            size_type sz = 0;
            for(const auto& [key, val]:m_row)
            {
                if(val != default_v)
                    ++sz;
            }
            return sz;
        }

    private:
        nested_row_t m_row;
    };


    using data_t = std::unordered_map<size_type, row_t<N-1>>;

    struct Iterator
    {
        using row_iterator = typename data_t::iterator;
        using el_iterator = typename row_t<N-1>::iterator;
//        using value_type = T;
//        using pointer = T*;
        using element_type = decltype(std::tuple_cat(std::tie(std::declval<row_iterator>()->first),
                                                              *std::declval<el_iterator>()));
        using reference = element_type;

        Iterator() noexcept = delete;
        Iterator(const Iterator&) noexcept = default;
        Iterator(Iterator&&) noexcept = default;
        Iterator& operator=(const Iterator& l) noexcept = default;
        Iterator& operator=(Iterator&& l) noexcept = default;
        ~Iterator() noexcept = default;

        Iterator(data_t& data, const row_iterator& row, const el_iterator& el) noexcept:
            m_data{data},
            m_row_it{row},
            m_el_it{el}
        {}

        Iterator& operator++() noexcept
        {
            ++m_el_it;
            if(m_el_it == std::end(m_row_it->second))
            {
                ++m_row_it;
                if(m_row_it != std::end(m_data))
                    m_el_it = std::begin(m_row_it->second);
            }
            return *this;
        }

        reference operator*() noexcept
        {
            return std::tuple_cat(std::tie(m_row_it->first), *m_el_it);
        }

//        pointer operator->() const noexcept
//        {
//            return &(m_el_it->second);
//        }

        friend bool operator==(const Iterator& l, const Iterator& r) noexcept
        {
            return (l.m_data == r.m_data) && (l.m_row_it == r.m_row_it) && (l.m_el_it == r.m_el_it);
        }

        friend bool operator!=(const Iterator& l, const Iterator& r) noexcept
        {
            return (l.m_data != r.m_data) || (l.m_row_it != r.m_row_it) || (l.m_el_it != r.m_el_it);
        }

    private:
        data_t& m_data;
        row_iterator m_row_it;
        el_iterator m_el_it;
    };

    using iterator = Iterator;
    using const_iterator = const Iterator;

    iterator begin() noexcept
    {
        return {m_matrix, std::begin(m_matrix), std::begin(std::begin(m_matrix)->second)};
    }

    const_iterator begin() const noexcept
    {
        return {m_matrix, std::begin(m_matrix), std::begin(std::begin(m_matrix)->second)};
    }

    iterator end() noexcept
    {
        const auto row = std::next(std::begin(m_matrix), m_matrix.size()-1);
        return {m_matrix, std::end(m_matrix), std::end(row->second)};
    }

    const_iterator end() const noexcept
    {
        const auto row = std::next(std::begin(m_matrix), m_matrix.size()-1);
        return {m_matrix, std::end(m_matrix), std::end(row->second)};
    }

    row_t<N-1>& operator[](const size_type& key)
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

private:
    data_t m_matrix;
};


