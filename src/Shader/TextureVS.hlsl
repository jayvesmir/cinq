cbuffer transformCBuffer {
    matrix transform;
};

struct VSOut {
    float4 pos : SV_Position;
    float2 texCoord : TextureCoordinate;
};

VSOut main(float3 pos : Position, float2 texCoord : TextureCoordinate) {
    VSOut o;
    o.pos = mul(float4(pos, 1), transform);
    o.texCoord = texCoord;
    return o; 
}