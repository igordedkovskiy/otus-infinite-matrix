#include <type_traits>
#include <unordered_map>
#include <iterator>

template<typename T> struct TD;

template<typename T> concept Integer = std::is_integral_v<T>;

template<Integer T, T default_v = 0> class InfiniteMatrix
{
public:
    using size_type = std::size_t;

    consteval T default_value() const noexcept
    {
        return default_v;
    }

    class row_t
    {
    public:
        using nested_row_t = std::unordered_map<size_type, T>;
        using Iterator = typename nested_row_t::iterator;
        using iterator = Iterator;
        using const_iterator = const Iterator;

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

        row_t& operator=(const row_t&) = delete;
        row_t& operator=(row_t&&) = delete;

        friend bool operator==(const row_t& l, const row_t& r)
        {
            return l.m_row == r.m_row;
        }

        size_type size() const noexcept
        {
            std::size_t sz = 0;
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

    using data_t = std::unordered_map<size_type, row_t>;

    struct Iterator
    {
        using element_type = std::tuple<size_type, size_type, T&>;

        using row_iterator = typename data_t::iterator;
        using el_iterator = typename data_t::mapped_type::iterator;
        using value_type = T;
        using pointer = T*;
        //using reference = T&;
        using reference = element_type;

        Iterator() noexcept = default;

        Iterator(data_t& data, const row_iterator& row, const el_iterator& el) noexcept:
            m_data{data},
            m_row_it{row},
            m_el_it{el}
        {}

        Iterator(const Iterator& l) noexcept:
            m_data{l.m_data},
            m_row_it{l.m_row_it},
            m_el_it{l.m_el_it}
        {}

        ~Iterator() noexcept = default;

        Iterator& operator=(const Iterator& l) noexcept
        {
            m_data = l.m_data;
            m_row_it = l.m_row_it;
            m_el_it = l.m_el_it;
            return *this;
        }

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
            return {m_row_it->first, m_el_it->first, m_el_it->second};
        }

        pointer operator->() const noexcept
        {
            return &(m_el_it->second);
        }

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


    row_t& operator[](const size_type& key)
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
