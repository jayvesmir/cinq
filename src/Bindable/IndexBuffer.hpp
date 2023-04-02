#pragma once
#include "Bindable.hpp"

class IndexBuffer : public Bindable {
public:
    IndexBuffer(Pipeline& pipeline, const std::vector<uint16_t>& indices);
    
    void bind(Pipeline& pipeline) override;
    uint getCount() const;
protected:
    uint count;
    wrl::ComPtr<ID3D11Buffer> indexBuffer;
};