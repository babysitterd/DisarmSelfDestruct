#include "CrackCodeGame.h"

#include <random>

CrackCodeGame::CrackCodeGame(int layersCount) :
  m_layersCount(layersCount)
{
  // generate random solution sequence
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, count - 1);

  for (int i = 0; i < m_layersCount; ++i)
  {
    m_code.push_back(static_cast<Direction>(distr(gen)));
  }
}

void CrackCodeGame::Guess(Direction guess)
{
  if (IsSolved())
  {
    return;
  }

  if (m_code[static_cast<std::size_t>(m_activeLayer)] == guess)
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

int CrackCodeGame::ActiveLayer() const
{
  return m_activeLayer;
}

std::optional<bool> const& CrackCodeGame::LastGuess() const
{
  return m_lastGuess;
}

bool CrackCodeGame::IsSolved() const
{
  return m_activeLayer == m_layersCount;
}
