#include <iostream>
#include "matrix.hpp"

using namespace Slate;
using namespace Slate::Math;

int main()
{
    Vector<2, double> u{}, v{};

    u[0] = 3;
    u[1] = 4;

    v[0] = 3;
    v[1] = 4;

    std::cout << u.norm() << std::endl;
}