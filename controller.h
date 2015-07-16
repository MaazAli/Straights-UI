#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "card.h"
#include "model.h"
#include <vector>

class Controller {
public:
  Controller(Model*);
  virtual ~Controller();
  
  void selectCard(Card* card);
  void initGame(int seed, std::vector<std::string> types);
  void startRound(int seed);
  void quit();

  // rage-quits the current player
  void rageQuit();
private:
  Model* model_;
  Model* model() const;
};

#endif
