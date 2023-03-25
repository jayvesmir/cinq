float4 main(float3 color : Color) : SV_Target {
    return float4(color.r, color.g, color.b, 1);
}