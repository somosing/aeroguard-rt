#include "aeroguard/version.hpp"

#include <iostream>
#include <string_view>

int main()
{
    constexpr std::string_view expected{"0.1.0-dev"};

    if (aeroguard::version() != expected) {
        std::cerr << "Expected version " << expected
                  << ", got " << aeroguard::version() << '\n';
        return 1;
    }

    return 0;
}
