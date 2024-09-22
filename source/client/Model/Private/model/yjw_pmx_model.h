#pragma once
#include "Public/Model/yjw_model.h"
#include "Private/model/yjw_model_implement.h"

namespace yjw
{
    class PMXModelBuilder : public ModelBuildImplement
    {
    public:
        ~PMXModelBuilder();
        virtual void FillModelData(const std::string& filePath, const std::string& fileName) override;
    };

}