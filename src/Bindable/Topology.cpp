#include "Topology.hpp"

Topology::Topology(Pipeline& pipeline, D3D11_PRIMITIVE_TOPOLOGY type) 
    : type(type) {}

void Topology::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->IASetPrimitiveTopology(type);
}