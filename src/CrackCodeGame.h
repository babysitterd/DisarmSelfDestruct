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

  explicit CrackCodeGame(std::size_t layersCount);
  explicit CrackCodeGame(std::vector<Direction> code);

  void Guess(Direction guess);

  [[nodiscard]] std::size_t ActiveLayer() const;

  [[nodiscard]] std::optional<bool> const& LastGuess() const;
  [[nodiscard]] bool IsSolved() const;

private:
  std::size_t m_activeLayer = 0;
  std::vector<Direction> m_code;
  std::optional<bool> m_lastGuess;
};