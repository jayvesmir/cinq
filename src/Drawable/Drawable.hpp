#pragma once
#include <memory>
#include <random>
#include <vector>
#include "../Pipeline.hpp"
#include "../Bindable/IndexBuffer.hpp"

class Bindable;

class Drawable {
public:
    Drawable() = default;
    Drawable( const Drawable& ) = delete;
    virtual ~Drawable() = default;

    virtual void update(float ts) = 0;
    void draw(Pipeline& pipeline) const;
    void bind(std::unique_ptr<Bindable> bind);
    void addIndexBuffer(std::unique_ptr<IndexBuffer> buffer);
    virtual DirectX::XMMATRIX getTransformMatrix() const = 0;
private:
    const IndexBuffer* indexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable>> binds;
};