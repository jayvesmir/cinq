#pragma once

#include "Math.hpp"
#include "Image.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "Logger.hpp"
#include "Window/Imgui.hpp"
#include "Window/Window.hpp"
#include "Drawable/Drawables.hpp"

#include <ctime>
#include <memory>
#include <random>
#include <cstdlib>
#include <execution>

class Cinq {
public:
    Cinq();
    Cinq(int width, int height, const char* title);

    int run();
private:
    Imgui gui;
    Timer timer;
    Window window;
    Camera camera;
    PointLight light;
    
    const char* title;
    int width, height;
    float aspectRatio;
    std::vector<std::unique_ptr<Drawable>> drawables;
    const uint32_t drawableCount = 256;

    void update();

    class Factory {
    public:
        Factory(Pipeline& pipeline)
            : pipeline( pipeline ) {}

        std::unique_ptr<Drawable> operator()(){
            switch(typedist(rng)) {
            case 0:
            case 1:
                return std::make_unique<CubeSolid>(
                    pipeline, rng, adist,ddist,
                    odist, rdist, bdist
                );
            case 2:
                return std::make_unique<Plane>(
                    pipeline, rng, adist, ddist,
                    odist, rdist
                );
            default:
                assert(false && "Invalid drawable type");
                return {};
            }
        }
    private:
        Pipeline& pipeline;
        std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> adist { 0.f, PI * 2.f };
        std::uniform_real_distribution<float> ddist { 0.f, PI * .5f };
        std::uniform_real_distribution<float> odist { 0.f, PI * .08f };
        std::uniform_real_distribution<float> rdist { 6.f, 20.f };
        std::uniform_real_distribution<float> bdist { .4f, PI };
        std::uniform_int_distribution<int> xDist    { 5  , 20 };
        std::uniform_int_distribution<int> yDist    { 10 , 30 };
        std::uniform_int_distribution<int> typedist { 0  , 1 };
    };
};