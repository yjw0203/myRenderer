#pragma once
#include "Utils/Serialize/Stream.h"
#include "Utils/DataStructure/Binary.h"
#include "iostream"

using namespace yjw;

struct B
{
public:
    int x;
    int z;
};

class A : public Serializable
{
public:
    int y;
    B b;
};

int main()
{
    A a;
    a.y = 5;
    a.b.x = 3;
    a.b.z = 6;
    Stream stream;
    stream << a;
    stream >> a;
    std::cout << a.y << " " << a.b.x << " " << a.b.z;
    return 0;
}