#pragma once
#include "Bindable.hpp"

class Topology : public Bindable {
public:
    Topology(Pipeline& pipeline, D3D11_PRIMITIVE_TOPOLOGY type);
    
    void bind(Pipeline& pipeline) override;
protected:
    D3D11_PRIMITIVE_TOPOLOGY type;
};