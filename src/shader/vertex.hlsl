struct VertexOut {
    float3 color : Color;
    float4 pos : SV_Position;
};

cbuffer constantBuffer {
    matrix transform;
};

VertexOut main(float2 pos : Position, float3 color : Color) {
    VertexOut o;
    o.pos = mul(float4(pos.x, pos.y, 0, 1), transform);
    o.color = color;
    return o;
}