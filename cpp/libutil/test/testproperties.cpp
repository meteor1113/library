#pragma warning(disable: 4786)
#include <assert.h>

#include <iostream>

#include "properties.hpp"

using namespace util;

void Test()
{
    Properties pro1("a.properties");
    pro1.Set("dafd", "dafs{1}");
    pro1.Set("a", "");
    pro1.Save();
    std::string v = pro1.Get("dafs");
    std::cout << v << std::endl;
    assert(v == "dafs");
    std::cout << pro1.Get("dafd") << std::endl;
    assert("dafs{1}" == pro1.Get("dafd"));
    std::cout << pro1.Get("dafd", "test") << std::endl;
    assert("dafstest" == pro1.Get("dafd", "test"));
    pro1.Set("z", "zxcv");
    pro1.Save();

    Properties pro2("a.properties");
    std::string v1 = pro1.Get("z");
    std::cout << v1 << std::endl;
    assert("zxcv" == v1);
}


int main()
{
    Test();
    return 0;
}
