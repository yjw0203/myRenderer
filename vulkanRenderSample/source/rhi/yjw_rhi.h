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

	class BuildContext;
	class DrawItem
	{
	public:
		DrawItem();
		~DrawItem();
		void draw();
		void setRasterizationState(RasterizationState state);

	private:
		RasterizationState rasterizationState = Rasterization_default;
		int id = 0;
		bool dirty = true;
		void build();
		BuildContext* buildContext = nullptr;
	};

}