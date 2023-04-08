#include "Cube.hpp"

Cube::Cube(Pipeline& pipeline, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist)
    : r(rdist(rng)), droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(adist(rng)) {
    // First of type to be constructed
    if (!isInitialized()) {
        // Generate mesh
        auto model = Geometry::Cube::generate<Vertex>();

        // Bind (constant) buffers
        bindStatic(std::make_unique<VertexBuffer>(pipeline, model.vertices));
        addIndexBufferStatic(std::make_unique<IndexBuffer>(pipeline, model.indices));
        bindStatic(std::make_unique<PixelCBuffer<ColorCBuffer>>(pipeline, colorCBuffer));

        // Create & bind shaders
        std::unique_ptr<PixelShader> pixelShader = std::make_unique<PixelShader>(pipeline, L"shader/ColorIndexPS.cso");
        std::unique_ptr<VertexShader> vertexShader = std::make_unique<VertexShader>(pipeline, L"shader/ColorIndexVS.cso");
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

    // Set individual random scaling
    DirectX::XMStoreFloat3x3(
        &transformMatrix,
        DirectX::XMMatrixScaling(1.f, 1.f, bdist(rng))
    );
}

void Cube::update(float ts) {
    roll += droll * ts;
    pitch += dpitch * ts;
    yaw += dyaw * ts;
    theta += dtheta * ts;
    phi += dphi * ts;
    chi += dchi * ts;
}

DirectX::XMMATRIX Cube::getTransformMatrix() const {
    return (
        DirectX::XMLoadFloat3x3(&transformMatrix) *
        DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
        DirectX::XMMatrixTranslation(r, 0.f, 0.f) *
        DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
        DirectX::XMMatrixTranslation(0.f, 0.f, 20.f)
    );
}