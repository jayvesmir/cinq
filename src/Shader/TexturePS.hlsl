#include "Types.hlsl"

Texture2D _texture;
SamplerState _sampler;

float4 main(TextureVSOut input) : SV_Target {
    return _texture.Sample(_sampler, input.texCoord);
}