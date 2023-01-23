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

	enum RasterizationState
	{
		Rasterization_default
	};

	class DrawItem
	{
	public:
		void draw();
		void setRasterizationState(RasterizationState state);

	private:
		RasterizationState rasterizationState = Rasterization_default;
		int id = 0;
		bool dirty = true;
		void build();
		DrawItem();
		~DrawItem();
	};

}