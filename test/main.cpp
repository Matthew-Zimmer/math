#include <iostream>
#include "matrix.hpp"

using namespace Slate;
using namespace Slate::Math;

int main()
{
    Vector<2, double> u{ 3, 4 }, v{};

    v[0] = 3;
    v[1] = 4;

    std::cout << u.norm() << std::endl;
}