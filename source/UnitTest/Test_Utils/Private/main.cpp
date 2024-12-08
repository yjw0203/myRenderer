#include "Engine/Utils/Public/Serialize/Stream.h"
#include "Engine/Utils/Public/DataStructure/Binary.h"
#include "iostream"
#include "json.hpp"
#include "main.h"
#include "Generate/Public/generate.h"
#include "Engine/Asset/Public/Asset.h"

using namespace yjw;

int main()
{
    Asset<A> ast("Content/1.ast");

    AssetManager::Get()->process();

    A* a = ast.GetData();
    a->y = 5;
    a->b.w = 3;
    a->b_array.push_back(B{ 1, 2, 3 });
    a->b_array.push_back(B{ 2, 3, 4 });
    ast.Save();

    return 0;
}
