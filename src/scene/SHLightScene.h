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
        kHdrCubMap,

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
    TextureRef createCubMap(const TextureRef& hdr);
    void createShLight(const TextureRef& hdr, float (&shLight)[16]);

    void drawHdrCubeMap();
    void drawTextureCubeMap();

    Camera3DRef camera3d;
    Camera2DRef camera2d;
    ShaderRef shaderPicture;
    ShaderRef shaderHdrToCubeMap;
    ShaderRef shaderCubMap;
    ShaderRef shader;

    ModelRef sphere;
    ModelRef cube;

    math::Vec3 lightColor{};
    math::Vec3 lightDir{};

    TextureRef roomHdr;
    TextureRef roomCubeMap;
    float roomShLight[16]{};

    float (&r)[16] = roomShLight;

    TextureRef skyHdr;
    TextureRef skyCubeMap;
    TextureRef skyIrradiance;
    float skyShLight[16]{};

    TextureRef textureHdr;
    TextureRef textureCubeMap;
    float* shLight{};

    int hdrType = 0;
    int drawType = kHdrTexture;

};

