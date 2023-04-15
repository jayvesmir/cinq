#include "Types.hlsl"

TextureVSOut main(float3 pos : Position, float2 texCoord : TextureCoordinate) {
    TextureVSOut o;
    o.pos = mul(float4(pos, 1), transform);
    o.texCoord = texCoord;
    return o; 
}