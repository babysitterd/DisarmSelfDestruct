#pragma once

#include <vector>
#include <optional>

class CrackCodeGame
{
public:
  enum Direction
  {
    up, right, down, left, count
  };

  explicit CrackCodeGame(int layersCount);

  void Guess(Direction guess);

  [[nodiscard]] int ActiveLayer() const;

  [[nodiscard]] std::optional<bool> const& LastGuess() const;
  [[nodiscard]] bool IsSolved() const;

private:
  int const m_layersCount;
  int m_activeLayer = 0;
  std::vector<Direction> m_code;
  std::optional<bool> m_lastGuess;
};