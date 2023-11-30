//
// Created by Kyle on 2023/10/27.
//

#pragma once

#include "Scene.h"

class PolarPlotsScene : public Scene
{
public:
    static constexpr const char* ID = "Polar Plots";

    static SceneRef create();

    ~PolarPlotsScene() override = default;
    PolarPlotsScene(const PolarPlotsScene&) = delete;
    PolarPlotsScene& operator=(PolarPlotsScene&) = delete;

private:
    PolarPlotsScene(int width, int height);

    void draw() override;
    void reset() override;
    void drawSettings() override;

    void onMouseEvent(const MouseEvent* e) override;

    Camera3DRef camera;
    ShaderRef shader;

    ModelRef sphere;


    float lightIntensity{};
    math::Vec3 lightDir{};

    math::Vec3 positiveColor{};
    math::Vec3 negativeColor{};
};

