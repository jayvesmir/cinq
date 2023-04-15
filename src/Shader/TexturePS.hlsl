Texture2D _texture;
SamplerState _sampler;

float4 main(float2 texCoord : TextureCoordinate) : SV_Target {
    return _texture.Sample(_sampler, texCoord);
}