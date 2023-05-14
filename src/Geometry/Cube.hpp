#pragma once
#include <DirectXMath.h>

#include "IndexedTriangleList.hpp"

namespace Geometry { 
    class Cube {
    public:
        template<class T>
        static IndexedTriangleList<T> generate() {
            constexpr float side = 1.f/2.f;

            std::vector<T> vertices(8);
            vertices[0].pos = { -side, -side, -side };
            vertices[1].pos = {  side, -side, -side };
            vertices[2].pos = { -side,  side, -side };
            vertices[3].pos = {  side,  side, -side };
            vertices[4].pos = { -side, -side,  side };
            vertices[5].pos = {  side, -side,  side };
            vertices[6].pos = { -side,  side,  side };
            vertices[7].pos = {  side,  side,  side };

            return {
                std::move(vertices),
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

        template<class T>
        static IndexedTriangleList<T> generateTextured() {
            constexpr float side = 1.f/2.f;

            std::vector<T> vertices(14);
            vertices[0].pos      = { -side, -side, -side };
            vertices[1].pos      = {  side, -side, -side };
            vertices[2].pos      = { -side,  side, -side };
            vertices[3].pos      = {  side,  side, -side };
            vertices[4].pos      = { -side, -side,  side };
            vertices[5].pos      = {  side, -side,  side };
            vertices[6].pos      = { -side,  side,  side };
            vertices[7].pos      = {  side,  side,  side };
            vertices[8].pos      = { -side, -side, -side };
            vertices[9].pos      = {  side, -side, -side };
            vertices[10].pos     = { -side, -side, -side };
            vertices[11].pos     = { -side, -side,  side };
            vertices[12].pos     = {  side, -side, -side };
            vertices[13].pos     = {  side, -side,  side };
            vertices[0].texture  = { 2.f/3.f, 0.f/4.f };
            vertices[1].texture  = { 1.f/3.f, 0.f/4.f };
            vertices[2].texture  = { 2.f/3.f, 1.f/4.f };
            vertices[3].texture  = { 1.f/3.f, 1.f/4.f };
            vertices[4].texture  = { 2.f/3.f, 3.f/4.f };
            vertices[5].texture  = { 1.f/3.f, 3.f/4.f };
            vertices[6].texture  = { 2.f/3.f, 2.f/4.f };
            vertices[7].texture  = { 1.f/3.f, 2.f/4.f };
            vertices[8].texture  = { 2.f/3.f, 4.f/4.f };
            vertices[9].texture  = { 1.f/3.f, 4.f/4.f };
            vertices[10].texture = { 3.f/3.f, 1.f/4.f };
            vertices[11].texture = { 3.f/3.f, 2.f/4.f };
            vertices[12].texture = { 0.f/3.f, 1.f/4.f };
            vertices[13].texture = { 0.f/3.f, 2.f/4.f };

            return {
                std::move(vertices),
                {
                    0, 2, 1,  2, 3, 1,
                    4, 8, 5,  5, 8, 9,
                    2, 6, 3,  3, 6, 7,
                    4, 5, 7,  4, 7, 6,
                    2,10,11,  2,11, 6,
                    12,3, 7,  12,7,13
                }
            };
        }

        template<class T>
        static IndexedTriangleList<T> generateIndependent() {
            constexpr float side = 1.f/2.f;

            std::vector<T> vertices(24);
            vertices[0].pos  = { -side,-side,-side }; // Near side
            vertices[1].pos  = { side,-side,-side  };
            vertices[2].pos  = { -side,side,-side  };
            vertices[3].pos  = { side,side,-side   };
            vertices[4].pos  = { -side,-side,side  }; // Far side
            vertices[5].pos  = { side,-side,side   };
            vertices[6].pos  = { -side,side,side   };
            vertices[7].pos  = { side,side,side    };
            vertices[8].pos  = { -side,-side,-side }; // Left side
            vertices[9].pos  = { -side,side,-side  };
            vertices[10].pos = { -side,-side,side  };
            vertices[11].pos = { -side,side,side   };
            vertices[12].pos = { side,-side,-side  }; // Right side
            vertices[13].pos = { side,side,-side   };
            vertices[14].pos = { side,-side,side   };
            vertices[15].pos = { side,side,side    };
            vertices[16].pos = { -side,-side,-side }; // Bottom side
            vertices[17].pos = { side,-side,-side  };
            vertices[18].pos = { -side,-side,side  };
            vertices[19].pos = { side,-side,side   };
            vertices[20].pos = { -side,side,-side  }; // Top side
            vertices[21].pos = { side,side,-side   };
            vertices[22].pos = { -side,side,side   };
            vertices[23].pos = { side,side,side    };

            return{
                std::move(vertices),
                {
                    0,  2, 1, 2,  3, 1,
                    4,  5, 7, 4,  7, 6,
                    8, 10, 9, 10,11, 9,
                    12,13,15, 12,15,14,
                    16,17,18, 18,17,19,
                    20,23,21, 20,22,23
                }
            };
        }
    };
}
