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
  this->model()->selectCard(card);
}

void Controller::initGame(int seed, std::vector<std::string> types) {
  for (auto it = types.begin(); it != types.end(); it++) {
    this->model()->addPlayer(*it);
  }
  this->model()->startGame(seed);
}

void Controller::quit() {
  this->model()->endGame();
}

void Controller::rageQuit() {
  this->model()->rageQuit();
}
