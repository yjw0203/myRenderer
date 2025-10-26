#if defined(__clang__)
#define Meta(...) __attribute__((annotate("CR"#__VA_ARGS__)))
#else
#define Meta(...)
#endif

#define Class(...) class Meta(type=class) __VA_ARGS__

//#include <iostream>
#include "main.generated.h"

int ff;

namespace TestNamespace
{
class Meta() TestC
{
    GENERATED_BODY();
public:
    int w;
};

Meta(1)
TestC ccc;
}


enum Meta(g) EA
{
    ea_a = 1,
    ea_b,
    ea_c
};

enum class EB
{
    eb_a = 1,
    eb_b,
    eb_c
};

class Meta(type = class) TestA : public TestNamespace::TestC
{
    GENERATED_BODY();
public:
    class B
    {
    public:
        int x;
    };
public:
    Meta(name=qqq)
    int qqq(int x) {};
    Meta(true)
    int test_x;
    Meta(static)
    static int gg;
private:
    Meta(This looks good)
    int test_y;
};

using namespace TestNamespace;
Meta(var=hhh)
TestNamespace::TestC aaa;

Meta(func)
void ggg(int x)
{

}

typedef TestA hhh;

int main()
{
    //TestA a;
    //std::cout << "ClassName is " << a.GetClassName() << std::endl;
    return 0;
}