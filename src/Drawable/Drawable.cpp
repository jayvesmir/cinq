#include "Drawable.hpp"

void Drawable::draw(Pipeline& pipeline) const {
    for (auto& bind : binds) {
        bind->bind(pipeline);
    }
    pipeline.draw(indexBuffer->getCount());
}

void Drawable::bind(std::unique_ptr<Bindable> bind) {
    binds.push_back(std::move(bind)); 
}

void Drawable::addIndexBuffer(std::unique_ptr<IndexBuffer> buffer) {
    indexBuffer = buffer.get();
    binds.push_back(std::move(buffer));
}