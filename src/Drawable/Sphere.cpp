#include "Sphere.hpp"
#include "Geometry/Sphere.hpp"
#include "Bindable/Bindables.hpp"

Sphere::Sphere(Pipeline& pipeline, float radius) {
    // First of type to be constructed
    if (!isInitialized()) {
        // Generate mesh
        auto model = Geometry::Sphere::generate<Vertex>();
        model.Transform(DirectX::XMMatrixScaling(radius, radius, radius));

        struct PSColorConstants {
            DirectX::XMFLOAT3 color = {1.0f, 1.0f, 1.0f};
            float padding;
        } colorConst;

        // Bind (constant) buffers
        bindStatic(std::make_unique<VertexBuffer>(pipeline, model.vertices));
        addIndexBufferStatic(std::make_unique<IndexBuffer>(pipeline, model.indices));
        bindStatic(std::make_unique<PixelCBuffer<PSColorConstants>>(pipeline, colorConst));

        // Create & bind shaders
        std::unique_ptr<PixelShader> pixelShader = std::make_unique<PixelShader>(pipeline, L"shader/SolidPS.cso");
        std::unique_ptr<VertexShader> vertexShader = std::make_unique<VertexShader>(pipeline, L"shader/SolidVS.cso");
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

void Sphere::update( float dt ) {}

void Sphere::setPosition(DirectX::XMFLOAT3 pos) {
    this->pos = pos;
}

DirectX::XMMATRIX Sphere::getTransformMatrix() const {
    return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}