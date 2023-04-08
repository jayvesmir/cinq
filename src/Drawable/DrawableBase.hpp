#pragma once

#include "Drawable.hpp"
#include "Bindable/IndexBuffer.hpp"

template<class T>
class DrawableBase : public Drawable {
public:
    bool isInitialized() const { return !bindsStatic.empty(); }
    void bindStatic(std::unique_ptr<Bindable> bind) {
        bindsStatic.push_back(std::move(bind));
    }

    void addIndexBufferStatic(std::unique_ptr<IndexBuffer> buffer) {
        indexBuffer = buffer.get();
        bindsStatic.push_back(std::move(buffer));
    }

    void copyStaticIndexBuffer() {
        for (const auto& bind : bindsStatic) {
            if (const IndexBuffer* buffer = dynamic_cast<IndexBuffer*>(bind.get())) {
                indexBuffer = buffer;
                return;
            }
        }
    }
private:
    const std::vector<std::unique_ptr<Bindable>>& getBindsStatic() const { return bindsStatic; }
    static std::vector<std::unique_ptr<Bindable>> bindsStatic;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::bindsStatic;