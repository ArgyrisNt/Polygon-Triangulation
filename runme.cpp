#include <iostream>
#include "polygon.h"


int main()
{
    std::vector<vertex> vertices1{{0.0, 0.0}, {1.0, -1.0}, {2.0, 1.0}, {3.0, 0.0}, {4.0, 3.0}, {2.0, 4.0}, {1.0, 3.0}, {0.72, 1.58}, {0.24, 1.84}};

    std::vector<vertex> vertices2{{2, 2}, {3, 1}, {3.98, 1.56}, {5, 0}, {4.56, 3.02}, {6, 2}, {6.95, 4.59}, {5.34, 4.21},
                                  {5, 7}, {4.16, 6}, {3.38, 7}, {2.68, 5.68}, {3.1, 4.82}, {2.84, 4}, {2.22, 4.44}};

    polygon poly(vertices2);
    poly.makeMonotone();

    std::cout << poly;

    return 0;
}