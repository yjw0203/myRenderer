#pragma once
#include "Engine/Model/Public/yjw_model.h"
#include "Engine/Model/Private/model/yjw_model_implement.h"

namespace yjw
{
    class PMXModelBuilder : public ModelBuildImplement
    {
    public:
        ~PMXModelBuilder();
        virtual void FillModelData(const std::string& filePath, const std::string& fileName) override;
    };

}