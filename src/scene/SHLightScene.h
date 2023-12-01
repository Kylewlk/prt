//
// Created by Kyle on 2023/10/27.
//

#pragma once

#include "Scene.h"

class SHLightScene : public Scene
{
public:
    static constexpr const char* ID = "Spherical Harmonics Light";

    static SceneRef create();

    ~SHLightScene() override = default;
    SHLightScene(const SHLightScene&) = delete;
    SHLightScene& operator=(SHLightScene&) = delete;

private:
    enum DrawType
    {
        kHdrTexture,
        kTextureCubeMap,

        kCubeEnvironment,
        kSphereEnvironment,
        kSkyBoxEnvironment,
        kUnfoldEnvironment,
    };


    SHLightScene(int width, int height);

    void draw() override;
    void reset() override;
    void drawSettings() override;
    void onMouseEvent(const MouseEvent* e) override;

    Camera3DRef camera3d;
    Camera2DRef camera2d;
    ShaderRef shaderPicture;
    ShaderRef shader;

    ModelRef sphere;

    math::Vec3 lightColor{};
    math::Vec3 lightDir{};

    TextureRef roomHdr;
    TextureRef roomCubeMap;
    float roomShLight[16]{};

    TextureRef skyHdr;
    TextureRef skyCubeMap;
    TextureRef skyIrradiance;
    float skyShLight[16]{};

    TextureRef textureHdr;
    TextureRef textureCubeMap;
    float* shLight{};


    int drawType = kHdrTexture;

};

