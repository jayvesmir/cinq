#pragma once
#include "Core/Pipeline.hpp"
#include "Bindable/IndexBuffer.hpp"

#include <memory>
#include <random>
#include <vector>

class Bindable;

class Drawable {
    template<class T>
    friend class DrawableBase;
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;
    virtual ~Drawable() = default;

    virtual void update(float ts) = 0;
    void draw(Pipeline& pipeline) const;

    void bind(std::unique_ptr<Bindable> bind) {
        binds.push_back(std::move(bind));
    }

    void addIndexBuffer(std::unique_ptr<IndexBuffer> buffer) {
        indexBuffer = buffer.get();
        binds.push_back(std::move(buffer));
    }

    virtual DirectX::XMMATRIX getTransformMatrix() const = 0;
private:
    virtual const std::vector<std::unique_ptr<Bindable>>& getBindsStatic() const = 0;

    const IndexBuffer* indexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable>> binds;
};