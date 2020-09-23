#include <iostream>
#include <tuple>


int main()
{
    auto tuple = std::tuple<std::string, int>("apple", 3);

    auto [fruit_name, quantity] = tuple;

    std::cout << fruit_name << " " << quantity << std::endl;
}