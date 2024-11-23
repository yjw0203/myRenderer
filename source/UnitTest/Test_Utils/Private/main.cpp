#pragma once
#include "Utils/Serialize/Stream.h"
#include "Utils/DataStructure/Binary.h"
#include "iostream"
#include "json.hpp"

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

using json = nlohmann::json;
void to_json(json& j, const B& b) {
    j = json{ {"x",  b.x},
             {"z", b.z} };
}

void from_json(const json& j, B& a) {
    j.at("x").get_to(a.x);
    j.at("z").get_to(a.z);
}
void to_json(json& j, const A& a) {
    j = json{ {"y",  a.y},
             {"b", a.b} };
}

void from_json(const json& j, A& a) {
    j.at("y").get_to(a.y);
    j.at("b").get_to(a.b);
}

int main()
{
    json Json;
    A a;
    a.y = 5;
    a.b.x = 3;
    a.b.z = 6;
    Json["aaa"] = a;
    std::cout << Json << std::endl;
    Stream stream;
    stream << a;
    stream >> a;
    std::cout << a.y << " " << a.b.x << " " << a.b.z;
    return 0;
}