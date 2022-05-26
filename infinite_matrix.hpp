#include <type_traits>
#include <vector>
#include <unordered_map>

template<typename T> concept Integer = std::is_integral_v<T>;

template<Integer T, T default_v = 0> class InfiniteMatrix
{
public:
    using size_type = std::size_t;

    class Proxy
    {
    public:
        using row_t = std::unordered_map<size_type,T>;

        T& operator[](const T& key)
        {
            auto el = m_row.find(key);
            if(el == m_row.end())
                ++m_size;
            return m_row[key];
        }

        Proxy& operator=(const Proxy&) = delete;
        Proxy& operator=(Proxy&&) = delete;

    private:
        row_t m_row;
        size_type m_size = 0;
    };

    using row_t = Proxy;
    row_t& operator[](const T& key)
    {
        return m_matrix[key];
    }

    size_type size() const noexcept
    {
//        size_type sz = 0;
//        for(const auto& it:m_matrix)
//            sz += it.size();
//        return sz;
        return m_size;
    }

private:
    using data_t = std::unordered_map<size_type, Proxy>;
    data_t m_matrix;
    size_type m_size = 0;
};
