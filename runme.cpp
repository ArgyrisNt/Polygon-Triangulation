#include <iostream>
#include "polygon.h"

int main()
{
    std::vector<edge> edges{{{0.0, 0.0}, {1.0, -1.0}}, {{1.0, -1.0}, {2.0, 1.0}}, {{2.0, 1.0}, {3.0, 0.0}}, {{3.0, 0.0}, {4.0, 3.0}}, {{4.0, 3.0}, {2.0, 4.0}}, 
                            {{2.0, 4.0}, {1.0, 3.0}}, {{1.0, 3.0}, {0.72, 1.58}}, {{0.72, 1.58}, {0.24, 1.84}}, {{0.24, 1.84}, {0.0, 0.0}}};
    polygon poly(edges);
    poly.makeMonotone();

    std::cout << poly;

    return 0;
}