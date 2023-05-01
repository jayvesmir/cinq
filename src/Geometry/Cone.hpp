#pragma once
#include <DirectXMath.h>

#include "Core/Math.hpp"
#include "IndexedTriangleList.hpp"

namespace Geometry {
    class Cone {
    public:
        template<class T>
        static IndexedTriangleList<T> generateTesselated(int resolution) {
            assert(resolution >= 3);

            DirectX::XMVECTOR base = DirectX::XMVectorSet( 1.0f,0.0f,-1.0f,0.0f );
            float longitudeAngle = 2.0f * PI / resolution;

            // Base vertices
            std::vector<T> vertices;
            for(int i = 0; i < resolution; i++) {
                vertices.emplace_back();
                DirectX::XMVECTOR v = DirectX::XMVector3Transform( 
                    base,
                    DirectX::XMMatrixRotationZ(longitudeAngle * i)
                );
                DirectX::XMStoreFloat3( &vertices.back().pos,v );
            }

            // Center
            vertices.emplace_back();
            vertices.back().pos = {0.0f, 0.0f, -1.0f};
            const uint16_t iCenter = (uint16_t)(vertices.size() - 1);

            // Tip
            vertices.emplace_back();
            vertices.back().pos = {0.0f, 0.0f, 1.0f};
            const uint16_t iTip = (uint16_t)(vertices.size() - 1);

            // Base indices
            std::vector<uint16_t> indices;
            for(uint16_t i = 0; i < resolution; i++) {
                indices.push_back(iCenter);
                indices.push_back((i + 1) % resolution);
                indices.push_back(i);
            }

            // Cone indices
            for(uint16_t i = 0; i < resolution; i++) {
                indices.push_back(i);
                indices.push_back((i + 1) % resolution);
                indices.push_back(iTip);
            }

            return {std::move(vertices), std::move(indices)};
        }

        template<class T>
        static IndexedTriangleList<T> generate() {
            return generateTesselated<T>( 24 );
        }   
    };
}
