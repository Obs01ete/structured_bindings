#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <limits>


int main()
{
    std::vector<double> elements {
        0.6, 0.4, 0.1, 4.4, 0.6, 4.1, 1.4, 2.4, 4.1, 2.8
        };

    auto printVector = [](const std::vector<double> vector)
    {
        std::cout << "[ ";
        for (const auto& elem: vector)
        {
            std::cout << elem << " ";
        }
        std::cout << "]";
    };

    printVector(elements);
    std::cout << std::endl;

    const auto [median_value, indices] = [&elements]()
    {
        auto result = std::make_tuple(
            std::numeric_limits<double>::quiet_NaN(),
            std::vector<size_t>());

        auto size = elements.size();

        if (size > 0)
        {
            auto elementsCopy = elements;
            std::sort(elementsCopy.begin(), elementsCopy.end());

            if (size % 2 == 0)
            {
                std::vector<size_t> indices {size / 2 - 1, size / 2};
                result = std::make_tuple(
                    (elements[indices[0]] + elements[indices[1]]) / 2.0,
                    indices);
            }
            else
            {
                std::vector<size_t> indices {size / 2};
                result = std::make_tuple(
                    elements[indices[0]],
                    indices);
            }
        }
        return result;
    }();

    printVector(elements);
    std::cout << std::endl;

    // ....... long code here .........

    // By mistake we try to reuse short variable names,
    // but therefore overwrite the original values

    // median_value = 12.3; // Does not compile
    // indices = {100, 200} // Does not compile

    // Voila! Our fancy const structured bindings save us from a bug,
    // and this happens at compile stage, not after a couple of hours
    // of debugging.

    // ....... more code here .........

    // At some point we again need the original "median_value".

    {
        std::cout << "median_value=" << median_value << " ";
        std::cout << "indices=[ ";
        for (const auto& index: indices)
        {
            std::cout << index << " ";
        }
        std::cout << "]" << std::endl;
    }

    return 0;
}
