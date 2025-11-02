#pragma once
#include "Engine/Model/Public/yjw_model.h"
#include "Engine/Model/Private/model/yjw_model_implement.h"

class GLTFModelBuilder : public ModelBuildImplement
{
public:
    GLTFModelBuilder(bool isBinary);
    virtual void FillModelData(const std::string& filePath, const std::string& fileName) override;
private:
    bool m_is_binary = false;
};
