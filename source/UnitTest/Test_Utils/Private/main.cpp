#include "Engine/Utils/Public/Serialize/Stream.h"
#include "Engine/Utils/Public/DataStructure/Binary.h"
#include "iostream"
#include "json.hpp"
#include "main.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Engine/Public/Asset/Mesh.h"
#include "Engine/Math/Public/Math.h"

#include "Engine/Utils/Public/Serialize/FileArchive.h"
#include "projectInfo.h"


class AA
{
public:
    struct B
    {
        char c;
        int a;
    };
    void Serilize(Archive& Ar)
    {
        Ar << x;
        Ar << y;
        Ar << z;
        Ar << b;
        Ar.Serialize(q, sizeof(q));
        Ar << v;
        Ar << mp;
        Ar.close();
    }

    int x{};
    int y{};
    int z{};
    B b{};
    int q[2];
    std::vector<int>v;
    std::map<std::string, std::vector<std::string>> mp;
};



int main()
{
    /*
    Asset<MeshAST> ast("Content/3.ast");

    AssetManager::Get()->process();

    MeshAST* a = ast.GetData();
    ast.Save();
    */
    AA a;
    a.x = 1;
    a.y = 2;
    a.z = 3;
    a.b.a = 4;
    a.b.c = '1';
    a.q[0] = 4;
    a.q[1] = 5;
    a.v.resize(3);
    a.v[0] = 7;
    a.v[1] = 8;
    a.v[2] = 9;
    a.mp["aaa"].push_back("qqq");
    a.mp["aaa"].push_back("www");
    a.mp["bbb"].push_back("eee");
    a.mp["ccc"].push_back("rrr");

    Archive* writer = new FileWriterArchive(RESOURCE_FILE(test_archive.txt));
    a.Serilize(*writer);

    Archive* reader = new FileReaderArchive(RESOURCE_FILE(test_archive.txt));
    AA b;
    b.Serilize(*reader);
    printf("a:{%d,%d,%d,{%c,%d},[%d,%d],[%d,%d,%d]}\n", b.x, b.y, b.z, b.b.c, b.b.a, b.q[0], b.q[1], b.v[0], b.v[1], b.v[2]);
    for (auto it : b.mp)
    {
        printf("(%s,[", it.first.c_str());
        for (auto v : it.second)
        {
            printf("%s ", v.c_str());
        }
        printf("])\n");
    }

    return 0;
}
