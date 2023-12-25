#pragma once
#include <vector>
#include "yjw_shader_define.h"

namespace vulkan{
namespace shader{
namespace utils
{
	void loadFromSpirvFile(const char* fileName, Shader* shader);
	void getReflectFromSpirv(const char* spirvPointer, int spirvSize, std::unordered_map<std::string, ShaderReflect>& refects);
}}}