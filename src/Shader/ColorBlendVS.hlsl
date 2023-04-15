#include "Types.hlsl"

ColorBlendVSOut main(float3 pos : Position, float4 color : Color) {
    ColorBlendVSOut o;
    o.pos = mul(float4(pos, 1), transform);
    o.color = color;
    return o;
}