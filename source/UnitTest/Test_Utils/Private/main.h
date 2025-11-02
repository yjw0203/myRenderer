#pragma once
#if defined(__clang__)
#define Meta(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#define Meta(...)
#endif

#define Class(...) class Meta(type=class) __VA_ARGS__

#include <vector>
#include "Generate/Public/Generate.h"

Class(B)
{
    GENERATED_BODY();
public:
    int x;
    int z;
    int w;
};

Class(A)
{
    GENERATED_BODY();
public:
    int y;
    B b;
    std::vector<B> b_array;
};
