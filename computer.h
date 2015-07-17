#ifndef COMPUTER_H
#define COMPUTER_H
#include "player.h"
#include "human.h"

class Computer : public Player {
public:
  void takeOver(Human* player);
  Card* play(std::vector<Card*> legalPlays);
  virtual std::string type() const;
private:
};

#endif
