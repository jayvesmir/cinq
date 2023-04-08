#pragma once
#include "DrawableBase.hpp"

class Cube : public DrawableBase<Cube> {
public:
    Cube(Pipeline& pipeline,std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist );
    void update(float ts) override;
    DirectX::XMMATRIX getTransformMatrix() const override;
private:
    // Positional
    float roll = 0.f;
    float pitch = 0.f;
    float yaw = 0.f;
    float r, theta, phi, chi;

    // Speed (delta/s)
    float droll, dpitch, dyaw, dtheta, dphi, dchi;

    struct ColorCBuffer {
        struct {
            float r, g, b, a;
        } faceColors[6];
    };

    const std::vector<Vertex> vertices {
        {-1.f, -1.f, -1.f},
        { 1.f, -1.f, -1.f},
        {-1.f,  1.f, -1.f},
        {-1.f, -1.f,  1.f},
        { 1.f,  1.f, -1.f},
        { 1.f, -1.f,  1.f},
        {-1.f,  1.f,  1.f},
        { 1.f,  1.f,  1.f}
    };

    const std::vector<uint16_t> indices {
        0, 2, 1,   2, 4, 1,
        1, 4, 5,   4, 7, 5,
        2, 6, 4,   4, 6, 7,
        3, 5, 7,   3, 7, 6,
        0, 3, 2,   2, 3, 6,
        0, 1, 3,   1, 5, 3
    };

    const ColorCBuffer colorCBuffer { {
            {.2f, .2f, .2f},
            {.4f, .4f, .4f},
            {.5f, .5f, .5f},
            {.7f, .7f, .7f},
            {.8f, .8f, .8f},
            {.9f, .9f, .9f},
        }
    };

    const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
};