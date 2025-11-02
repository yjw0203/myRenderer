#pragma once
#include "Engine/Model/Public/yjw_model.h"
#include "Engine/Model/Private/model/yjw_model_implement.h"

class PMXModelBuilder : public ModelBuildImplement
{
public:
    ~PMXModelBuilder();
    virtual void FillModelData(const std::string& filePath, const std::string& fileName) override;
};
