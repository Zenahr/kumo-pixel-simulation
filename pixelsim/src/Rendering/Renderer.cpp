#include "Common.hpp"
#include "Renderer.hpp"

namespace Kumo {

    static constexpr std::array<float, 6 * 4> SquareVertices {
    //   x      y     u     v
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    static constexpr GLchar* VertexSrc = R"(
        #version 450

        in vec2 v_pos;
        in vec2 v_texcoords;

        out vec2 f_texcoords;

        void main() {
            gl_Position = vec4(v_pos, 0.0f, 1.0f);
            f_texcoords = v_texcoords;
        }
    )";

    static constexpr GLchar* FragmentSrc = R"(
        #version 450

        uniform sampler2D u_texture;

        in vec2 f_texcoords;

        out vec4 o_color;

        void main() {
            o_color = texture(u_texture, f_texcoords);
        }
    )";

    static void GLAPIENTRY GLMessageCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {
        if (severity == GL_DEBUG_SEVERITY_HIGH
                || severity == GL_DEBUG_SEVERITY_MEDIUM
                || severity == GL_DEBUG_SEVERITY_LOW) {
            fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                type, severity, message);
        }
    }

    Renderer::Renderer(GLFWwindow* window_handle)
        : m_window_handle(window_handle)
    { 
        glfwMakeContextCurrent(window_handle);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("Could not initialize Glad.");
        }
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLMessageCallback, nullptr);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glCreateVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        glCreateBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SquareVertices.size(),
            SquareVertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            4 * sizeof(float),
            reinterpret_cast<const void*>(0)
        );
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            4 * sizeof(float),
            reinterpret_cast<const void*>(2 * sizeof(float))
        );
        glBindVertexArray(0);
        m_shader = glCreateProgram();
        const GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshader, 1, &VertexSrc, nullptr);
        glCompileShader(vshader);
        glAttachShader(m_shader, vshader);
        
        const GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshader, 1, &FragmentSrc, nullptr);
        glCompileShader(fshader);
        glAttachShader(m_shader, fshader);

        glLinkProgram(m_shader);

        glDetachShader(m_shader, fshader);
        glDetachShader(m_shader, vshader);
        glDeleteShader(fshader);
        glDeleteShader(vshader);

        glUseProgram(m_shader);
        glUniform1i(glGetUniformLocation(m_shader, "u_texture"), 0);
        glUseProgram(0);
    }

    Renderer::~Renderer() {
        glDeleteProgram(m_shader);
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }

    void Renderer::Render(const PixelBuffer& buffer) {
        buffer.BufferData();
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(m_vao);
        buffer.BindTexture(0);
        glUseProgram(m_shader);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glUseProgram(0);
        glBindVertexArray(0);
    }

}