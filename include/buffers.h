#pragma once
#include <glad/glad.h>

/*
usage
VAO vao;
VBO vbo;
EBO ebo;
vbo.setLayout();
vao.bind();
vbo.bind();
ebo.bind();
vao.unbind();
vbo.unbind();
ebo.unbind();

*/

struct IndexBuffer
{
    unsigned int *mIndices;
    int mSize;
    unsigned int ID;
    IndexBuffer(unsigned int indices[], uint32_t size)
    {
        mIndices = indices;
        mSize = size;
        glGenBuffers(1, &ID);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
        unbind();
    }
    void bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }
    void unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};

struct VertexBuffer
{
    float *mVertices;
    int mSize;
    unsigned int ID;
    VertexBuffer(float vertices[], uint32_t size)
    {
        mVertices = vertices;
        mSize = size;
        glGenBuffers(1, &ID);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        unbind();
    }
    void setLayout(uint32_t index, uint32_t stride, uint32_t vertexSize, uint32_t offset)
    {
        bind();
        glVertexAttribPointer(index, stride, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void *)(offset * sizeof(float)));
        glEnableVertexAttribArray(index);
        unbind();
    }
    void bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    }
    void unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

struct VertexArray
{
    unsigned int ID;
    VertexArray()
    {
        glGenVertexArrays(1, &ID);
    }
    void bind()
    {
        glBindVertexArray(ID);
    }
    void unbind()
    {
        glBindVertexArray(0);
    }
};
