#include <lwirth.hpp>

void sortingTest()
{
    lw::Random random;

    using T = int;
    auto condition = [](T a, T b){return a <= b;};
    auto generator = [&](){return random.nextI32(-100, 100);};
    const size_t num = 10;

    lw::List<T> bubbleList;
    bubbleList.fillConstruct(num, generator);
    lw::List<T> quickList = bubbleList;
    for(const auto& e : quickList)
    {
        lw::print(e, " ");
    }
    lw::println("");
    size_t bubbleTime = lw::chronometer([&](){lw::bubbleSort(bubbleList.begin(), bubbleList.end(), condition);});
    size_t quickTime = lw::chronometer([&](){lw::quickSort(quickList.begin(), quickList.end(), condition);});
    lw::println("--------------------------------------");
    for(const auto& e : quickList)
    {
        lw::print(e, " ");
    }
    lw::println("--------------------------------------");
    lw::println("Bubble-time: ", bubbleTime, " | Quick-time: ", quickTime);


}
    
