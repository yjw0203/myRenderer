#include "Utils/Serialize/Stream.h"
#include "Utils/DataStructure/Binary.h"
#include "iostream"
#include "json.hpp"
#include "main.h"
#include "Generate/generate.h"

using namespace yjw;

using json = nlohmann::json;

int main()
{
    json Json;
    A a;
    a.y = 5;
    a.b.x = 3;
    a.b.z = 6;
    a.b.w = 6;
    Json["aaa"] = a;
    std::cout << Json << std::endl;
    /*
    Stream stream;
    stream << a;
    stream >> a;
    std::cout << a.y << " " << a.b.x << " " << a.b.z;*/
    return 0;
}