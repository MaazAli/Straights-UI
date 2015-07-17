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
  std::vector<Card*> cardsInHand() const;
  std::vector<std::vector<Card*> > cardsOnTable() const;
  std::vector<int> points() const;
  std::vector<std::vector<Card*> > discards() const;
  std::vector<Card*> legalPlays() const;
  
  bool gameEnded() const;
  bool roundEnded() const;

  // get the id of player with 80+ points; -1 if no winner
  int winner() const;

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


  void addPlayer(std::string type);
  void activePlayerId(int id);
  void selectCard(Card* card);

  // temporary public method
  // std::vector<Card*> legalPlaysInHand(std::vector<Card*> hand) const;

  // public accessors
  int activePlayerId() const;
private:
  
  // class that helps record previous games
  // class Game {
  // public:
  //   Game(std::vector<int> points, std::vector<std::vector<Card*> > discards);
  //   std::vector<int> points() const;
  //   std::vector<std::vector<Card*> > discards() const;
  // private:
  //   std::vector<int> points_;
  //   std::vector<std::vector<Card*> > discards_;
  // };
  
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

  // increments activePlayerId_
  // if the next player is computer, we make him play his turn and go to the next player
  void nextPlayer();

  // given the hand, what are the legal plays?
  // ehhh, introduces coupling between computer player and model class
  std::vector<Card*> legalPlaysInHand(std::vector<Card*> hand) const;

  // helper accessors
  Player* activePlayer() const;
  int seed() const;
  Deck* deck() const;

  // helper mutators
  //   replaces the active player with p
  void activePlayer(Player* p);
  void decrementActivePlayerId();
  void incrementActivePlayerId();
  void seed(int seed);

  // private members
  std::vector<Player*> players_;
  std::vector<std::vector<Card*> > cardsOnTable_;
  int activePlayerId_;
  Deck* deck_;
  int seed_;

  // state members
  bool gameEnded_;
  bool roundEnded_;
  // std::vector<Game> gameHistory_;
};

#endif
