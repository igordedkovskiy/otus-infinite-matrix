#include <type_traits>
#include <vector>

template<typename T> concept Integer = std::is_integral_v<T>;

template<Integer T> class Matrix
{
public:
    ;
private:
    ;
};
