#include "parser.hpp"

int main()
{
    constexpr auto p = Parser("bsh");
    constexpr auto p1 = p.parse("bs");
    std::cout << p1;
    constexpr auto p2 = p1.parse('h');
    std::cout << p2;
    constexpr auto p3 = p2.parse("aaa");
    std::cout << p3;
    constexpr auto p4 = p.parse("sdfsdf");
    std::cout << p4;
    return 0;
}
