#include <ftxui/component/component.hpp> // Renderer, CatchEvent
#include <ftxui/dom/canvas.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/color.hpp>

#include <optional>
#include <fmt/core.h>

#include "CrackCodeGame.h"

namespace
{

constexpr int CanvasSize = 150;
constexpr std::size_t LayersCount = 5;

std::optional<CrackCodeGame::Direction> EventToDirection(ftxui::Event const& e)
{
  if (e == ftxui::Event::ArrowUp)
  {
    return CrackCodeGame::up;
  }

  if (e == ftxui::Event::ArrowRight)
  {
    return CrackCodeGame::right;
  }

  if (e == ftxui::Event::ArrowDown)
  {
    return CrackCodeGame::down;
  }

  if (e == ftxui::Event::ArrowLeft)
  {
    return CrackCodeGame::left;
  }

  return { };
}

auto LayerColor(std::size_t current, std::size_t active)
{
      if (current == active)
      {
        return ftxui::Color::Yellow;
      }

      return current % 2 == 0 ? ftxui::Color::BlueLight : ftxui::Color::Blue;
}

} // namespace

int main()
{
  using namespace ftxui;

  CrackCodeGame puzzle(LayersCount);

  auto const timeout = std::chrono::seconds(30);
  auto const step = std::chrono::seconds(1);
  auto timeLeft = timeout.count();

  auto isGameOver = [&]
  {
    return puzzle.IsSolved() || timeLeft == 0;
  };
  
  auto renderer = Renderer([&] {
    auto c = Canvas(CanvasSize, CanvasSize);

    // draw layers
    const auto delta = CanvasSize / 2 / (static_cast<int>(LayersCount) + 1);
    auto radius = CanvasSize / 2;
    for (std::size_t i = 0; i < LayersCount; ++i, radius -= delta)
    {
      c.DrawBlockCircleFilled(CanvasSize / 2, CanvasSize / 2, radius, LayerColor(i, puzzle.ActiveLayer()));
    }
    // draw grid
    c.DrawBlockLine(0, 0, CanvasSize, CanvasSize, Color::Default);
    c.DrawBlockLine(0, CanvasSize, CanvasSize, 0, Color::Default);
    // draw core
    auto color = LayersCount % 2 == 0 ? Color::BlueLight : Color::Blue;
    if (puzzle.LastGuess())
    {
      color = *puzzle.LastGuess() ? Color::Green : Color::Red;
    }
    c.DrawBlockCircleFilled(CanvasSize / 2, CanvasSize / 2, radius, color);

    auto document = 
        vbox({
          text("ROGUE AI: OVERRIDE SYSTEM") | hcenter,
          canvas(std::move(c)) | hcenter
        }) | border;

    return document;
  });

  auto timerRenderer = Renderer([&] {
    auto message = text(fmt::format("{:<15}", "TIME LEFT:")) | color(Color::Yellow);

    if (puzzle.IsSolved())
    {
      message = text(fmt::format(":<15", "YOU WON!")) | color(Color::Green);
    }

    if (timeLeft == 0)
    {
      message = text(fmt::format(":<15", "YOU LOST :[")) | color(Color::Red);
    }

    auto document = border(hbox({
        message,
        gauge(static_cast<float>(timeLeft) / static_cast<float>(timeout.count())) | flex,
        text(" " + std::to_string(timeLeft))
    }));
    return document;
  });

  auto layout = Container::Vertical(
      { renderer, timerRenderer });

  auto screen = ftxui::ScreenInteractive::TerminalOutput();

  std::thread refreshProgressBar([&] {
    for (; !isGameOver(); timeLeft -= step.count())
    {
      std::this_thread::sleep_for(step);
      screen.PostEvent(ftxui::Event::Custom);
    }
  });

  auto decoratedLayout = CatchEvent(layout, [&](Event const& e) {
    if (isGameOver())
    {
      return true;
    }

    auto const direction = EventToDirection(e);
    if (direction)
    {
      puzzle.Guess(*direction);
      return true;
    }

    return false;
  });

  screen.Loop(decoratedLayout);
}
