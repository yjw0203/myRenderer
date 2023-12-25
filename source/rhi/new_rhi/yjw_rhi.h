#pragma once
#include "yjw_rhi_define.h"

namespace rhi
{
	void init(InitConfig initConfig);

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