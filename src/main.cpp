#include "aeroguard/version.hpp"

#include <iostream>

int main()
{
    std::cout << "AeroGuard-RT " << aeroguard::version() << '\n';
    return 0;
}
