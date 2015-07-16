#include "controller.h"
#include "card.h"
#include "model.h"
#include "command.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <cassert>

void print(std::string prefix, std::vector<Card*> cards) {
  std::cout << prefix;

  for (auto it = cards.begin(); it != cards.end(); it++) {
    if (*it != NULL) {
      std::cout << " " << *(*it);
    }
  }

  std::cout << std::endl;
}

void print(std::string prefix, std::vector<std::vector<Card*> > cards) {
  std::vector<Card*> printCards;
  for (int i = 0; i < cards.size(); i++) {
    for (int j = 0; j < cards[i].size(); j++) {
      printCards.push_back(cards[i][j]);
    }
  }

  print(prefix, printCards);
}

int main(int argc, char* argv[]) {

  // get the seed to randomize the game
  int seed = 0;
  if (argc >= 2) {
    seed = (int)strtol(argv[1], NULL, 10);
  }

  Model model;
  Controller controller{&model};
  // assert (false);

  for (int i = 0; i < 4; i++) {
    std::cout
      << 	"Is player "
      << 	i + 1
      << 	" a human(h) or a computer(c)?"
      << 	std::endl
      << 	">";

    std::string type;
    std::cin >> type;

    // only valid input is allowed for the time being
    assert(type == "h" || type == "c");

    model.addPlayer(type);
  }

  // 	SYSTEM STATE: 	We've successfully inserted the players and
  //					instantiated the deck. We now start the rounds.
  //

  bool gameOver = false;

  for (int gameRound = 0; !gameOver; gameRound++) {


    //  This deals out the cards and figures out the active player
    controller.startRound(seed);

    // 	Print out the intro message regardless of if the user is a
    // 	computer or human.
    std::cout
      << "A new round begins. It's player "
      << model.activePlayerId() + 1 << "'s turn to play."
      << std::endl;

    // we make each player take turns until the a player has 0 cards
    for (int playerId = model.activePlayerId();
	 model.cardsInHand().size() != 0;
	 playerId = (playerId + 1) % 4) {
      
      // update the active player id in the model/controller
      model.activePlayerId(playerId);
      

	print("Cards on table: ", model.cardsOnTable());
	print("Cards in hand: ",  model.cardsInHand());
	print("Legal Plays: ", model.legalPlaysInHand(model.cardsInHand()));
	
	// gameController.displayCardsOnTheTable();
	// gameController.displayActivePlayersHand();
	// gameController.displayActivePlayersLegalPlays();
	
	bool continueSamePlayerInput = false;
	
	while(true) {
	  std::cout << ">";
	  
	  Command playerCommand;
	  std::cin >> playerCommand;

	  switch(playerCommand.type){
	  case PLAY: {
	    try {
	      Card* card = new Card(playerCommand.card.getSuit(), playerCommand.card.getRank());
	      model.activePlayerSelectCard(card);
	      std::cout << "Player "
			<< model.activePlayerId() + 1
			<< " plays "
			<< playerCommand.card
			<< std::endl;
	       
	      continueSamePlayerInput = false;
	    } catch (const char* ex) {
	      std::cout << "This is not a legal play." << std::endl;
	      continueSamePlayerInput = true;
	    }
	    
	    break;
	  }
	  case QUIT: {
	    // end the game
	    return 0;
	    break;
	  }
	  case RAGEQUIT: {
	    model.rageQuitActivePlayer();
	    break;
	  }
	  default: {
	  }
	  }

	  // Get out of the while true loop if we're done with the same player
	  if (continueSamePlayerInput == false) {
	    break;
	  }

	
      }

    }

  }

}
