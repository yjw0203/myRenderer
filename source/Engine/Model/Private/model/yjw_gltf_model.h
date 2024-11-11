#pragma once
#include "Public/Model/yjw_model.h"
#include "Private/model/yjw_model_implement.h"

namespace yjw
{
    class GLTFModelBuilder : public ModelBuildImplement
    {
    public:
        GLTFModelBuilder(bool isBinary);
        virtual void FillModelData(const std::string& filePath, const std::string& fileName) override;
    private:
        bool m_is_binary = false;
    };

}