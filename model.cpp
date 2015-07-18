#include "model.h"
#include "player.h"
#include "human.h"
#include "computer.h"
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <tuple>

Model::Model() : deck_(new Deck{}), seed_(0) {
    for (int i = 0; i < 4; i++) {
        this->cardsOnTable_.push_back(std::vector<Card*>(13, NULL));
    }
}

Model::~Model() {
  delete this->deck_;
}

// View Getters:
//   - these return copies of internal data structures!
std::vector<Card*> Model::cardsInHand() const {
  if (this->players_.size() == 0) {
    return std::vector<Card*>();
  }

  return this->activePlayer()->hand();
}

std::vector<std::vector<Card*> > Model::cardsOnTable() const {
  return this->cardsOnTable_;
}

std::vector<int> Model::points() const {
  std::vector<int> points;
  const std::vector<Player*>& players = this->players_;

  assert(players.size() == 4);

  for (auto it = players.begin(); it != players.end(); it++) {
    points.push_back((*it)->points());
  }

  return points;
}

std::vector<std::vector<Card*> > Model::discards() const {
  std::vector<std::vector<Card*> > discards;
  const std::vector<Player*>& players = this->players_;

  assert(players.size() == 4);

  for (auto it = players.begin(); it != players.end(); it++) {
    discards.push_back((*it)->discards());
  }

  return discards;
}

std::vector<Card*> Model::legalPlays() const {
  return this->legalPlaysInHand(this->cardsInHand());
}

bool Model::gameKilled() const {
  return this->gameKilled_;
}

bool Model::gameEnded() const {
  return this->gameEnded_;
}

bool Model::roundEnded() const {
  return this->roundEnded_;
}

std::vector<std::tuple<int,int> > Model::winners() const {
  return this->winners_;
}

// manipulate model
void Model::rageQuit() {
  assert(this->players_.size() != 0);
  assert(this->activePlayer()->type() == "Human");

  Human* human = (Human*)this->activePlayer();
  Computer* computer = new Computer();

  // copy human's hand, points, and discards
  computer->takeOver(human);

  // replace human in game
  this->activePlayer((Player*)computer);

  // no need for human
  delete human;

  // try this player's turn again
  this->decrementActivePlayerId();
  this->nextPlayer();
}

void Model::startRound() {
  this->roundEnded_ = false;
  this->gameKilled_ = false;
  this->gameEnded_ = false;
  this->resetPlayers(false);
  this->clearCardsOnTable();
  this->shuffleDeck();
  this->dealCardsToPlayers();

  this->notify();

  if (this->activePlayer()->type() == "Computer") {
    this->decrementActivePlayerId();
    this->nextPlayer();
  }

}

void Model::startGame(int seed) {
  this->roundEnded_ = false;
  this->gameKilled_ = false;
  this->gameEnded_ = false;
  this->resetPlayers(true);
  this->clearCardsOnTable();
  this->unshuffleDeck();
  this->seed(seed);
  this->shuffleDeck();
  // sets active player too
  this->dealCardsToPlayers();

  this->notify();

  if (this->activePlayer()->type() == "Computer") {
    this->decrementActivePlayerId();
    this->nextPlayer();
  }
}

// if the game has ended normally,
// the round has also ended normally
void Model::endGame() {
  this->calculateWinners();
  this->resetPrivates();
  this->roundEnded_ = true;
  this->gameEnded_ = true;
  this->gameKilled_ = false;
  this->notify();
}

// user just prematurely kills the game
void Model::killGame() {
  this->resetPrivates();
  this->roundEnded_ = false;
  this->gameEnded_ = false;
  this->gameKilled_ = true;
  std::cerr << "Was able to kill the game properly" << std::endl;
  this->notify();
}

void Model::addPlayer(std::string type) {
  if (type == "Human") {
    this->players_.push_back(new Human{});
  } else if (type == "Computer") {
    this->players_.push_back(new Computer{});
  }
}

void Model::activePlayerId(int id) {
  this->activePlayerId_ = id;
}


// precondition:
//   - card is in the hand
//   - card is on the heap and can be accessed using the deck
void Model::selectCard(Card* card) {
  Player* activePlayer = this->activePlayer();
  std::vector<Card*> cardsInHand = activePlayer->hand();
  std::vector<Card*> legalPlays = this->legalPlaysInHand(cardsInHand);

  if (legalPlays.size() == 0) {
    // if there were no legal plays in our hand, discard the card
    activePlayer->discardCard(card);
  } else {

    // look for card inside legalPlays
    bool found = false;
    for (int i = 0; i < legalPlays.size(); i++) {
      if (*card == *legalPlays.at(i)) {
      	found = true;
      	break;
      }
    }

    if (found) {
      activePlayer->playCard(card);
      // card's been played, so we update table
      this->cardsOnTable_[card->getSuit()][card->getRank()] = card;
    } else {
      // throw exception because there were legal cards we
      // could play but we chose to play something illegal
      throw "Screwed up by trying to discard a card when you could play a legal card";
    }
  }

  // notify that things are done
  this->notify();

  // move on to the next player
  this->nextPlayer();
}

// Private Methods
void Model::resetPlayers(bool hardReset) {
  std::vector<Player*>& players = this->players_;
  for (auto it = players.begin(); it != players.end(); it++) {
    (*it)->cleanup();

    if (hardReset) {
      (*it)->clearPoints();
    }
  }
}

void Model::clearCardsOnTable() {
  // clear all the cards on the table
  auto& tableCards = this->cardsOnTable_;

  for (int i = 0; i < tableCards.size(); i++) {
    tableCards.at(i).clear();
    for (int j = 0; j < 13; j++) {
      tableCards.at(i).push_back(NULL);
    }
  }
}

