#include "computer.h"
#include "human.h"

void Computer::takeOver(Human* player) {
  std::vector<Card*> discards = player->discards();
  std::vector<Card*> hand = player->hand();
  int points = player->points();

  this->points(points);
  this->discards(discards);
  this->hand(hand);
}

// precondition:
//   - legalPlays is a subset of the computer's hand
//   - hand isn't empty
void Computer::play(std::vector<Card*> legalPlays) {
  std::vector<Card*> hand = this->hand();
  if (legalPlays.size() == 0) {
    this->discardCard(hand.at(0));
  } else {
    this->playCard(legalPlays.at(0));
  }
}
