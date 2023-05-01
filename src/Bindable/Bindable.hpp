#pragma once
#include "Core/Pipeline.hpp"

#include <vector>
#include <string>

class Bindable {
public:
    virtual ~Bindable() = default;

    virtual void bind(Pipeline& pipeline) = 0;
protected:
    static ID3D11Device* getDevice(Pipeline& pipeline) { return pipeline.device.Get(); }
    static ID3D11DeviceContext* getDeviceContext(Pipeline& pipeline) { return pipeline.deviceContext.Get(); }
};