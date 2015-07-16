#include "controller.h"
#include "model.h"

Controller::Controller(Model* m) : model_(m) {}
Controller::~Controller(){}

void Controller::startRound(int seed) {
  this->model()->resetGame(seed);
}

Model* Controller::model() const{
  return this->model_;
}

void Controller::selectCard(Card* card) {
  this->model()->activePlayerSelectCard(card);
}

void Controller::quit() {}
void Controller::rageQuit() {}
