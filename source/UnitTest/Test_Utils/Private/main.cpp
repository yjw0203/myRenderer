#include "Engine/Utils/Public/Serialize/Stream.h"
#include "Engine/Utils/Public/DataStructure/Binary.h"
#include "iostream"
#include "json.hpp"
#include "main.h"
#include "Generate/Public/generate.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Engine/Public/Framework/Asset/Mesh.h"

using namespace yjw;

int main()
{
    Asset<yjw::MeshAST> ast("Content/3.ast");

    AssetManager::Get()->process();

    yjw::MeshAST* a = ast.GetData();
    ast.Save();

    return 0;
}
