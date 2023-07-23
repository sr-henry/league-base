#include "menu.h"
#include "imgui/imgui.h"

void Menu::RenderUI()
{
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("league-base", &Menu::open);

    ImGui::Text("This is some useful text.");
    //ImGui::Checkbox("Demo Window", &show_demo_window);
    //ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    if (ImGui::Button("Button"))
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::End();
}
