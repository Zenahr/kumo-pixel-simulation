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
//-------------------------- O.O naru-hodo


namespace Kumo {

    static constexpr UInt32
        ScreenWidth  = 900,
        ScreenHeight = 900;

	// define the data which will be put into the fragment shader.
	// format: RGBA
    static constexpr Pixel
        Black = { 0.0f, 0.0f, 0.0f,  1.0f },
        Sandy = { 0.5f, 0.8f, 0.05f, 1.0f };
        //Sandy = { 0.1f, 0.4f, 0.9f,  1.0f };

	// init window, renderer and buffer. Shouldn't the pixelbuffer be initialized before the renderer? Not that it matters but maybe would follow the rendering logic more.
    static GLFWwindow*  WindowHandle;
    static Renderer*    RendererInstance;
    static PixelBuffer* Buffer;

	// Some kind of init for the data to be passed to buffer.
    struct Sand {
        UIndex X, Y;
    };

	// Create window via GLFW
    static void Initialize() {
        if (!glfwInit())
            throw std::runtime_error("glfwInit");
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        WindowHandle = glfwCreateWindow(
            static_cast<int>(ScreenWidth),
            static_cast<int>(ScreenHeight),
            "Pixel-Rendering-Simulation",
            nullptr,
            nullptr
        );
        if (!WindowHandle)
            throw std::runtime_error("glfwCreateWindow");
        glfwSwapInterval(0);

		// Init renderer
        RendererInstance = new Renderer(WindowHandle);
        
		// Init buffer. Again, why in this order?
		Buffer = new PixelBuffer(ScreenWidth, ScreenHeight);
    }

	// closes the window and removes buffer, renderer and window from memory.
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

	// Sets bounds for filling when sand touches ground
	unsigned int sandContainerWidth = 10;
    static UIndex
		XLeft  = ScreenWidth/2 - sandContainerWidth,
        XRight = ScreenWidth / 2 + sandContainerWidth;

    static void Update() {
        std::vector<Sand> sand;
        while (!glfwWindowShouldClose(WindowHandle)) {
            glfwPollEvents();

            if (glfwGetKey(WindowHandle, GLFW_KEY_SPACE)) {
				// push pixel into stack
				// first value sets exit point of sand
                sand.push_back({ScreenWidth/2, 12});
                Buffer->Set(ScreenWidth/2, 12, Sandy);
            }

			// Moves sand to the left and right.
            if (glfwGetKey(WindowHandle, GLFW_KEY_R)) {
                XLeft  = 255;
                XRight = 255;
            }

            for (Sand& s : sand) {
				// factor makes the pixel move pixel*factor pixel.
				unsigned int factor = 10;
                const UIndex
                    under = s.Y + 1 * factor,
                    left  = s.X - 1 * factor,
                    right = s.X + 1 * factor;
                
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
