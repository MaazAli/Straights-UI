#include "model.h"
#include "player.h"
#include "human.h"
#include "computer.h"
#include <vector>
#include <string>
#include <iostream>

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
std::vector<Card*> Model::cardsInHand(){
  if (this->players_.size() == 0) {
    return std::vector<Card*>();
  }

  return this->activePlayer()->hand();
}

std::vector<std::vector<Card*> > Model::cardsOnTable() {
  return this->cardsOnTable_;
}

std::vector<int> Model::points() {
  std::vector<int> points;
  for (int i = 0; i < 4; i++) {
    points.push_back(this->players_.at(i)->points());
  }
  return points;
}

std::vector<std::vector<Card*> > Model::discards() {
  std::vector<std::vector<Card*> > discards;
  for (int i = 0; i < 4; i++) {
    discards.push_back(this->players_.at(i)->discards());
  }
  return discards;
}

bool Model::gameEnded() const {
  return this->gameEnded_;
}

// manipulate model
void Model::rageQuit() {
  Human* human = (Human*)this->activePlayer();
  Computer* computer = new Computer();

  // copy human's hand, points, and discards
  computer->takeOver(human);

  // replace human in game
  this->activePlayer((Player*)computer);

  // no need for human
  delete human;

  // try this player's turn again
  this->activePlayerId((this->activePlayerId() - 1)%4);
  this->nextPlayer();
}

void Model::startRound() {

  this->resetPlayers(false);
  this->clearCardsOnTable();
  this->shuffleDeck();
  this->dealCardsToPlayers();

  this->notify();

  if (this->activePlayer()->type() == "Computer") {
    this->activePlayerId((this->activePlayerId() - 1) %4);
    this->nextPlayer();
  }

}

void Model::startGame(int seed) {
  this->gameEnded_ = false;
  this->resetPlayers(true);
  this->clearCardsOnTable();

  for (int i = 0; i < 52; i++) {
    std::cout << this->deck_->cardAt(i) << " ";
  }
  std::cout << std::endl;

  this->unshuffleDeck();

  for (int i = 0; i < 52; i++) {
    std::cout << this->deck_->cardAt(i) << " ";
  }
  std::cout << std::endl;

  this->seed(seed);
  this->shuffleDeck();

  for (int i = 0; i < 52; i++) {
    std::cout << this->deck_->cardAt(i) << " ";
  }
  std::cout << std::endl;

  // sets active player too
  this->dealCardsToPlayers();

  this->notify();

  if (this->activePlayer()->type() == "Computer") {
    this->activePlayerId((this->activePlayerId() - 1) %4);
    this->nextPlayer();
  }


}

void Model::endGame() {

  // delete all the players
  std::vector<Player*>& players = this->players_;
  for (auto it = players.begin(); it != players.end(); it++) {
    delete *it;
  }

  this->players_.clear();

  // everything else
  this->unshuffleDeck();
  this->activePlayerId(0);
  this->seed(0);
  this->clearCardsOnTable();
  this->gameEnded_ = true;
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

void Model::seed(int seed) {
  this->seed_ = seed;
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

/*
* TODO Figure out what happens when the player's hand is empty. Do we toggle something in the model (roundOver)? how does the view know?
*/

void Model::nextPlayer() {
  this->activePlayerId((this->activePlayerId() + 1)% 4);
  Player* player = this->activePlayer();

  // check if this player has any cards to play
  // if not, end the turn and notify
  if (player->hand().size() == 0) {

    this->notify();
    return;
  }

  if (player->type() == "Computer") {
    Computer* computer = (Computer*)player;
    Card* card = computer->play(this->legalPlaysInHand(computer->hand()));
    if (card != NULL) {
      std::cout << *(card) << std::endl;
      this->cardsOnTable_[card->getSuit()][card->getRank()] = card;
    }
    std::cout << "SUP " << std::endl;
    this->notify();
    std::cout << "SUP" << std::endl;
    this->nextPlayer();
  }

  this->notify();
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
