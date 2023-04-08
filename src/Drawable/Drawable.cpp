#include "Drawable.hpp"

void Drawable::draw(Pipeline& pipeline) const {
    for (auto& bind : binds) {
        bind->bind(pipeline);
    }

    for (auto& bind : getBindsStatic()) {
        bind->bind(pipeline);
    }
    pipeline.draw(indexBuffer->getCount());
}