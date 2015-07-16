#include "controller.h"
#include "model.h"

Controller::Controller(Model* m) : model_(m) {}
Controller::~Controller(){}

void Controller::startRound() {
  this->model()->startRound();
}

Model* Controller::model() const{
  return this->model_;
}

void Controller::selectCard(Card* card) {
  this->model()->activePlayerSelectCard(card);
}

void Controller::initGame(int seed, std::vector<std::string> types) {
  this->model()->seed(seed);
  for (auto it = types.begin(); it != types.end(); it++) {
    this->model()->addPlayer(*it);
  }
  this->startRound();
}

void Controller::quit() {}
void Controller::rageQuit() {}
