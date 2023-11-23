//
// Created by Yun on 2022/6/25.
//

#include "Model.h"

Model::Model()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

Model::~Model()
{
    if (this->vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    if (this->vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (this->ebo != 0)
    {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }
}

ModelRef Model::create()
{
    auto model = new Model;
    return ModelRef(model);
}

ModelRef Model::create(std::string_view filePath)
{
    auto model = new Model;
    model->name = filePath;

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filePath.data()))
    {
        std::string msg = "Load Obj model failed! path: " + std::string{filePath};
        LOGE("{}", msg);
        if (!reader.Error().empty())
        {
            LOGE("TinyObjReader: {}", reader.Error());
        }
        throw std::runtime_error(msg.c_str());
    }

    if (!reader.Warning().empty())
    {
        LOGW("TinyObjReader: {}", reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    // Loop over shapes
    for (const auto & shape : shapes) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            auto fv = size_t(shape.mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                auto& vertex = model->vertices.emplace_back();
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                vertex.position.x = attrib.vertices[3*size_t(idx.vertex_index)+0];
                vertex.position.y = attrib.vertices[3*size_t(idx.vertex_index)+1];
                vertex.position.z = attrib.vertices[3*size_t(idx.vertex_index)+2];

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    vertex.normal.x = attrib.normals[3*size_t(idx.normal_index)+0];
                    vertex.normal.y = attrib.normals[3*size_t(idx.normal_index)+1];
                    vertex.normal.z = attrib.normals[3*size_t(idx.normal_index)+2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    if (tx > 1.0 || tx < 0.0)
                    {
                        tx = glm::mod(tx, 1.0f);
                    }
                    if (ty > 1.0 || ty < 0.0)
                    {
                        ty = glm::mod(ty, 1.0f);
                    }
                    vertex.texCoord = {tx, ty};
                }
            }
            index_offset += fv;
        }
    }

    model->updateGL();

    return ModelRef(model);
}

void Model::clearVertices()
{
    this->vertices.clear();
}

void Model::reserveVertices(size_t size)
{
    this->vertices.reserve(size);
}

void Model::addVertices(const Vertex& vertex)
{
    this->vertices.push_back(vertex);
}

void Model::setVertices(std::vector<Vertex> vs, std::vector<uint32_t> is /*={}*/)
{
    this->vertices = std::move(vs);
    this->indices = std::move(is);
    this->updateGL();
}

void Model::draw()
{
    if(vao == 0 || getVertexCount() <= 0)
    {
        return;
    }

    glBindVertexArray(vao);
    if(this->ebo != 0 && getIndexCount()>0)
    {
        glDrawElements(primitiveType, getIndexCount(), GL_UNSIGNED_INT, nullptr );
    }
    else
    {
        glDrawArrays(primitiveType, 0, getVertexCount());
    }
    glBindVertexArray(0);

}

void Model::updateGL()
{
    auto bufferSize = static_cast<int>(this->vertices.size() * sizeof(Vertex));
    if(bufferSize == 0)
    {
        return ;
    }
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if (vboBufferSize < bufferSize)
    {
        vboBufferSize = bufferSize;
        glBufferData(GL_ARRAY_BUFFER, bufferSize, this->vertices.data(), GL_STATIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, this->vertices.data());
    }

    glEnableVertexAttribArray(AttributePointer::kPosition);
    glVertexAttribPointer(AttributePointer::kPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));

    glEnableVertexAttribArray(AttributePointer::kNormal);
    glVertexAttribPointer(AttributePointer::kNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(AttributePointer::kTexture);
    glVertexAttribPointer(AttributePointer::kTexture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));


    if (!indices.empty())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        bufferSize = static_cast<int>(indices.size() * sizeof(uint32_t));
        if(eboBufferSize < bufferSize)
        {
            eboBufferSize = bufferSize;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, this->indices.data(), GL_STATIC_DRAW);
        }
        else
        {
            glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, this->indices.data());
        }
    }

    glBindVertexArray(0);
}

