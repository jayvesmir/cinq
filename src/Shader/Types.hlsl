cbuffer transformCBuffer {
    matrix transform;
};

cbuffer colorCBuffer {
    float4 faceColors[8];
};

struct TextureVSOut {
    float4 pos : SV_Position;
    float2 texCoord : TextureCoordinate;
};

struct ColorBlendVSOut {
    float4 color : Color;
    float4 pos : SV_Position;
};