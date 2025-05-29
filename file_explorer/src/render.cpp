#include "render.hpp"

#include <filesystem>
#include <iostream>

void Renderer::Draw(const std::string &label,
                    const ImGuiWindowFlags flags) noexcept {
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(static_cast<float>(Renderer::SCREEN_W),
                                  static_cast<float>(Renderer::SCREEN_H)));

  ImGui::Begin(label.c_str(), nullptr, flags);

  DrawContent();
  ImGui::SameLine();
  DrawMenu();
  ImGui::SameLine();
  DrawActions();
  ImGui::SameLine();
  DrawFilter();

  ImGui::End();
}

void Renderer::DrawContent() noexcept {
  if (ImGui::Button("Go Up")) {
    if (m_current_path.has_parent_path()) {
      m_current_path = m_current_path.parent_path();
    }
  }

  ImGui::SameLine();
  ImGui::Text("Current directory, %s", m_current_path.c_str());
}

void Renderer::DrawMenu() noexcept {
  for (const auto &entry : fs::directory_iterator(m_current_path)) {
    std::cout << entry << std::endl;
  }
}

void Renderer::DrawFilter() noexcept { ImGui::Text("Filter"); }

void Renderer::DrawActions() noexcept { ImGui::Text("Actions"); }
