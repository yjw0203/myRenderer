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
		Rasterization_default,
		Rasterization_count
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

		friend class BuildContext;
	};

	class ImageContext;
	class Image
	{
	public:
		Image(int width,int height);
		~Image();
	private:
		ImageContext* imageContext = nullptr;
		int width;
		int height;

		friend class ImageContext;
	};
}