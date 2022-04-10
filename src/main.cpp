#include <ftxui/component/component.hpp> // Renderer, CatchEvent
#include <ftxui/dom/canvas.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/color.hpp>

#include "CrackCodeGame.h"

namespace
{

constexpr int CanvasSize = 150;
constexpr int LayersCount = 5;

}

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
    const auto delta = CanvasSize / 2 / (LayersCount + 1);
    auto radius = CanvasSize / 2;
    for (int i = 0; i < LayersCount; ++i, radius -= delta)
    {
      auto color = i % 2 == 0 ? Color::BlueLight : Color::Blue;
      if (i == puzzle.ActiveLayer())
      {
        color = Color::Yellow;
      }

      c.DrawBlockCircleFilled(CanvasSize / 2, CanvasSize / 2, radius, color);
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
    auto message = text("TIME LEFT:") | color(Color::Yellow);

    if (puzzle.IsSolved())
    {
      message = text("YOU WON   ") | color(Color::Green);
    }

    if (timeLeft == 0)
    {
      message = text("YOU LOST  ") | color(Color::Red);
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

  auto decoratedTab = CatchEvent(layout, [&](Event const& e) {
    if (isGameOver())
    {
      return true;
    }

    if (e == Event::ArrowUp)
    {
      puzzle.Guess(CrackCodeGame::up);
      return true;
    }
    else if (e == Event::ArrowRight)
    {
      puzzle.Guess(CrackCodeGame::right);
      return true;
    }
    else if (e == Event::ArrowDown)
    {
      puzzle.Guess(CrackCodeGame::down);
      return true;
    }
    else if (e == Event::ArrowLeft)
    {
      puzzle.Guess(CrackCodeGame::left);
      return true;
    }

    return false;
  });

  screen.Loop(decoratedTab);
}
