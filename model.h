#ifndef MODEL_H
#define MODEL_H
#include "card.h"
#include "deck.h"
#include "subject.h"
#include "player.h"
#include <vector>
#include <string>
#include <tuple>

class Model : public Subject {
public:
  Model();
  ~Model();

  // View getters
  std::vector<Card*> cardsInHand() const;
  std::vector<std::vector<Card*> > cardsOnTable() const;
  std::vector<int> points() const;
  std::vector<std::vector<Card*> > discards() const;
  std::vector<Card*> legalPlays() const;

  bool gameKilled() const;
  bool gameEnded() const;
  bool roundEnded() const;

  // returns (playerId, points); the playerId is 1 indexed
  std::vector<std::tuple<int,int> > winners() const;

  // manipulate model
  void rageQuit();

  // Tasks:
  //   - clear player's hand
  //   - clear player's discards
  //   - clear cards on the table
  //   - shuffle deck
  //   - redistribute cards
  //   - set active player to one with 7 of spades
  //   - notify view
  void startRound();

  // Tasks:
  //   - clear player's hand
  //   - clear player's discards
  //   - clear player's points
  //   - clear cards on the table
  //   - reset deck
  //   - shuffle deck
  //   - distribute cards and set active player to one with 7 of spades
  //   - notify view
  void startGame(int seed);

  // Tasks:
  //   - clear player vector
  //   - delete deck and create a new one
  //   - clear cards on the table
  //   - reset seed to 0
  //   - reset activePlayerId to 0
  //   - notify user
  void endGame();

  // Tasks:
  //   - clear all private state
  //   - set gameKilled to true
  //   - notify user
  void killGame();

  void addPlayer(std::string type);
  void activePlayerId(int id);
  void selectCard(Card* card);

  // public accessors
  int activePlayerId() const;
private:
  
  // Tasks:
  //   - clear the discards and hands of all players
  //   - if hardReset is true, the players' points are reset too
  void resetPlayers(bool hardReset = false);
  void clearCardsOnTable();
  void shuffleDeck();

  // Tasks:
  //   - using the sorted deck copy, we'll sort the current deck
  void unshuffleDeck();
  void dealCardsToPlayers();
  bool hasWinner() const;

  // Tasks:
  //   - increments activePlayerId_
  //   - if the next player is computer, we make him play his turn
  //     and go to the next player
  void nextPlayer();

  // given the hand, what are the legal plays?
  std::vector<Card*> legalPlaysInHand(std::vector<Card*> hand) const;

  // helper accessors
  Player* activePlayer() const;
  int seed() const;
  Deck* deck() const;

  // helper mutators
  void activePlayer(Player* p);
  void decrementActivePlayerId();
  void incrementActivePlayerId();
  void seed(int seed);
  void calculateWinners();
  void resetPrivates();

  // Privates: private members
  std::vector<Player*> players_;
  std::vector<std::vector<Card*> > cardsOnTable_;
  int activePlayerId_;
  Deck* deck_;
  int seed_;

  // state members
  bool gameEnded_;
  bool gameKilled_;
  bool roundEnded_;

  // defaults to an empty vector in the beginning
  std::vector<std::tuple<int, int> > winners_;
};

#endif
