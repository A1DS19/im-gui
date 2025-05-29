#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class Renderer final {
public:
  inline static constexpr uint16_t SCREEN_W = 1280;
  inline static constexpr uint16_t SCREEN_H = 720;

  Renderer() : m_current_path(fs::current_path()) {}

  void Draw(const std::string &label,
            const ImGuiWindowFlags flags = 0) noexcept;

private:
  fs::path m_current_path;
  void DrawContent() noexcept;
  void DrawMenu() noexcept;
  void DrawFilter() noexcept;
  void DrawActions() noexcept;
};
