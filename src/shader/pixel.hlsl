cbuffer colorCBuffer {
    float4 faceColors[6];
}

float4 main(uint id : SV_PrimitiveID) : SV_Target {
    return faceColors[id / 2];
}