#include "CrackCodeGame.h"

#include <random>

CrackCodeGame::CrackCodeGame(std::size_t layersCount)
{
  // generate random solution sequence
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, count - 1);

  for (std::size_t i = 0; i < layersCount; ++i)
  {
    m_code.push_back(static_cast<Direction>(distr(gen)));
  }
}

CrackCodeGame::CrackCodeGame(std::vector<Direction> code) :
  m_code(std::move(code))
{
}

void CrackCodeGame::Guess(Direction guess)
{
  if (IsSolved())
  {
    return;
  }

  if (m_code[m_activeLayer] == guess)
  {
    ++m_activeLayer;
    m_lastGuess = true;
  }
  else
  {
    m_activeLayer = 0;
    m_lastGuess = false;
  }
}

std::size_t CrackCodeGame::ActiveLayer() const
{
  return m_activeLayer;
}

std::optional<bool> const& CrackCodeGame::LastGuess() const
{
  return m_lastGuess;
}

bool CrackCodeGame::IsSolved() const
{
  return m_activeLayer == m_code.size();
}
