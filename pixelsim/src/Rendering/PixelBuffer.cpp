#include "Common.hpp"
#include "PixelBuffer.hpp"

namespace Kumo {

    PixelBuffer::PixelBuffer(UCount width, UCount height)
        : m_width(width)
        , m_height(height)
        , m_buffer(nullptr)
        , m_texture(0)
    {
        m_buffer = new Pixel[width * height];
        for (UIndex v = 0; v < height; v++)
            for (UIndex u = 0; u < width; u++)
                Set(u, v, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
        glTextureStorage2D(m_texture, 1, GL_RGBA32F, width, height);
    }

    PixelBuffer::~PixelBuffer() {
        delete[] m_buffer;
    }

    void PixelBuffer::BufferData() const {
        glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGBA, GL_FLOAT, m_buffer);
    }

}