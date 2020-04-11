#include "Common.hpp"

#include <GLFW/glfw3.h>

#include "Rendering/PixelBuffer.hpp"
#include "Rendering/Renderer.hpp"

// Pixel is a synonym for glm::vec4 which is a struct of four floats. - Kumo, 09.03.2020
// The buffer is an array of those structs
// ------------------------ so basically you feed the texturemapper just an array of structs
// Yes
//------------------------- ...
// But OpenGL just reads all the bytes without knowledge of structures
//-------------------------- O.O


namespace Kumo {

    static constexpr UInt32
        ScreenWidth  = 1256,
        ScreenHeight = 1256;

    static constexpr Pixel
        Black = { 0.0f, 0.0f, 0.0f,  1.0f },
        // Sandy = { 0.5f, 0.8f, 0.05f, 1.0f };
        Sandy = { 0.1f, 0.4f, 0.9f,  1.0f };

    static GLFWwindow*  WindowHandle;
    static Renderer*    RendererInstance;
    static PixelBuffer* Buffer;

    struct Sand {
        UIndex X, Y;
    };

    static void Initialize() {
        if (!glfwInit())
            throw std::runtime_error("glfwInit");
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        WindowHandle = glfwCreateWindow(
            static_cast<int>(ScreenWidth),
            static_cast<int>(ScreenHeight),
            "Pixelsim",
            nullptr,
            nullptr
        );
        if (!WindowHandle)
            throw std::runtime_error("glfwCreateWindow");
        glfwSwapInterval(0);

        RendererInstance = new Renderer(WindowHandle);
        Buffer = new PixelBuffer(ScreenWidth, ScreenHeight);
    }

    static void Clean() {
        delete Buffer;
        delete RendererInstance;
        glfwDestroyWindow(WindowHandle);
    }

    static bool IsSand(UIndex x, UIndex y) {
        return Buffer->Get(x, y) == Sandy;
    }

    static bool IsVoid(UIndex x, UIndex y) {
        return Buffer->Get(x, y) == Black;
    }

    static void SetSand(UIndex x, UIndex y) {
        Buffer->Set(x, y, Sandy);
    }

    static void SetVoid(UIndex x, UIndex y) {
        Buffer->Set(x, y, Black);
    }

    static UIndex
        XLeft  = 100,
        XRight = 156;

    static void Update() {
        std::vector<Sand> sand;
        while (!glfwWindowShouldClose(WindowHandle)) {
            glfwPollEvents();

            if (glfwGetKey(WindowHandle, GLFW_KEY_SPACE)) {
                sand.push_back({128, 12});
                Buffer->Set(128, 12, Sandy);
            }
            if (glfwGetKey(WindowHandle, GLFW_KEY_R)) {
                XLeft  = 0;
                XRight = 255;
            }

            for (Sand& s : sand) {
                const UIndex
                    under = s.Y + 1,
                    left  = s.X - 1,
                    right = s.X + 1;
                
                if (under >= ScreenHeight)
                    continue;
                if (IsVoid(s.X, under)) {
                    SetVoid(s.X, s.Y);
                    s.Y = under;
                    SetSand(s.X, s.Y);
                    continue;
                }
                if (left >= XLeft && IsVoid(left, under)) {
                    SetVoid(s.X, s.Y);
                    s.X = left;
                    s.Y = under;
                    SetSand(s.X, s.Y);
                    continue;
                }
                if (right <= XRight && IsVoid(right, under)) {
                    SetVoid(s.X, s.Y);
                    s.X = right;
                    s.Y = under;
                    SetSand(s.X, s.Y);
                    continue;
                }
                if (left >= XLeft && IsVoid(left, s.Y)) {
                    SetVoid(s.X, s.Y);
                    s.X = left;
                    SetSand(s.X, s.Y);
                    continue;
                }
                if (right <= XRight && IsVoid(right, s.Y)) {
                    SetVoid(s.X, s.Y);
                    s.X = right;
                    SetSand(s.X, s.Y);
                    continue;
                }
            }

            RendererInstance->Render(*Buffer);
            glfwSwapBuffers(WindowHandle);
        }
    }

}

int main(int, char**) {
    Kumo::Initialize();
    Kumo::Update();
    Kumo::Clean();
    return 0;
}
