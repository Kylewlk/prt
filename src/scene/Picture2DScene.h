//
// Created by Kyle on 2023/10/26.
//

#pragma once

#include "Scene.h"


class Picture2DScene : public Scene
{
public:
    static constexpr const char* ID = "Picture 2D Scene";

    static SceneRef create();

    ~Picture2DScene() override = default;
    Picture2DScene(const Picture2DScene&) = delete;
    Picture2DScene& operator=(Picture2DScene&) = delete;

private:
    Picture2DScene(int width, int height);

    void reset() override;
    void draw() override;
    void drawSettings() override;

    void onMouseEvent(const MouseEvent* e) override;

    TextureRef texture;
    ShaderRef shader;
    Camera2DRef camera;

    math::Vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
    float rotation = 0.0f;
};


