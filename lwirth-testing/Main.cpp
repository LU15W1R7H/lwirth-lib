#include <lwirth.hpp>

lw::Random random;

void func(size_t runs)
{
    for (int i = 0; i < runs; ++i) {
        random.nextI32(0);

    }
}

int main()
{
    lw::println("Begin");

    //u64 dur = lw::chronometer(&lw::Random::nextColor, random);
    //i64 dur = lw::chronometer(func, 100000);



    lw::println(dur);
    //std::cin.get();
    return 0;
}