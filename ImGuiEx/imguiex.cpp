#include "imguiex.h"

#define GLAD_GL_IMPLEMENTATION

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace ImGuiEx
{
    Window::Window(const std::string& title, uint32_t width, uint32_t height)
        : m_title(title), m_width(width), m_height(height)
    {
        glfwInit();

        m_handle = glfwCreateWindow(m_width, m_height, m_title.data(), nullptr, glfwGetCurrentContext());
        glfwMakeContextCurrent(m_handle);
        glfwSwapInterval(0);

        static bool initGL = false;
        if (!initGL)
        {
            initGL = true;
            gladLoadGL(glfwGetProcAddress);
        }

        m_lastTime = GetTime();

        glfwSetWindowUserPointer(m_handle, this);
        glfwSetInputMode(m_handle, GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwSetInputMode(m_handle, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        glfwSetInputMode(m_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        glfwSetFramebufferSizeCallback(m_handle, OnFramebufferSizeCallbackWrapper);
        glfwSetKeyCallback(m_handle, OnKeyCallbackWrapper);
        glfwSetCursorPosCallback(m_handle, OnCursorCallbackWrapper);
        glfwSetMouseButtonCallback(m_handle, OnMouseCallbackWrapper);
        glfwSetScrollCallback(m_handle, OnScrollCallbackWrapper);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsClassic();

        ImGui_ImplGlfw_InitForOpenGL(m_handle, true);
        ImGui_ImplOpenGL3_Init();
    }

    Window::~Window()
    {
        for (auto& layer : m_layers)
        {
            delete layer;
            layer = nullptr;
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_handle);
    }

    void Window::PushLayer(Layer* layer)
    {
        m_layers.push_back(layer);
        m_layers.back()->OnAttach();
    }

    bool Window::Execute()
    {
        while (m_running && !ShouldClose())
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            for (auto& layer : m_layers)
            {
                layer->OnUpdate(m_deltaTime);
                layer->OnRender();
            }

            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(static_cast<float>(GetWidth()), static_cast<float>(GetHeight()));

            ImGui::Render();
            glViewport(0, 0, static_cast<int>(GetWidth()), static_cast<int>(GetHeight()));
            glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup);
            }

            double time = GetTime();
            m_deltaTime = time - m_lastTime;
            m_lastTime = time;

            PollEvents();
            SwapBuffers();
        }

        return true;
    }

    bool Window::Execute(Layer* layer)
    {        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        layer->OnUpdate(m_deltaTime);
        layer->OnRender();
        
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(GetWidth()), static_cast<float>(GetHeight()));
        
        ImGui::Render();
        glViewport(0, 0, static_cast<int>(GetWidth()), static_cast<int>(GetHeight()));
        glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
        }

        double time = GetTime();
        m_deltaTime = m_lastTime - time;
        m_lastTime = time;

        return true;
    }

    void Window::Close()
    {
        m_running = false;
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(m_handle);
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(m_handle);
    }

    void Window::WaitEvents()
    {
        glfwWaitEvents();
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    double Window::GetTime() const
    {
        return glfwGetTime();
    }

    void* Window::GetHandle() const
    {
        return m_handle;
    }

    const std::string& Window::GetTitle() const
    {
        return m_title;
    }

    uint32_t Window::GetWidth() const
    {
        return m_width;
    }

    uint32_t Window::GetHeight() const
    {
        return m_height;
    }

    Action Window::GetKey(KeyCode code) const
    {
        return static_cast<Action>(glfwGetKey(m_handle, static_cast<int>(code)));
    }

    Action Window::GetMouse(ButtonCode code) const
    {
        return static_cast<Action>(glfwGetMouseButton(m_handle, static_cast<int>(code)));
    }

    void Window::GetScroll(double& xoffset, double& yoffset) const
    {
        xoffset = m_xoffset; yoffset = m_yoffset;
    }

    void Window::GetCursor(double& x, double& y) const
    {
        glfwGetCursorPos(m_handle, &x, &y);
        y = m_height - y - 1.0;
    }

    void Window::SetFramebufferSizeCallback(const FramebufferSizeCallback& callback)
    {
        m_sizeCallbacks.push_back(callback);
    }

    void Window::SetKeyCallback(const KeyCallback& callback)
    {
        m_keyCallbacks.push_back(callback);
    }

    void Window::SetMouseCallback(const MouseCallback& callback)
    {
        m_mouseCallbacks.push_back(callback);
    }

    void Window::SetScrollCallback(const ScrollCallback& callback)
    {
        m_scrollCallbacks.push_back(callback);
    }

    void Window::SetCursorCallback(const CursorPosCallback& callback)
    {
        m_cursorCallbacks.push_back(callback);
    }

    void OnFramebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height)
    {
        Window* current = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

        for (auto& callback : current->m_sizeCallbacks)
        {
            current->m_width = width;
            current->m_height = height;
            callback(width, height);
        }
    }

    void OnKeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* current = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

        for (auto& callback : current->m_keyCallbacks)
        {
            callback(static_cast<KeyCode>(key), static_cast<Action>(action), static_cast<Modifier>(mods));
        }
    }

    void OnMouseCallbackWrapper(GLFWwindow* window, int button, int action, int mods)
    {
        Window* current = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

        for (auto& callback : current->m_mouseCallbacks)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            callback(static_cast<ButtonCode>(button), static_cast<Action>(action), static_cast<Modifier>(mods), xpos, current->m_height - ypos - 1.0);
        }
    }

    void OnScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset)
    {
        Window* current = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

        for (auto& callback : current->m_scrollCallbacks)
        {
            current->m_xoffset = xoffset;
            current->m_yoffset = yoffset;
            callback(xoffset, yoffset);
        }
    }

    void OnCursorCallbackWrapper(GLFWwindow* window, double xpos, double ypos)
    {
        Window* current = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

        for (auto& callback : current->m_cursorCallbacks)
        {
            callback(xpos, current->m_height - ypos - 1.0);
        }
    }
}
