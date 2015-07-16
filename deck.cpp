#include "card.h"
#include "deck.h"
#include <string>
#include <random>

Deck::Deck() {
  for (int suit = 0; suit < 4; suit++) {
    for (int rank = 0; rank < 13; rank++) {
      Card* card = new Card{(Suit)suit, (Rank)rank};
      this->cards_.push_back(card);
      this->orderedCards_.push_back(card);
    }
  }
}

Card& Deck::cardAt(int index) const {
  return *this->cards_.at(index);
}

void Deck::shuffle(int seed) {
  static std::mt19937 rng(seed);
  int n = 52;

  while ( n > 1 ) {
    int k = (int) (rng() % n);
    --n;
    Card *c = cards_[n];
    cards_[n] = cards_[k];
    cards_[k] = c;
  }
}

void Deck::unshuffle() {
  this->cards_ = this->orderedCards_;
}
