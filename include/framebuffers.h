#pragma once
#include <glad/glad.h>

#include <iostream>

class FrameBuffer
{
    uint32_t mWidth;
    uint32_t mHeight;
    unsigned int fbo;
    unsigned int tbo;
    unsigned int rbo;
    void addTextureAttachment()
    {
        bind();
        glGenTextures(1, &tbo);
        glBindTexture(GL_TEXTURE_2D, tbo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
        unbind();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void addDepthAttachment()
    {
        bind();
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        unbind();
    }

public:
    FrameBuffer(uint32_t width, uint32_t height, bool addColorBuffer = false) : mWidth(width), mHeight(height)
    {
        glGenFramebuffers(1, &fbo);
        if (addColorBuffer)
            addTextureAttachment();
        addDepthAttachment();
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    unsigned getTextureBuffer() { return tbo; }
    unsigned getDepthBuffer() { return rbo; }
    void bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, mWidth, mHeight);
    }
    void unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};