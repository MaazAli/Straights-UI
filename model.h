#ifndef MODEL_H
#define MODEL_H
#include "card.h"
#include "deck.h"
#include "subject.h"
#include "player.h"
#include <vector>
#include <string>

class Model : public Subject {
public:
  Model();
  ~Model();

  // View getters
  std::vector<Card*> cardsInHand();
  std::vector<std::vector<Card*> > cardsOnTable();
  std::vector<int> points();
  std::vector<std::vector<Card*> > discards();

  // manipulate model
  void rageQuitActivePlayer();
  void resetGame();
  void addPlayer(std::string type);
  void activePlayerId(int id);
  void activePlayerSelectCard(Card* card);
  void seed(int seed);

  // temporary public method
  std::vector<Card*> legalPlaysInHand(std::vector<Card*> hand) const;

  // public accessors
  int activePlayerId() const;
private:
  // helper methods
  void resetPlayers();
  void clearCardsOnTable();
  void shuffleDeck(int seed);
  void dealCardsToPlayers();

  // given the hand, what are the legal plays?
  // ehhh, introduces coupling between computer player and model class
  // std::vector<Card*> legalPlaysInHand(std::vector<Card*> hand);

  // helper accessors
  Player* activePlayer() const;
  int seed() const;
  Deck* deck() const;

  // helper mutators
  //   replaces the active player with p
  void activePlayer(Player* p);

  // private members
  std::vector<Player*> players_;
  std::vector<std::vector<Card*> > cardsOnTable_;
  std::vector<int> points_;
  int activePlayerId_;
  Deck* deck_;
  int seed_;
};

#endif
