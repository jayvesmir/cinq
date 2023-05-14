cbuffer transformCBuffer {
    matrix transform;
};

cbuffer lightCBuffer {
    float3 lightPos;
};

cbuffer ModelCBuffer {
    matrix model;
    matrix modelViewProjection;
};

cbuffer colorCBuffer {
    float4 faceColors[8];
};

cbuffer SolidCBuffer{
    float4 color;
};

struct TextureVSOut {
    float4 pos : SV_Position;
    float2 texCoord : TextureCoordinate;
};

struct ColorBlendVSOut {
    float4 color : Color;
    float4 pos : SV_Position;
};

struct PhongVSOut {
    float3 normal : Normal;
    float3 worldPos : Position;
    float4 pos : SV_Position;
};