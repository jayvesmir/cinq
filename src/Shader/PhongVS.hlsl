#include "Types.hlsl"

PhongVSOut main(float3 pos : Position, float3 normal : Normal) {
    PhongVSOut o;
    o.worldPos = (float3)mul(float4(pos, 1.f), model);
    o.normal = mul(normal, (float3x3)model);
    o.pos = mul(float4(pos, 1.f), modelViewProjection);
    return o;
}