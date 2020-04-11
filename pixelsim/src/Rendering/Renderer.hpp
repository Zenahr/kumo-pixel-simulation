#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Rendering/PixelBuffer.hpp"

namespace Kumo {

    class Renderer {
    public:
        Renderer(GLFWwindow* window_handle);
        ~Renderer();

        void Render(const PixelBuffer& buffer);
    private:
        GLFWwindow*  m_window_handle;
        GLuint       m_vao;
        GLuint       m_vbo;
        GLuint       m_shader;
    };

}