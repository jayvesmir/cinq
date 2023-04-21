#include "Camera.hpp"

DirectX::XMMATRIX Camera::getTransformMatrix() const {
    const DirectX::XMVECTOR pos = DirectX::XMVector3Transform(
        DirectX::XMVectorSet(0.f, 0.f, -r, 0.f),
        DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.f)
    );

    return (
        DirectX::XMMatrixLookAtLH(
            pos, DirectX::XMVectorZero(),
            DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f)
        ) * 
        DirectX::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll)
    );
}