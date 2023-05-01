#pragma once
#include <DirectXMath.h>

#include "Core/Math.hpp"
#include "IndexedTriangleList.hpp"

namespace Geometry {
    class Sphere {
    public:
        template<class T>
        static IndexedTriangleList<T> generateTesselated(int resolutionX, int resolutionY) {
            assert(resolutionX >= 3);
            assert(resolutionY >= 3);

            const float radius = 1.f;
            const DirectX::XMVECTOR base = DirectX::XMVectorSet(0.f, 0.f, radius, 0.f);
            const float latitudeAngle = PI / resolutionX;
            const float longitudeAngle = 2.0f * PI / resolutionY;

            std::vector<T> vertices;
            for (int iLat = 1; iLat < resolutionX; iLat++) {
                const DirectX::XMVECTOR latBase = DirectX::XMVector3Transform( 
                    base,
                    DirectX::XMMatrixRotationX(latitudeAngle * iLat) 
                );

                for (int iLong = 0; iLong < resolutionY; iLong++) {
                    vertices.emplace_back();
                    DirectX::XMVECTOR v = DirectX::XMVector3Transform( 
                        latBase,
                        DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
                    );

                    DirectX::XMStoreFloat3(&vertices.back().pos, v);
                }
            }

            // Add the cap vertices
            const uint16_t iNorthPole = (uint16_t)vertices.size();
            vertices.emplace_back();
            DirectX::XMStoreFloat3(&vertices.back().pos, base);

            const uint16_t iSouthPole = (uint16_t)vertices.size();
            vertices.emplace_back();
            DirectX::XMStoreFloat3(&vertices.back().pos, DirectX::XMVectorNegate(base));

            const auto calcIDirectX = [resolutionX, resolutionY] (uint16_t iLat, uint16_t iLong) { 
                return iLat * resolutionY + iLong; 
            };
            std::vector<uint16_t> indices;
            for(uint16_t iLat = 0; iLat < resolutionX - 2; iLat++) {
                for(uint16_t iLong = 0; iLong < resolutionY - 1; iLong++) {
                    indices.push_back(calcIDirectX(  iLat  ,   iLong  ));
                    indices.push_back(calcIDirectX(iLat + 1,   iLong  ));
                    indices.push_back(calcIDirectX(  iLat  , iLong + 1));
                    indices.push_back(calcIDirectX(  iLat  , iLong + 1));
                    indices.push_back(calcIDirectX(iLat + 1,   iLong  ));
                    indices.push_back(calcIDirectX(iLat + 1, iLong + 1));
                }

                // Wrap band
                indices.push_back(calcIDirectX(  iLat  , resolutionY - 1 ));
                indices.push_back(calcIDirectX(iLat + 1, resolutionY - 1 ));
                indices.push_back(calcIDirectX(  iLat  ,         0        ));
                indices.push_back(calcIDirectX(  iLat  ,         0        ));
                indices.push_back(calcIDirectX(iLat + 1, resolutionY - 1 ));
                indices.push_back(calcIDirectX(iLat + 1,         0        ));         
            }

            // Cap fans
            for( uint16_t iLong = 0; iLong < resolutionY - 1; iLong++ )
            {
                // North
                indices.push_back(iNorthPole);
                indices.push_back(calcIDirectX(0, iLong));
                indices.push_back(calcIDirectX(0, iLong + 1));

                // South
                indices.push_back(calcIDirectX(resolutionX - 2, iLong + 1));
                indices.push_back(calcIDirectX(resolutionX - 2, iLong));
                indices.push_back(iSouthPole );
            }

            // Wrap triangles
            // North
            indices.push_back(iNorthPole);
            indices.push_back(calcIDirectX(0,resolutionY - 1));
            indices.push_back(calcIDirectX(0,0 ) );

            // South
            indices.push_back(calcIDirectX(resolutionX - 2, 0));
            indices.push_back(calcIDirectX(resolutionX - 2, resolutionY - 1));
            indices.push_back(iSouthPole);

            return { std::move( vertices ),std::move( indices ) };
        }
        template<class T>
        static IndexedTriangleList<T> generate() {
            return generateTesselated<T>( 12,24 );
        }
    };
}

