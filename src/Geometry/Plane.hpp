#pragma once
#include <vector>
#include <array>

#include "../Math.hpp"
#include "IndexedTriangleList.hpp"

namespace Geometry {
    class Plane {
    public:
        template<class T>
        static IndexedTriangleList<T> generateTesselated(int resolutionX, int resolutionY) {
            assert(resolutionX >= 1);
            assert(resolutionY >= 1);

            constexpr float width = 2.f;
            constexpr float height = 2.f;
            const int nVerticesX = resolutionX + 1;
            const int nVerticesY = resolutionY + 1;
            std::vector<T> vertices(nVerticesX * nVerticesY);

            {
                const float sideX = width / 2.0f;
                const float sideY = height / 2.0f;
                const float divisionSizeX = width / float( resolutionX );
                const float divisionSizeY = height / float( resolutionY );
                const DirectX::XMVECTOR bottomLeft = DirectX::XMVectorSet(-sideX, -sideY, 0.f, 0.f);

                for (int y = 0, i = 0; y < nVerticesY; y++) {
                    const float posY = float(y) * divisionSizeY;
                    for (int x = 0; x < nVerticesX; x++, i++)
                    {
                        const DirectX::XMVECTOR v = DirectX::XMVectorAdd( 
                            bottomLeft,
                            DirectX::XMVectorSet(float(x) * divisionSizeX, posY, 0.f, 0.f)
                        );

                        DirectX::XMStoreFloat3(&vertices[i].pos, v);
                    }
                }
            }

            std::vector<uint16_t> indices;
            indices.reserve((resolutionX * resolutionY)*(resolutionX * resolutionY) * 6);

            {
                const auto vxy2i = [nVerticesX]( size_t x,size_t y ) {
                    return (uint16_t)(y * nVerticesX + x);
                };

                for(size_t y = 0; y < resolutionY; y++) {
                    for(size_t x = 0; x < resolutionX; x++) {
                        const std::array<uint16_t,4> indexArray = {
                            vxy2i(  x  ,   y  ), 
                            vxy2i(x + 1,   y  ),
                            vxy2i(  x  , y + 1),
                            vxy2i(x + 1, y + 1)
                        };
                        indices.push_back( indexArray[0] );
                        indices.push_back( indexArray[2] );
                        indices.push_back( indexArray[1] );
                        indices.push_back( indexArray[1] );
                        indices.push_back( indexArray[2] );
                        indices.push_back( indexArray[3] );
                    }
                }
            }

            return { std::move(vertices), std::move(indices) };
        }

        template<class T>
        static IndexedTriangleList<T> generate() {
            return generateTesselated<T>(1, 1);
        }

        // TODO: Generalize
        template<class T>
        static IndexedTriangleList<T> generateTextured() {
            auto mesh = generateTesselated<T>(1, 1);

            mesh.vertices[0].texture = {0.f, 0.f};
            mesh.vertices[1].texture = {1.f, 0.f};
            mesh.vertices[2].texture = {0.f, 1.f};
            mesh.vertices[3].texture = {1.f, 1.f};

            return mesh;
        }
    };
}
