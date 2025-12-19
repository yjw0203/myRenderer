#pragma once
#include "Engine/Oracle/Public/Render/OracleStyle.h"
#include "Engine/Oracle/Public/Layout/OracleLayout.h"
#include "Engine/Oracle/Public/Utils/Utils.h"

struct OracleGlobalDrawInfo
{
    Vector2 m_resolution{ 1000,1000 };
};

struct OracleRenderTransform
{
    Vector2 local_position{};
    Matrix2x2 transform{};
};

class OcacleDrawElementList;
class OracleDrawElement
{
public:
    OracleRenderTransform m_transform;
    void SetLayout(const OracleLayout* Layout, const OracleGlobalDrawInfo& draw_info);

public:
    static void MakeBox(OcacleDrawElementList& ElementList, const OracleStyle& Style, const OracleLayout& Layout);
};

class OcacleDrawElementList
{
public:
    void SetResolution(Vector2 resolution) { m_global_info.m_resolution = resolution; };
    const OracleGlobalDrawInfo& GetDrawInfo() { return m_global_info; }

    template<typename DrawElementType>
    DrawElementType& AddUninitialized()
    {
        // todo: cache map
        m_draw_elements.push_back(new DrawElementType());
        return *(DrawElementType*)m_draw_elements.back();
    }
    OcArray<OracleDrawElement*>& GetArray() { return m_draw_elements; }
private:
    OcArray<OracleDrawElement*> m_draw_elements;

    OracleGlobalDrawInfo m_global_info{};
};

class OracleBackgroundColourAttribute
{
public:
    LinearColour m_background_colour;

    void SetStyle(const OracleStyle* Style)
    {
        m_background_colour = Style->m_background_color;
    }
};

class OracleImageAttribute
{
public:
    OracleRenderResource m_image;

    void SetStyle(const OracleStyle* Style)
    {
        m_image = Style->m_resource;
    }
};