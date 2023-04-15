#include "Types.hlsl"

float4 main(uint id : SV_PrimitiveID) : SV_Target {
    return faceColors[(id / 2) % 8];
}