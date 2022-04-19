#include <iostream>

#include "adj_list.h"

int main()
{
    graph<int> something;
    something += 3;
    something += 8;
    something(3, 8) = 5;
    std::cout << "The edge from " << something[0] << " to " <<
              something[1] << " has a weight of " << something(3, 8) << std::endl;
    std::cout << something[0][1] << " is the first neighbor of " <<
              something[0] << std::endl;
    return 0;
}
