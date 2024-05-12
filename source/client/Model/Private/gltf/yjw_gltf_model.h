#pragma once
#include "Public/Model/yjw_model.h"
#include "Private/model/yjw_model_implement.h"

namespace yjw
{
    class GLTFModelBuilder : public ModelBuildImplement
    {
    public:
        virtual void FillModelData(const std::string& filePath, const std::string& fileName) override;
    };

}