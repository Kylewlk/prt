//
// Created by Kyle on 2023/10/27.
//

#pragma once

#include "Scene.h"

class Model3DScene : public Scene
{
public:
    static constexpr const char* ID = "Model 3D Scene";

    static SceneRef create();

    ~Model3DScene() override = default;
    Model3DScene(const Model3DScene&) = delete;
    Model3DScene& operator=(Model3DScene&) = delete;

private:
    Model3DScene(int width, int height);

    void draw() override;
    void reset() override;
    void drawSettings() override;

    void onMouseEvent(const MouseEvent* e) override;

    Camera3DRef camera;
    ShaderRef shader;

    ModelRef sphere;
    ModelRef cube;
    ModelRef plane;

    math::Vec3 lightColor{};
    math::Vec3 lightDir{};

    math::Vec3 sphereColor{};
    math::Vec3 cubeColor{};
    math::Vec3 planeColor{};
};

