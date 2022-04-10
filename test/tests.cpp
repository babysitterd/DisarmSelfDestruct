#include <catch2/catch.hpp>

#include "../src/CrackCodeGame.h"

TEST_CASE("CrackCodeGame")
{
  CrackCodeGame puzzle({ CrackCodeGame::up, CrackCodeGame::down });

  REQUIRE(puzzle.ActiveLayer() == 0);
  REQUIRE_FALSE(puzzle.LastGuess());
  REQUIRE_FALSE(puzzle.IsSolved());

  SECTION("Guessed wrong first attempt")
  {
    puzzle.Guess(CrackCodeGame::right);
    REQUIRE(puzzle.ActiveLayer() == 0);
    REQUIRE(puzzle.LastGuess());
    REQUIRE_FALSE(*puzzle.LastGuess());
    REQUIRE_FALSE(puzzle.IsSolved());
  }

  SECTION("Guessed right first attempt")
  {
    puzzle.Guess(CrackCodeGame::up);
    REQUIRE(puzzle.ActiveLayer() == 1);
    REQUIRE(puzzle.LastGuess());
    REQUIRE(*puzzle.LastGuess());
    REQUIRE_FALSE(puzzle.IsSolved());

    SECTION("Guessed right second attempt - solved!")
    {
      puzzle.Guess(CrackCodeGame::down);
      REQUIRE(puzzle.ActiveLayer() == 2);
      REQUIRE(puzzle.LastGuess());
      REQUIRE(*puzzle.LastGuess());
      REQUIRE(puzzle.IsSolved());
    }

    SECTION("Guessed wrong second attempt")
    {
      puzzle.Guess(CrackCodeGame::right);
      REQUIRE(puzzle.ActiveLayer() == 0);
      REQUIRE(puzzle.LastGuess());
      REQUIRE_FALSE(*puzzle.LastGuess());
      REQUIRE_FALSE(puzzle.IsSolved());
    }
  }
}
