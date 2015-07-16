#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "card.h"

class Player {
public:
  Player();
  virtual ~Player();

  // accessors for the Model
  int points() const;
  std::vector<Card*> discards() const;
  std::vector<Card*> hand() const;

  // methods that mutate state
  void playCard(Card* card); // returns the card we've played
  void takeCard(Card* card);
  void discardCard(Card* card);
  void cleanup();

protected:
  // since descendents can change types, we provide setters
  void points(int p);
  void discards(std::vector<Card*>& cards);
  void hand(std::vector<Card*>& cards);
private:
  // helper methods
  void removeFromHand(Card* card);
  
  std::vector<Card*> hand_;
  std::vector<Card*> discards_;
  int points_;
};

#endif
