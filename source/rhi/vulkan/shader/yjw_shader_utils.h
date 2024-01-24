#pragma once
#include <vector>
#include "yjw_shader_define.h"

namespace vulkan
{
	class ShaderPool
	{
	public:
		void createShaderFromFile(const char* fileName, Shader*& shader);
		void destoryShader(Shader*& shader);
	};

	EXTERN_GLOBAL_REF(ShaderPool);
}