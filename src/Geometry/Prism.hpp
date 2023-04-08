#pragma once
#include <DirectXMath.h>

#include "Math.hpp"
#include "IndexedTriangleList.hpp"

namespace Geometry {
    class Prism {
    public:
        template<class T>
        static IndexedTriangleList<T> generateTesselated(int resolution) {
            assert(resolution >= 3);

            const auto base = DirectX::XMVectorSet(1.f, 0.f, -1.f, 0.f);
            const auto offset = DirectX::XMVectorSet(0.f, 0.f, 2.f, 0.f);
            const float longitudeAngle = 2.f * PI / resolution;

            // Near center
            std::vector<T> vertices;
            vertices.emplace_back();
            vertices.back().pos = { 0.f, 0.f, -1.f };
            const uint16_t iCenterNear = (uint16_t)(vertices.size() - 1);

            // Far center
            vertices.emplace_back();
            vertices.back().pos = { 0.f, 0.f, 1.f };
            const uint16_t iCenterFar = (uint16_t)(vertices.size() - 1);

            // Base vertices
            for(int iLong = 0; iLong < resolution; iLong++) {
                // Near base
                {
                    vertices.emplace_back();
                    DirectX::XMVECTOR v = DirectX::XMVector3Transform(
                        base,
                        DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
                    );
                    DirectX::XMStoreFloat3(&vertices.back().pos, v);
                }
                // Far base
                {
                    vertices.emplace_back();
                    DirectX::XMVECTOR v = DirectX::XMVector3Transform(
                        base,
                        DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
                    );
                    v = DirectX::XMVectorAdd( v,offset );
                    DirectX::XMStoreFloat3(&vertices.back().pos, v);
                }
            }

            // Side indices
            std::vector<uint16_t> indices;
            for(uint16_t iLong = 0; iLong < resolution; iLong++) {
                const int i = iLong * 2;
                const int mod = resolution * 2;
                indices.push_back(i + 2);
                indices.push_back((i + 2) % mod + 2);
                indices.push_back(i + 1 + 2);
                indices.push_back((i + 2) % mod + 2);
                indices.push_back((i + 3) % mod + 2);
                indices.push_back(i + 1 + 2);
            }

            // Base indices
            for(uint16_t iLong = 0; iLong < resolution; iLong++) {
                const int i = iLong * 2;
                const int mod = resolution * 2;
                indices.push_back(i + 2);
                indices.push_back(iCenterNear);
                indices.push_back((i + 2) % mod + 2);
                indices.push_back(iCenterFar);
                indices.push_back(i + 1 + 2);
                indices.push_back((i + 3) % mod + 2);
            }

            return { std::move(vertices), std::move(indices) };
        }
        
        template<class T>
        static IndexedTriangleList<T> generate() {
            return generateTesselated<T>( 24 );
        }
    };
    }
