#pragma once

namespace rhi
{
	struct CreateInfo
	{
		void* window;
	};

	void rhiInit(CreateInfo &info);

	void rhiBeginFrame();

	void rhiEndFrame();


}