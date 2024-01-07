#pragma once
#include "yjw_rhi_define.h"
#include "yjw_rhi_resource.h"

namespace rhi
{
	class RHI
	{
		virtual void init(InitConfig initConfig) = 0;
		virtual void createResource(const ResourceInitConfig& initConfig, Resource* resource) = 0;
		virtual void destoryResource(Resource* resource) = 0;
	};


	class PSO
	{
	public:
		template<typename T,typename ... Others>
		void bind(T&& t, Others&& ... args)
		{
			bind(std::forward(t));
			bind(std::forward(args ...));
		}
	};
}