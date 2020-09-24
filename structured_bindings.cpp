// Tutorial on how to use structured bindings of C++17 to
// write more functional style code.
//
// Dmitrii Khizbullin, 2020
//
// This example shows how to create a lambda, instantly call it,
// return multiple values from it and unpack them as const-s
// in the caller scope, thus protecting from occasional modification.


#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>


// Here we declare a helper function to print out a vector to the console.
template<typename T>
void printVector(const std::vector<T>& vector)
{
    std::cout << "[ ";
    for (const auto& elem: vector)
    {
        std::cout << elem << " ";
    }
    std::cout << "]";
};


// Entry point to the application.
// In this example we are going to a median value of an array of floats.
// In particular we want to figure out not just the median value itself,
// but an index (or 2 indices) of elements from which the mean
// was calculated. We get 2 indices when the array is of an odd size,
// and to preserve symmetry we have to take 2 central elements of
// the sorted array.
int main()
{
    // Let's create an vector of floats to run our algorithm on.
    const std::vector<double> elements {
        1.2, 1.1, -0.1, -0.2, 0, 1
        };

    {
        // Print out the values.
        std::cout << "elements=";
        printVector(elements);
        std::cout << std::endl;
    }

    // At this point we declare what we want - constant value variables
    // "median_value" and indices. They are returned from the lambda and
    // unpacked by means of a structured binding into separate variables
    // right in the current scope.
    // Please notice that we do not capture "elements" into lambda's
    // capture list. A reference (&) capture list enables non-const access
    // to the outer-scope "elements". If for some reason "elements" are
    // declared non-const, lambda's body may by mistake modify its contents.
    // Instead we pass elements as a const reference parameter.
    const auto [median_value, indices] = [](const std::vector<double>& elements)
    {
        // Inside the lambda we allow ourselves some non-functional style by
        // initializing the result variable with a default value which is
        // a tuple of NaN and an empty list to account for possible
        // empty input list.
        auto result = std::make_tuple(
            std::numeric_limits<double>::quiet_NaN(),
            std::vector<size_t>());

        auto size = elements.size();

        if (size > 0)
        {
            // To keep track of the indices which were used to calculate
            // a median, we enumerate elements much alike in python.
            std::vector<std::pair<size_t, double> > enumeratedElements;
            for (size_t index = 0; index < elements.size(); ++index)
            {
                enumeratedElements.emplace_back(index, elements[index]);
            }

            // Now we are all set to sort the elements by values which
            // are stored in "second" field of a tuple.
            std::sort(enumeratedElements.begin(), enumeratedElements.end(),
                [](auto a, auto b){ return a.second > b.second; });

            // A median is calculated out of a middle element of odd-sized
            // lists and out of the two middle elements of an even-sized list.
            std::vector<size_t> indices = (size % 2 == 0) ?
                std::vector<size_t>{size / 2 - 1, size / 2} :
                std::vector<size_t>{size / 2};

            // Let's disentangle indices and values back from the tuples
            // that we are interested in.
            std::vector<size_t> originalIndices;
            std::vector<double> values;
            for (const auto& index : indices)
            {
                originalIndices.push_back(enumeratedElements[index].first);
                values.push_back(enumeratedElements[index].second);
            }

            // Here we stick to the functional paradigm again to mean-reduce
            // one or two values of interest.
            auto median = std::accumulate(values.begin(), values.end(), 0.0) /
                values.size();

            // And return a tuple to be unpacked with a structured binding.
            result = std::make_tuple(median, originalIndices);
        }
        return result;
    }(elements); // Our lambda is one-time use, so just call it.

    // ....... long code here .........

    // By mistake we try to reuse short variable names,
    // but therefore overwrite the original values

    // median_value = 12.3; // Does not compile
    // indices = std::vector<size_t>{100, 200}; // Does not compile

    // Voila! Our fancy const structured bindings save us from a bug,
    // and this happens at compile stage, not after a couple of hours
    // of debugging.

    // ....... more code here .........

    // At some point we again need the original "median_value" or "indices".
    // It is safe to assume that the values are never corrupted
    // thanks to the const qualifier.

    {
        // Print out the results.
        std::cout << "median_value=" << median_value << " ";
        std::cout << "indices=";
        printVector(indices);
        std::cout << std::endl;
    }

    return 0;
}
