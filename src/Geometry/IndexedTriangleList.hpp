#pragma once
#include <vector>
#include <DirectXMath.h>

template<class T>
class IndexedTriangleList {
public:
    IndexedTriangleList() = default;
    IndexedTriangleList(std::vector<T> vertices, std::vector<uint16_t> indices)
        : vertices(std::move(vertices)), indices(std::move(indices)) {}

    void Transform(DirectX::FXMMATRIX matrix) {
        for(auto& v : vertices){
            const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
            DirectX::XMStoreFloat3(
                &v.pos,
                DirectX::XMVector3Transform(pos, matrix)
            );
        }
    }
public:
    std::vector<T> vertices;
    std::vector<uint16_t> indices;
};