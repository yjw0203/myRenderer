#include "Engine/Render/Private/Pass/SkyBoxPass.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "projectInfo.h"

namespace yjw
{
    void SkyBoxPass::Initialize()
    {
        float vertices[] = {
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
        };

        unsigned short indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 1, 1, 0, 4,
            7, 6, 5, 5, 4, 7,
            3, 2, 6, 6, 7, 3,
            4, 0, 3, 3, 7, 4,
            1, 5, 6, 6, 2, 1,
        };

        m_vertex_buffer = RPICreateGpuVertexBuffer(sizeof(vertices));
        RPIUpdateBuffer(m_vertex_buffer, vertices, 0, sizeof(vertices));

        m_index_buffer = RPICreateGpuIndexBuffer(sizeof(indices));
        RPIUpdateBuffer(m_index_buffer, indices, 0, sizeof(indices));

        m_vs = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(SkyBox.hlsl), "VSMain");
        m_ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(SkyBox.hlsl), "PSMain");

        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_depth_read);
        m_pipeline = RPICreateRenderPipeline(pipelineDesc);

        m_custom_ps_resource_set = RPICreateResourceSet(RPIResourceSetType::ps, m_ps->GetShaderReflect());

        m_primitive_binding = RPICreatePrimitiveBinding(m_vs);

        m_primitive_binding.SetVertexBuffer(RHIName("POSITION"), m_vertex_buffer);
        m_primitive_binding.AddIndexBuffer(m_index_buffer, 0, 36, true);
    }

    void SkyBoxPass::LoadResource(const char* sky_box_path)
    {
        if (m_sky_box)
        {
            m_sky_box.Release();
        }
        std::string str[6] = {
            std::string(sky_box_path) + "/px.jpg",
            std::string(sky_box_path) + "/nx.jpg",
            std::string(sky_box_path) + "/py.jpg",
            std::string(sky_box_path) + "/ny.jpg",
            std::string(sky_box_path) + "/pz.jpg",
            std::string(sky_box_path) + "/nz.jpg",
        };
        const char* files[6] = {
            str[0].c_str(),
            str[1].c_str(),
            str[2].c_str(),
            str[3].c_str(),
            str[4].c_str(),
            str[5].c_str(),
        };
        m_sky_box = RPICreateTextureCubeFromFile(files);
        m_custom_ps_resource_set.SetTexture(RHIName("skybox"), m_sky_box);
    }

    void SkyBoxPass::Destroy()
    {
        m_sky_box.Release();
        m_vertex_buffer.Release();
        m_vs->release();
        m_ps->release();
        m_primitive_binding.Release();
    }

    void SkyBoxPass::Submit(RPIContext context)
    {
        RPICmdPushEvent(context, "SkyBoxPass");
        RPICmdSetResourceSet(context, RPIResourceSetType::ps, m_custom_ps_resource_set);
        RPICmdSetPrimitiveBinding(context, m_primitive_binding, 0);
        RPICmdSetRenderPipeline(context, m_pipeline, m_primitive_binding, m_ps);
        RPICmdDrawIndex(context, 0, 1);
        RPICmdPopEvent(context);
    }
}