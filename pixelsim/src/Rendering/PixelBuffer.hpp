#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Kumo {

    using Pixel = glm::vec4;

    class PixelBuffer {
    public:
        PixelBuffer(UCount width, UCount height);
        ~PixelBuffer();
        
        void BufferData() const;

		// sets the pixel at coordinates(u, v) to a value
        inline void Set(UIndex u, UIndex v, Pixel pixel) {
            m_buffer[u +  m_width * (m_height - 1 - v)] = pixel;
        }

        inline Pixel Get(UIndex u, UIndex v) {
            return m_buffer[u + m_width * (m_height - 1 - v)];
        }

        inline void BindTexture(GLuint slot) const {
            glBindTextureUnit(slot, m_texture);
        }

        inline UCount GetWidth() const {
            return m_width;
        }

        inline UCount GetHeight() const {
            return m_height;
        }
        
    private:
        UCount m_width;
        UCount m_height;
        Pixel* m_buffer;
        GLuint m_texture;
    };

}