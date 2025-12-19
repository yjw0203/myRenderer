#pragma once
#include "Engine/Math/Public/Math.h"

struct LinearColour
{
    union
    {
        struct
        {
            float R, G, B, A;
        };

        float RGBA[4];
    };

    LinearColour()
        : R(0.0f)
        , G(0.0f)
        , B(0.0f)
        , A(0.0f)
    {}

    LinearColour(float r, float g, float b, float a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }

    LinearColour(Vector4 rgba)
    {
        R = rgba[0];
        G = rgba[1];
        B = rgba[2];
        A = rgba[3];
    }
};
