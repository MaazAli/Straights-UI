#include "model.h"
#include "player.h"
#include "human.h"
#include "computer.h"
#include <vector>
#include <string>

Model::Model() : deck_(new Deck{}), seed_(0) {
    for (int i = 0; i < 4; i++) {
        this->cardsOnTable_.push_back(std::vector<Card*>(13, NULL));
    }
}

Model::~Model() {
  delete this->deck_;
}

// View Getters: these return copies of internal data structures!
std::vector<Card*> Model::cardsInHand(){
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


// manipulate model
void Model::rageQuitActivePlayer() {
  Human* human = (Human*)this->activePlayer();
  Computer* computer = new Computer();

  // copy human's hand, points, and discards
  computer->takeOver(human);

  // replace human in game
  this->activePlayer((Player*)computer);

  // no need for human
  delete human;

  // play computer's move
  computer->play(this->legalPlaysInHand(computer->hand()));
}

void Model::resetGame(int seed) {
  // reset player's hand/discards
  // clear cards on the table
  // reset card order to default, and shuffle the deck
  // deal cards: player i gets [(i - 1) * 13, i * 13 - 1] cards
  // Debug::log("About to reset players.");
  this->resetPlayers();
  this->clearCardsOnTable();
  this->shuffleDeck(seed);
  this->dealCardsToPlayers();
}

void Model::addPlayer(std::string type) {
  if (type == "h") {
    this->players_.push_back(new Human{});
  } else if (type == "c") {
    this->players_.push_back(new Computer{});
  }
}

void Model::activePlayerId(int id) {
  this->activePlayerId_ = id;
}

// precondition:
//   - card is in the hand
//   - card is on the heap and can be accessed using the deck
void Model::activePlayerSelectCard(Card* card) {
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
}

void Model::seed(int seed) {
  this->seed_ = seed;
}

// Private Methods
void Model::resetPlayers() {
  std::vector<Player*>& players = this->players_;
  for (auto it = players.begin(); it != players.end(); it++) {
    (*it)->cleanup();
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

void Model::shuffleDeck(int seed) {
  this->deck()->shuffle(seed);
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


