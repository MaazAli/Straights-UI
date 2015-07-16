#ifndef DECK_H
#define DECK_H
#include "card.h"
#include <vector>

class Deck {
public:
  Deck();
  Card& cardAt(int index) const;
  void shuffle(int seed);
  void unshuffle();
private:
  std::vector<Card*> cards_;
  std::vector<Card*> orderedCards_;
};

#endif
