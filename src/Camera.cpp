#include "Camera.hpp"

DirectX::XMMATRIX Camera::getTransformMatrix() const {
    const DirectX::XMVECTOR pos = DirectX::XMVector3Transform(
        DirectX::XMVectorSet(0.f, 0.f, -r, 0.f),
        DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.f)
    );

    // Flip the camera upside down if going down from over the poles.
    // Hard to explain, remove this code and try going over one of the poles
    // you'll see why it's here
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
    if (std::abs(phi) >= deg2rad(90.f))
        up = DirectX::XMVectorSet(0.f, -1.f, 0.f, 0.f);

    return (
        DirectX::XMMatrixLookAtLH(
            pos, DirectX::XMVectorZero(), up
        ) * 
        DirectX::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll)
    );
}

void Camera::reset() {
    roll  = 0.f;
    pitch = 0.f;
    yaw   = 0.f;
    r     = 5.f;
    theta = 0.f;
    phi   = 0.f;
    chi   = 0.f;
}