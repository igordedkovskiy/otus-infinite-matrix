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
            node_t(const T& v, row_t& r) noexcept:
                m_element{v},
                m_parent{r}
            {
                check_size();
            }
            node_t(T&& v, row_t& r) noexcept:
                m_element{std::move(v)},
                m_parent{r}
            {
                check_size();
            }

            node_t& operator=(const T& v)
            {
                if(m_element == default_v && v != default_v)
                    ++m_parent.m_size;
                else if(m_element != default_v && v == default_v)
                    --m_parent.m_size;
                m_element = v;
                return *this;
            }

            node_t& operator++() noexcept
            {
                ++m_element;
                return *this;
            }

            node_t& operator++(int) noexcept
            {
                m_element++;
                return *this;
            }

            friend bool operator==(const T& l, const node_t& r) noexcept
            {
                return r.m_element == l;
            }

            friend bool operator==(const node_t& l, const T& r) noexcept
            {
                return l.m_element == r;
            }

//            friend node_t& operator+(const T& l, const node_t& r) noexcept
//            {
//                return r.m_element + l;
//            }

//            friend node_t& operator+(const node_t& l, const T& r) noexcept
//            {
//                return l.m_element + r;
//            }

        private:
            void check_size() noexcept
            {
                if(m_element != default_v)
                    ++m_parent.m_size;
            }

            T m_element;
            row_t& m_parent;
        };

        using nested_row_t = std::unordered_map<size_type, node_t>;

        node_t& operator[](const T& key)
        {
            auto el = m_row.find(key);
            if(el == m_row.end())
            {
                auto res = m_row.emplace(std::make_pair(key, node_t{default_v, *this}));
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
            return m_size;
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
        std::size_t m_size = 0;
    };

//    class node_t
//    {
//    public:
////        node_t(){}
//        node_t& operator[](const T& key)
//        {
//            return m_row[key];
//        }

//        auto size() const
//        {
//            return m_row.size();
//        }
//    private:
//        row_t m_row;
//    };

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

private:
    using data_t = std::unordered_map<size_type, row_t>;
    //using data_t = std::unordered_map<size_type, node_t>;
    data_t m_matrix;
};
