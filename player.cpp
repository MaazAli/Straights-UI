#include "card.h"
#include "player.h"
#include <vector>

// nothing special
Player::Player() : points_(0) {}
Player::~Player() {}

// getters
int Player::points() const {
  return this->points_;
}

// accessors for the model
std::vector<Card*> Player::discards() const {
  return this->discards_;
}

std::vector<Card*> Player::hand() const {
  return this->hand_;
}

// playing a card just removes it from the player's hand
void Player::playCard(Card* card) {
  this->removeFromHand(card);
}

// NOTE: the hand accessor returns a new vector!
void Player::takeCard(Card* card) {
  this->hand_.push_back(card);
}

// discarding it removes it from the player's hand and adds it to his discards
// also updates his points
void Player::discardCard(Card* card) {
  this->removeFromHand(card);
  this->discards_.push_back(card);

  // update points
  this->points_ += card->getRank() + 1; 
}

void Player::cleanup() {
  this->hand_.clear();
  this->discards_.clear();
}

void Player::removeFromHand(Card* card) {
  for (auto it = this->hand_.begin(); it != this->hand_.end(); it++) {
    if (*(*it) == (*card)) {
      this->hand_.erase(it);
      return;
    }
  }
}

// protected setters that enable a computer to take over a human
void Player::points(int p) {
  this->points_ = p;
}

void Player::discards(std::vector<Card*>& cards) {
  this->discards_ = cards;
}

void Player::hand(std::vector<Card*>& cards) {
  this->hand_ = cards;
}
