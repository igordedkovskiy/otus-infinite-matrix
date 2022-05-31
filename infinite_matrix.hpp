#include <type_traits>
#include <unordered_map>
#include <iterator>

template<typename T> struct TD;

template<typename T> concept Integer = std::is_integral_v<T>;

template<Integer T, T default_v = 0> class InfiniteMatrix
{
public:
    using size_type = std::size_t;

    constexpr T default_value() const noexcept
    {
        return default_v;
    }

    class row_t
    {
    public:
//        class node_t
//        {
//        public:
//            node_t(const T& el, size_type row, size_type col):
//                m_element{el},
//                m_row_number{row},
//                m_column_number{col}
//            {}
//        private:
//            T m_element;
//            size_type m_row_number = 0;
//            size_type m_column_number = 0;
//        };

        using nested_row_t = std::unordered_map<size_type, T>;
        //using nested_row_t = std::unordered_map<size_type, node_t>;

//        struct Iterator
//        {
//            using value_type = std::pair<size_type, T>;
//            using pointer = T*;
//            using reference = T&;

//            Iterator() noexcept = default;

//            Iterator(const typename nested_row_t::iterator& p) noexcept:
//                ptr{p}
//            {}

//            Iterator(const Iterator& l) noexcept:
//                ptr{l.ptr}
//            {}

//            ~Iterator() noexcept = default;

//            Iterator& operator=(const Iterator& l) noexcept
//            {
//                ptr = l.ptr;
//                return *this;
//            }

//            Iterator& operator++() noexcept
//            {
//                ++ptr;
//                return *this;
//            }

//            Iterator& operator++(int) noexcept
//            {
//                ++ptr;
//                return *this;
//            }

//            reference operator*() const noexcept
//            {
//                return ptr->second;
//            }

//            pointer operator->() const noexcept
//            {
//                return &(ptr->second);
//            }

//            friend bool operator==(const Iterator& l, const Iterator& r) noexcept
//            {
//                return l.ptr == r.ptr;
//            }

//            friend bool operator!=(const Iterator& l, const Iterator& r) noexcept
//            {
//                return l.ptr != r.ptr;
//            }

//        private:
//            typename nested_row_t::iterator ptr;
//        };

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
                //auto res = m_row.emplace(std::make_pair(key, node_t{default_v, key}));
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
        using element_type = std::tuple<size_type, size_type, T>;

        using row_iterator = typename data_t::iterator;
        using el_iterator = typename data_t::mapped_type::iterator;
        using value_type = T;
        using pointer = T*;
        //using reference = T&;
        using reference = element_type&;

        Iterator() noexcept = default;

        Iterator(data_t& data, const row_iterator& row, const el_iterator& el) noexcept:
            m_data{data},
            m_row_it{row},
            m_el_it{el}
        {
            const auto rw = std::next(std::begin(m_data), m_data.size()-1);
            if((m_el_it != std::end(m_row_it->second)) && (m_el_it != std::end(rw->second)))
                m_element = std::make_tuple(m_row_it->first, m_el_it->first, m_el_it->second);
        }

        Iterator(const Iterator& l) noexcept:
            m_data{l.m_data},
            m_row_it{l.m_row_it},
            m_el_it{l.m_el_it},
            m_element{l.m_element}
        {}

        ~Iterator() noexcept = default;

        Iterator& operator=(const Iterator& l) noexcept
        {
            m_data = l.m_data;
            m_row_it = l.m_row_it;
            m_el_it = l.m_el_it;
            m_element = l.m_element;
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
            if(m_el_it != std::end(m_row_it->second))
                std::get<1>(m_element) = m_el_it->first;
            if(m_row_it != std::end(m_data))
                std::get<0>(m_element) = m_row_it->first;
            return *this;
        }

        reference operator*() noexcept
        {
            std::get<2>(m_element) = m_el_it->second;
            return m_element;
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
        element_type m_element{0,0,0};
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
        const auto el = std::end(row->second);
        return {m_matrix, std::end(m_matrix), el};
    }

    const_iterator end() const noexcept
    {
        const auto row = std::next(std::begin(m_matrix), m_matrix.size()-1);
        const auto el = std::end(row->second);
        return {m_matrix, std::end(m_matrix), el};
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
//    using data_t = std::unordered_map<size_type, row_t>;
    //using data_t = std::unordered_map<size_type, node_t>;
    data_t m_matrix;
};
