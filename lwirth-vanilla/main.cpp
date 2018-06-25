#include <iostream>

#include <array>

std::array<int, 3> arr;

template<class ... Args, class SFINAE = std::enable_if_t<sizeof...(Args) == 3>>
void lol(Args ... args)
{
    arr = { static_cast<int>(args)... };
}

int main()
{
    lol(3.4, 5.2, 3.8);

    std::cin.get();
}