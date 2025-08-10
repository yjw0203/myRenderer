#if defined(__clang__)
#define Meta(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#define Meta(...)
#endif

#define Class(...) class Meta(type=class) __VA_ARGS__

//#include <iostream>
#include "main.generated.h"

Class(TestC)
{
    GENERATED_BODY();
public:
    int w;
};

Class(TestA) : public TestC
{
    GENERATED_BODY();
public:
    class B
    {
    public:
        int x;
    };
public:
    int qqq(int x) {};
    Meta(true)
    int test_x;
private:
    Meta(This looks good)
    int test_y;
};

typedef TestA hhh;

int main()
{
    //TestA a;
    //std::cout << "ClassName is " << a.GetClassName() << std::endl;
    return 0;
}