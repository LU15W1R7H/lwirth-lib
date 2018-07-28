#include <iostream>

template<typename T, size_t N>
struct Components
{
public:
    using value_type = T;

    constexpr static size_t getDim() { return N; }


    union
    {
        T arr[N];
    };
};

template<class COMPONENTS>
struct Vector : public COMPONENTS
{
    typename COMPONENTS::value_type mag()
    {
        return 22.f;
    }
};

int main()
{
    Vector<Components<float, 3>> vec;
    vec.mag();
}