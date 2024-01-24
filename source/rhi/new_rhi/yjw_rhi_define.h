#pragma once
namespace rhi
{
	struct InitConfig
	{
		void* window;
		int width;
		int height;
		bool debug_mode;
	};

    enum class ResourceType : char
    {
        unkown,
        buffer,
        texture1D,
        texture2D,
        texture3D,
        textureCube,
        texture2DArray
    };

    enum class Format : char
    {
        unknow = 0,
        R8G8B8A8_unorm,
        R8G8B8A8_snorm,
        R8G8B8A8_srgb,
        R8G8B8A8_sfloat,
        R32G32B32_sfloat,
        R32G32B32A32_sfloat,
        R32G32_sfloat,
        D24_unorm_S8_uint
    };
    enum class ResourceUsageBits : int
    {
        none = 0,
        allow_render_target = 0x1,
        allow_depth_stencil = 0x2,
        allow_unordered_access = 0x4,
        deny_shader_resource = 0x8,
        allow_transfer_src = 0x10,
        allow_transfer_dst = 0x20,
        allow_vertex_buffer = 0x40,//only for buffer
        allow_index_buffer = 0x80,//only for buffer
        allow_indirect_buffer = 0x100 //only for buffer
    };
    typedef int ResourceUsage;

    enum class MemoryType : char
    {
        default_ = 1,
        upload = 2,
        readback = 3,
    };

    enum class ResourceState : char
    {
        undefine,
        render_target,
        depth_stencil_write,
        depth_stencil_read,
        shader_resource_read,
        transfer_src,
        transfer_dst,
        present_src
    };

    enum class PolygonMode
    {
        fill,
        line,
        point
    };

    enum class CullMode
    {
        none,
        back,
        front,
        front_and_back
    };

    enum class FrontFace
    {
        counter_clockwise,
        clockwise
    };

    enum class ShaderType
    {
        vertex_shader,
        pixel_shader,
        compute_shader
    };

    enum class DescriptorType
    {
        uniform_buffer,
        shader_resource_texture
    };
}