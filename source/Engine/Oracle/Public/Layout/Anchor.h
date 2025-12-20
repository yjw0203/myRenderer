#pragma once
#include "Engine/Math/Public/Math.h"

enum class EAnchorPreset
{
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    Center,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight,

    StretchLeft,
    StretchTop,
    StretchRight,
    StretchBottom,
    StretchCenter,
    StretchWidth,
    StretchHeight,
    Stretch,

    LeftStretchRight,
    TopStretchBottom,
    HorizontalStretch,
    VerticalStretch,
    TopStretchCenter,
    BottomStretchCenter,

    Count
};

struct FAnchor
{
    union
    {
        struct
        {
            Vector2 minimum;
            Vector2 maximum;
        };

        struct
        {
            float left;
            float right;
            float bottom;
            float top;
        };
    };
    

    FAnchor()
        : minimum(0.0f, 0.0f)
        , maximum(0.0f, 0.0f)
    { }

    FAnchor(const Vector2& _minimum, const Vector2& _maximum)
        : minimum(_minimum)
        , maximum(_maximum)
    { }

    FAnchor(float minX, float minY, float maxX, float maxY)
        : minimum(minX, minY)
        , maximum(maxX, maxY)
    { }

    FAnchor(EAnchorPreset preset)
    { 
        switch (preset)
        {
        case EAnchorPreset::TopLeft:
            this->FAnchor::FAnchor(0.0f, 0.0f, 0.0f, 0.0f); break;
        case EAnchorPreset::TopCenter:
            this->FAnchor::FAnchor(0.5f, 0.0f, 0.5f, 0.0f); break;
        case EAnchorPreset::TopRight:
            this->FAnchor::FAnchor(1.0f, 0.0f, 1.0f, 0.0f); break;
        case EAnchorPreset::CenterLeft:
            this->FAnchor::FAnchor(0.0f, 0.5f, 0.0f, 0.5f); break;
        case EAnchorPreset::Center:
            this->FAnchor::FAnchor(0.5f, 0.5f, 0.5f, 0.5f); break;
        case EAnchorPreset::CenterRight:
            this->FAnchor::FAnchor(1.0f, 0.5f, 1.0f, 0.5f); break;
        case EAnchorPreset::BottomLeft:
            this->FAnchor::FAnchor(0.0f, 1.0f, 0.0f, 1.0f); break;
        case EAnchorPreset::BottomCenter:
            this->FAnchor::FAnchor(0.5f, 1.0f, 0.5f, 1.0f); break;
        case EAnchorPreset::BottomRight:
            this->FAnchor::FAnchor(1.0f, 1.0f, 1.0f, 1.0f); break;

        case EAnchorPreset::StretchLeft:
            this->FAnchor::FAnchor(0.0f, 0.0f, 0.0f, 1.0f); break;
        case EAnchorPreset::StretchTop:
            this->FAnchor::FAnchor(0.0f, 0.0f, 1.0f, 0.0f); break;
        case EAnchorPreset::StretchRight:
            this->FAnchor::FAnchor(1.0f, 0.0f, 1.0f, 1.0f); break;
        case EAnchorPreset::StretchBottom:
            this->FAnchor::FAnchor(0.0f, 1.0f, 1.0f, 1.0f); break;
        case EAnchorPreset::StretchCenter:
            this->FAnchor::FAnchor(0.0f, 0.5f, 1.0f, 0.5f); break;
        case EAnchorPreset::StretchWidth:
            this->FAnchor::FAnchor(0.0f, 0.5f, 1.0f, 0.5f); break;
        case EAnchorPreset::StretchHeight:
            this->FAnchor::FAnchor(0.5f, 0.0f, 0.5f, 1.0f); break;
        case EAnchorPreset::Stretch:
            this->FAnchor::FAnchor(0.0f, 0.0f, 1.0f, 1.0f); break;

        case EAnchorPreset::LeftStretchRight:
            this->FAnchor::FAnchor(0.0f, 0.0f, 1.0f, 0.0f); break;
        case EAnchorPreset::TopStretchBottom:
            this->FAnchor::FAnchor(0.0f, 0.0f, 0.0f, 1.0f); break;
        case EAnchorPreset::HorizontalStretch:
            this->FAnchor::FAnchor(0.0f, 0.5f, 1.0f, 0.5f); break;
        case EAnchorPreset::VerticalStretch:
            this->FAnchor::FAnchor(0.5f, 0.0f, 0.5f, 1.0f); break;

        default:
            this->FAnchor::FAnchor(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    Vector2 Minimum() { return minimum; }
    Vector2 Maximum() { return maximum; }
};