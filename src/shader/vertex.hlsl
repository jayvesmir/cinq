struct VertexOut {
    float3 color : Color;
    float4 pos : SV_Position;
};

VertexOut main(float2 pos : Position, float3 color : Color) {
    VertexOut o;
    o.pos = float4(pos.x, pos.y, 0, 1);
    o.color = color;
    return o;
}