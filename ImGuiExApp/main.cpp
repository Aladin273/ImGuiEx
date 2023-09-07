#include "imguiex.h"

class ExampleLayer : public ImGuiEx::Layer
{
public:
    virtual void OnRender() override
    {
        ImGui::Begin("Hello");
        ImGui::Button("Button");
        ImGui::End();

        ImGui::ShowDemoWindow();
    }
};

int main()
{
    // Simple way (push layers and execute app)
    ///////////////////////////////////////////

    ImGuiEx::Window window;
    window.PushLayer(new ExampleLayer());

    return window.Execute();

    // Advanced way (loop and layers control)
    ///////////////////////////////////////////
    
    //ImGuiEx::Window window;
    //ExampleLayer* layer = new ExampleLayer();
    //
    //while (!window.ShouldClose())
    //{
    //    window.Viewport(0, 0, window.GetWidth(), window.GetHeight());
    //
    //    window.Clear();
    //    window.Execute(layer);
    //
    //    window.PollEvents();
    //    window.SwapBuffers();
    //}
    //
    //delete layer;
    //return 0;
}