void Model::shuffleDeck() {
  this->deck()->shuffle(this->seed());
}

void Model::unshuffleDeck() {
  this->deck()->unshuffle();
}

// distribute the deck's cards to players
void Model::dealCardsToPlayers() {
  Deck* deck = this->deck();
  Card startingCard{SPADE, SEVEN};

  for (int i = 0; i < 4; i++) {
    for (int j = 13 * i; j < 13 * (i + 1); j++) {
      Card& card = deck->cardAt(j);
      // we're figuring out which person should go first
      if (card == startingCard) {
	      this->activePlayerId(i);
      }
      this->players_.at(i)->takeCard(&card);
    }
  }
}

// Responsibilities:
//   - Return true if any player has more than 80 points,
//     false otherwise
bool Model::hasWinner() const{
  const std::vector<Player*>& players = this->players_;
  for (auto it = players.begin(); it != players.end(); it++) {
    if ((*it)->points() >= 80) {
      return true;
    }
  }
  return false;
}

/*
* TODO Figure out what happens when the player's hand is empty. Do we toggle something in the model (roundOver)? how does the view know?
*/

void Model::nextPlayer() {
  this->incrementActivePlayerId();
  Player* player = this->activePlayer();
  this->notify();

  // check if this player has any cards to play
  // if not, end the turn and notify
  if (player->hand().size() == 0) {
    std::cerr << "Setting round to ended" << std::endl;
    this->roundEnded_ = true;
    std::cerr << "Notifying user" << std::endl;
    this->notify();
    if (!this->hasWinner()) {
      std::cerr << "Game didn't have a winner, so we start another round" << std::endl;
      this->startRound();
    } else {
      std::cerr << "Game had a winner" << std::endl;
      this->endGame();
    }
    std::cerr << "Updating the view after either starting another round or ending the game" << std::endl;
    return;
  }

  if (player->type() == "Computer") {
    Computer* computer = (Computer*)player;
    Card* card = computer->play(this->legalPlaysInHand(computer->hand()));
    if (card != NULL) {
      this->cardsOnTable_[card->getSuit()][card->getRank()] = card;
    }
    this->notify();
    this->nextPlayer();
  }

}

// determine the legal plays in hand
std::vector<Card*> Model::legalPlaysInHand(std::vector<Card*> hand) const {
  std::vector<Card*> legalPlays;
  auto& cardsOnTable = this->cardsOnTable_;
  Card startingCard{SPADE, SEVEN};

  // If our hand contains a 7S we push that to legalPlays and return
  for (int i = 0; i < hand.size(); i++) {
    if ((*hand[i]) == startingCard) {
      legalPlays.push_back(hand[i]);
      return legalPlays;
    }
  }

  // System State:
  //   - There is no seven of spades in our hand, so we proceed as normal
  for(int i = 0; i < cardsOnTable.size(); i++) {
    int suit = i;
    for (int j = 0; j < cardsOnTable[i].size(); j++) {
      int rank = j;

      if (cardsOnTable[i][j] != NULL) {
	for(int k = 0; k < hand.size(); k++) {
	  Card* handCard = hand[k];
	  int handSuit = handCard->getSuit();
	  int handRank = handCard->getRank();

	  if (handSuit == suit && (handRank == (rank - 1) || handRank == (rank + 1))) {
	    legalPlays.push_back(handCard);
	  } else if (handRank == rank) {
	    legalPlays.push_back(handCard);
	  }
	}
      }
    }
  }

  return legalPlays;
}

// helper accessors
Deck* Model::deck() const {
  return this->deck_;
}

int Model::activePlayerId() const {
  return this->activePlayerId_;
}

Player* Model::activePlayer() const {
  return this->players_.at(this->activePlayerId());
}

int Model::seed() const {
  return this->seed_;
}

// helper mutators
void Model::activePlayer(Player* player) {
  this->players_[this->activePlayerId()] = player;
}

void Model::decrementActivePlayerId() {
  assert(this->players_.size() == 4);
  int id = this->activePlayerId() - 1;
  this->activePlayerId((id % 4 + 4) % 4);
}

void Model::incrementActivePlayerId() {
  assert(this->players_.size() == 4);
  int id = this->activePlayerId() + 1;
  this->activePlayerId((id % 4 + 4) % 4);
}

void Model::seed(int seed) {
  this->seed_ = seed;
}

void Model::calculateWinners() {
  assert(this->hasWinner());
  assert(this->players_.size() == 4);
  
  const std::vector<Player*>& players = this->players_;
  std::vector<std::tuple<int,int> > winners;
  
  int minPoints = players.at(0)->points();
  int minPlayerId = 0;
  winners.push_back(std::tuple<int, int>{minPlayerId + 1, minPoints});
  
  for (int i = minPlayerId + 1; i < players.size(); i++) {
    int points = players.at(i)->points();

    if (points < minPoints) {
      winners.clear();
      minPoints = points;
      minPlayerId = i;
    }

    if (points <= minPoints) {
      winners.push_back(std::tuple<int, int>{i + 1, points});
    }   
  }

  this->winners_ = winners;
}

void Model::resetPrivates() {
  // delete all the players from the heap
  std::vector<Player*>& players = this->players_;
  for (auto it = players.begin(); it != players.end(); it++) {
    delete *it;
  }

  this->players_.clear();

  // clear everything else
  this->unshuffleDeck();
  this->activePlayerId(0);
  this->seed(0);
  this->clearCardsOnTable();
}

