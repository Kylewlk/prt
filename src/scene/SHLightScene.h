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

    ~SHLightScene() override;
    SHLightScene(const SHLightScene&) = delete;
    SHLightScene& operator=(SHLightScene&) = delete;

private:
    enum DrawType
    {
        kHdrTexture,
        kHdrCubMap,

        kTextureCubeMap,

        kSHLight,
        kTextureShLightSphere,
        kTextureShLightCube,
    };


    SHLightScene(int width, int height);

    void draw() override;
    void reset() override;
    void drawSettings() override;
    void onMouseEvent(const MouseEvent* e) override;
    TextureRef createCubMap(const TextureRef& hdr);
    GLuint createShLight(const TextureRef& hdr);

    void drawHdrCubeMap();
    void drawTextureCubeMap();
    void drawShLight();

    Camera3DRef camera3d;
    Camera2DRef camera2d;
    ShaderRef shaderPicture;
    ShaderRef shaderHdrToCubeMap;
    ShaderRef shaderCubMap;
    ShaderRef shaderCreateSHLight;
    ShaderRef shaderShowSHLight;

    ModelRef sphere;
    ModelRef cube;

    math::Vec3 lightColor{};
    math::Vec3 lightDir{};

    TextureRef roomHdr;
    TextureRef roomCubeMap;
    GLuint roomShLight{};

    TextureRef skyHdr;
    TextureRef skyCubeMap;
    TextureRef skyIrradiance;
    GLuint skyShLight;

    TextureRef textureHdr;
    TextureRef textureCubeMap;
    GLuint shLight{};

    int hdrType = 0;
    int drawType = kHdrTexture;

};

