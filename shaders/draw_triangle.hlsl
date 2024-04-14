struct VertexOut  
{  
    float4 position : SV_POSITION;  
};  

VertexOut VSMain(uint vertexID : SV_VertexID)  
{  
    float4 triangleVertices[3] = {  
    float4(-0.5f, -0.5f, 0.0f, 1.0f),
    float4( 0.5f, -0.5f, 0.0f, 1.0f),
    float4( 0.0f,  0.5f, 0.0f, 1.0f)
};  
    VertexOut output;  
    output.position = triangleVertices[vertexID];  
    return output;  
}

struct PixelOut  
{  
    float4 color : SV_Target0;  
};
  
PixelOut PSMain(VertexOut input)  
{  
    PixelOut output;  
    output.color = float4(1,0,0,1);  
    return output;  
}