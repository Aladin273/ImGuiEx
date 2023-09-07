#pragma once

#include <vector>
#include <string>
#include <functional>

#include <imgui.h>

struct GLFWwindow;

namespace ImGuiEx
{
    enum class Modifier
    {
        NoModifier = 0,
        Shift = 1,
        Control = 2,
        Alt = 4,
        Super = 8,
    };

    enum class Action
    {
        Release = 0,
        Press = 1,
        Repeat = 2,
    };

    enum class ButtonCode
    {
        Button_1 = 0,
        Button_2 = 1,
        Button_3 = 2,
        Button_4 = 3,
        Button_5 = 4,
        Button_6 = 5,
        Button_7 = 6,
        Button_8 = 7,
        Button_Last = Button_8,
        Button_Left = Button_1,
        Button_Right = Button_2,
        Button_Middle = Button_3,
    };

    enum class KeyCode
    {
        Unknowm = -1,
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        Key_0 = 48,
        Key_1 = 49,
        Key_2 = 50,
        Key_3 = 51,
        Key_4 = 52,
        Key_5 = 53,
        Key_6 = 54,
        Key_7 = 55,
        Key_8 = 56,
        Key_9 = 57,
        Semicolon = 59,
        Equal = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        Left_Bracket = 91,
        Backslash = 92,
        Right_Bracket = 93,
        Grave_Accent = 96,
        World_1 = 161,
        World_2 = 162,
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Inset = 260,
        Delete = 261,
        Rigth = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        Page_Up = 266,
        Page_Down = 267,
        Home = 268,
        End = 269,
        Caps_Lock = 280,
        Scroll_Lock = 281,
        Num_Lock = 282,
        Print_Screen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        KP_0 = 320,
        KP_1 = 321,
        KP_2 = 322,
        KP_3 = 323,
        KP_4 = 324,
        KP_5 = 325,
        KP_6 = 326,
        KP_7 = 327,
        KP_8 = 328,
        KP_9 = 329,
        KP_Decimal = 330,
        KP_Divide = 331,
        KP_Multiply = 332,
        KP_Subtract = 333,
        KP_Add = 334,
        KP_Enter = 335,
        KP_Equal = 336,
        LEFT_Shift = 340,
        LEFT_Control = 341,
        LEFT_Alt = 342,
        LEFT_Super = 343,
        Right_Shift = 344,
        Right_Control = 345,
        Right_Alt = 346,
        Right_Super = 347,
        Menu = 348
    };

    class Layer
    {
    public:
        virtual ~Layer() = default;
        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnUpdate(double delta) {}
        virtual void OnRender() {}
    };

    class Window
    {
    public:
        using KeyCallback = std::function<void(KeyCode, Action, Modifier)>;
        using CursorPosCallback = std::function<void(double, double)>;
        using MouseCallback = std::function<void(ButtonCode, Action, Modifier, double, double)>;
        using ScrollCallback = std::function<void(double, double)>;
        using FramebufferSizeCallback = std::function<void(double, double)>;

        Window(const std::string& title = "ImGuiEx", uint32_t width = 1280, uint32_t height = 800, bool viewports = false, bool docking = false);
        virtual ~Window();

        // Collect layers for further execution
        void PushLayer(Layer* layer);
        // Executes all pushed layers in loop
        bool Execute();
        // Executes one layer one time
        bool Execute(Layer* layer);
        // Forcefully exits the application.
        void Close();

        void Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        void Clear(float r = 0.3f, float g = 0.3f, float b = 0.3f, float a = 1.0f);

        bool ShouldClose();
        void SwapBuffers();
        void WaitEvents();
        void PollEvents();

        double GetTime() const;
        void* GetHandle() const;

        const std::string& GetTitle() const;
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        Action GetKey(KeyCode) const;
        Action GetMouse(ButtonCode code) const;
        void GetScroll(double& xoffset, double& yoffset) const;
        void GetCursor(double& x, double& y) const;

        void SetFramebufferSizeCallback(const FramebufferSizeCallback& callback);
        void SetKeyCallback(const KeyCallback& callback);
        void SetMouseCallback(const MouseCallback& callback);
        void SetScrollCallback(const ScrollCallback& callback);
        void SetCursorCallback(const CursorPosCallback& callback);

        friend void OnFramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
        friend void OnKeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
        friend void OnMouseCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
        friend void OnScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset);
        friend void OnCursorCallbackWrapper(GLFWwindow* window, double xpos, double ypos);;

    private:
        std::string m_title = "ImGuiEx";
        uint32_t m_width = 1280;
        uint32_t m_height = 960;

        bool m_viewports = false;
        bool m_docking = false;

        bool m_running = true;
        double m_lastTime = 0.0;
        double m_deltaTime = 0.0;
        double m_xoffset = 0.0, m_yoffset = 0.0;

        GLFWwindow* m_handle = nullptr;
        std::vector<Layer*> m_layers;

        std::vector<KeyCallback> m_keyCallbacks;
        std::vector<CursorPosCallback> m_cursorCallbacks;
        std::vector<MouseCallback> m_mouseCallbacks;
        std::vector<ScrollCallback> m_scrollCallbacks;
        std::vector<FramebufferSizeCallback> m_sizeCallbacks;
    };

    void OnFramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
    void OnKeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
    void OnCursorCallbackWrapper(GLFWwindow* window, double xpos, double ypos);
    void OnMouseCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
    void OnScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset);
}