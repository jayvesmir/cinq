#include "Types.hlsl"

static const float3 materialColor        = {.7f, .7f, 1.f};
static const float3 ambientColor         = {.0f, .0f, .0f};
static const float3 diffuseColor         = {1.f, 1.f, 1.f};
static const float  diffuseIntesity      = 1.f;
static const float  attenuationConst     = 1.f;
static const float  attenuationLinear    = .045f;
static const float  attenuationQuadratic = 0.0075f;

float4 main(PhongVSOut input) : SV_Target {
    float3 v = lightPos - input.worldPos;
    float3 distance = length(v);
    float3 direction = v / distance;

    float attenuation = 1.f / (attenuationConst + attenuationLinear * distance + attenuationQuadratic * (distance * distance));
    float3 diffuse = diffuseColor * diffuseIntesity * attenuation * max(0.f, dot(direction, input.normal));

    return float4(saturate(diffuse + ambientColor), 1.f);
}