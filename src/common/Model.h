//
// Created by Yun on 2022/6/25.
//

#pragma once
#include "Define.h"
#include "MathHelp.h"
#include "Utils.h"

class Model
{
public:
    enum AttributePointer
    {
        kPosition = 0, // vec3
        kNormal = 1,  // vec3
        kTexture = 2, // vec2
        kTangent = 3, // vec3
    };

    struct Vertex
    {
        math::Vec3 position{0, 0, 0};
        math::Vec3 normal{ 0, 1, 0};
        math::Vec2 texCoord{0, 0};
//        math::Vec3 tangent;
        Vertex() = default;
        explicit Vertex(const math::Vec3& pos, const math::Vec3& n = {0, 1, 0}, const math::Vec2& t = {0, 0})
            : position(pos), normal(n), texCoord(t){}
    };

    virtual ~Model();
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    static ModelRef create(std::string_view filePath);
    static ModelRef create();

    [[nodiscard]] GLenum getPrimitiveType() const { return this->primitiveType; }
    void setPrimitiveType(GLenum primitive) { this->primitiveType = primitive; }
    [[nodiscard]] const std::vector<Vertex>& getVertices() const { return this->vertices; }
    [[nodiscard]] std::vector<Vertex>& getVertices() { return this->vertices; }
    [[nodiscard]] const std::vector<uint32_t>& getIndices() const { return this->indices; }
    [[nodiscard]] std::vector<uint32_t>& getIndices() { return this->indices; }
    [[nodiscard]] int32_t getVertexCount() const { return (int32_t)this->vertices.size(); }
    [[nodiscard]] int32_t getIndexCount() const { return (int32_t)this->indices.size(); }

    virtual void draw();
    void clearVertices();
    void reserveVertices(size_t size);
    void addVertices(const Vertex& vertex);
    void setVertices(std::vector<Vertex> vertices, std::vector<uint32_t> indices = {});
    void updateGL();

    std::string name{};

protected:
    Model();

    GLenum primitiveType{GL_TRIANGLES};
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    GLuint vao{};
    GLuint vbo{};
    GLuint ebo{};
    uint32_t vboBufferSize{0};
    uint32_t eboBufferSize{0};
};

