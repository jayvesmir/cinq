#pragma once
#include <DirectXMath.h>

#include "IndexedTriangleList.hpp"

namespace Geometry { 
    class Cube {
    public:
        template<class T>
        static IndexedTriangleList<T> generate() {
            constexpr float side = 1.f / 2.f;

            std::vector<DirectX::XMFLOAT3> vertices;
            vertices.emplace_back(-side, -side, -side);
            vertices.emplace_back( side, -side, -side);
            vertices.emplace_back(-side,  side, -side);
            vertices.emplace_back( side,  side, -side);
            vertices.emplace_back(-side, -side,  side);
            vertices.emplace_back( side, -side,  side);
            vertices.emplace_back(-side,  side,  side);
            vertices.emplace_back( side,  side,  side);

            std::vector<T> verts(vertices.size());
            for( size_t i = 0; i < vertices.size(); i++ ) {
                verts[i].pos = vertices[i];
            }

            return {
                std::move(verts),
                {
                    0,2,1, 2,3,1,
                    1,3,5, 3,7,5,
                    2,6,3, 3,6,7,
                    4,5,7, 4,7,6,
                    0,4,2, 2,4,6,
                    0,1,4, 1,5,4
                }
            };
        }
    };
}
