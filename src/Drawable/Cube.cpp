#include "Cube.hpp"

void Cube::initialize(Pipeline& pipeline, const char* textureFP) {
    // First of type to be constructed
    if (!isInitialized()) {
        // Generate mesh
        auto model = Geometry::Cube::generateTextured<Vertex>();

        // Bind (constant) buffers
        bindStatic(std::make_unique<VertexBuffer>(pipeline, model.vertices));
        addIndexBufferStatic(std::make_unique<IndexBuffer>(pipeline, model.indices));

        // Bind texture
        bindStatic(std::make_unique<Sampler>(pipeline));

        // Create & bind shaders
        std::unique_ptr<PixelShader> pixelShader = std::make_unique<PixelShader>(pipeline, L"shader/TexturePS.cso");
        std::unique_ptr<VertexShader> vertexShader = std::make_unique<VertexShader>(pipeline, L"shader/TextureVS.cso");
        wrl::ComPtr<ID3DBlob> vsBytecode = vertexShader->getBytecode();
        bindStatic(std::move(vertexShader));
        bindStatic(std::move(pixelShader));

        // Bind input layout
        bindStatic(std::make_unique<InputLayout>(pipeline, inputLayout, vsBytecode.Get()));
        bindStatic(std::make_unique<Topology>(pipeline, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    } else {
        copyStaticIndexBuffer();
    }
    
    // Bind non-static (constant) buffers
    bind(std::make_unique<TransformCBuffer>(pipeline, *this));
    bindStatic(std::make_unique<Texture>(pipeline, Image::load(textureFP)));
}

Cube::Cube(Pipeline& pipeline, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist)
    : r(rdist(rng)), droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(adist(rng)) {
    initialize(pipeline, "res/cube.png");
    cartesian = false;
}

Cube::Cube(Pipeline& pipeline, DirectX::XMFLOAT3 position, const char* textureFP) : x(position.x), y(position.y), z(position.z) { initialize(pipeline, textureFP); }

void Cube::update(float ts) {
    roll += droll * ts;
    pitch += dpitch * ts;
    yaw += dyaw * ts;
    theta += dtheta * ts;
    phi += dphi * ts;
    chi += dchi * ts;
}

DirectX::XMMATRIX Cube::getTransformMatrix() const {
    if (!cartesian) {
        return (
            DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
            DirectX::XMMatrixTranslation(r, y, z) *
            DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi)
        );
    }
    return DirectX::XMMatrixTranslation(x, y, z);
}

/* Cube Normal */

void CubeSolid::initialize(Pipeline& pipeline) {
    // First of type to be constructed
    if (!isInitialized()) {
        // Generate mesh
        auto model = Geometry::Cube::generateIndependent<Vertex>();
        model.SetNormalsIndependentFlat();

        // Bind (constant) buffers
        bindStatic(std::make_unique<VertexBuffer>(pipeline, model.vertices));
        addIndexBufferStatic(std::make_unique<IndexBuffer>(pipeline, model.indices));

        // Create & bind shaders
        std::unique_ptr<PixelShader> pixelShader = std::make_unique<PixelShader>(pipeline, L"shader/PhongPS.cso");
        std::unique_ptr<VertexShader> vertexShader = std::make_unique<VertexShader>(pipeline, L"shader/PhongVS.cso");
        wrl::ComPtr<ID3DBlob> vsBytecode = vertexShader->getBytecode();
        bindStatic(std::move(vertexShader));
        bindStatic(std::move(pixelShader));

        // Bind input layout
        bindStatic(std::make_unique<InputLayout>(pipeline, inputLayout, vsBytecode.Get()));
        bindStatic(std::make_unique<Topology>(pipeline, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    } else {
        copyStaticIndexBuffer();
    }
    
    // Bind non-static (constant) buffers
    bind(std::make_unique<TransformCBuffer>(pipeline, *this));
}

CubeSolid::CubeSolid(Pipeline& pipeline, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist)
    : r(rdist(rng)), droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(adist(rng)) {
    initialize(pipeline);
    cartesian = false;
}

CubeSolid::CubeSolid(Pipeline& pipeline, DirectX::XMFLOAT3 position) : x(position.x), y(position.y), z(position.z) { initialize(pipeline); }

void CubeSolid::update(float ts) {
    roll += droll * ts;
    pitch += dpitch * ts;
    yaw += dyaw * ts;
    theta += dtheta * ts;
    phi += dphi * ts;
    chi += dchi * ts;
}

DirectX::XMMATRIX CubeSolid::getTransformMatrix() const {
    if (!cartesian) {
        return (
            DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
            DirectX::XMMatrixTranslation(r, y, z) *
            DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi)
        );
    }
    return DirectX::XMMatrixTranslation(x, y, z);
}