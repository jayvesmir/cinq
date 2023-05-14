#pragma once
#include <vector>
#include <DirectXMath.h>

template<class T>
class IndexedTriangleList {
public:
    IndexedTriangleList() = default;
    IndexedTriangleList(std::vector<T> vertices, std::vector<uint16_t> indices)
        : vertices(std::move(vertices)), indices(std::move(indices)) {}

    // Chili
    void Transform(DirectX::FXMMATRIX matrix) {
        for(auto& v : vertices){
            const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
            DirectX::XMStoreFloat3(
                &v.pos,
                DirectX::XMVector3Transform(pos, matrix)
            );
        }
    }

    void SetNormalsIndependentFlat() {
        assert(indices.size() % 3 == 0 && indices.size() > 0);
        for (size_t i = 0; i < indices.size(); i += 3) {
            auto& v0 = vertices[indices[i]];
            auto& v1 = vertices[indices[i + 1]];
            auto& v2 = vertices[indices[i + 2]];
            const auto p0 = DirectX::XMLoadFloat3(&v0.pos);
            const auto p1 = DirectX::XMLoadFloat3(&v1.pos);
            const auto p2 = DirectX::XMLoadFloat3(&v2.pos);
            const auto n = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(p1, p0), DirectX::XMVectorSubtract(p2, p0)));

            DirectX::XMStoreFloat3(&v0.normal, n);
            DirectX::XMStoreFloat3(&v1.normal, n);
            DirectX::XMStoreFloat3(&v2.normal, n);
        }
    }
public:
    std::vector<T> vertices;
    std::vector<uint16_t> indices;
};