/*
 * MVC example of GTKmm program
 *
 * View class.  Is responsible for buttons (that user clicks) and for displaying
 * the top card of the deck.
 *
 *  Created by Jo Atlee on 06/07/09.
 *  Copyright 2009 UW. All rights reserved.
 *
 */
#include "view.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "card.h"
#include "observer.h"
#include <algorithm>

// Creates buttons with labels. Sets butBox elements to have the same size,
// with 10 pixels between widgets
View::View(Controller *c, Model *m) :
  model_(m),
  controller_(c),
  sections(true, 10),
  gameControlSection(false, 10),
  playerSection(true, 10),
  cardsOnTheTableGrid(true, 10),
    cardsOnTheTableHearts(true, 10),
    cardsOnTheTableDiamonds(true, 10),
    cardsOnTheTableClubs(true, 10),
    cardsOnTheTableSpades(true, 10),
  player1Box(true, 10),
  player2Box(true, 10),
  player3Box(true, 10),
  player4Box(true, 10),
  player1Button("Human"),
  player2Button("Human"),
  player3Button("Human"),
  player4Button("Human"),
  points1("Points: 0"),
  points2("Points: 0"),
  points3("Points: 0"),
  points4("Points: 0"),
  discards1("Discards: 0"),
  discards2("Discards: 0"),
  discards3("Discards: 0"),
  discards4("Discards: 0"),
  startGameButton( "Start New Game" ),
  endCurrentGameButton( "End Current Game" )

{


  // const Glib::RefPtr<Gdk::Pixbuf> nullCardPixbuf = deck.getNullCardImage();
	// const Glib::RefPtr<Gdk::Pixbuf> cardPixbuf     = deck.getCardImage( TEN, SPADE );

	// Sets some properties of the window.
  set_title( "Straights UI" );
  set_size_request(800, 600);
  set_border_width( 10 );



	// Add sections to the window
	add(sections);

  sections.add(gameControlSection);
    gameControlSection.set_size_request(40, 100);
    gameControlSection.add(startGameButton);
    gameControlSection.add(seedInput);
      seedInput.set_max_length(10);
      seedInput.set_text("0");
    gameControlSection.add(endCurrentGameButton);

  sections.add(playerSection);
    playerSection.add(player1Frame);
      player1Frame.set_label("Player 1");
      player1Frame.add(player1Box);
        player1Box.add(player1Button);
        player1Box.add(points1);
        player1Box.add(discards1);

    playerSection.add(player2Frame);
      player2Frame.set_label("Player 2");
      player2Frame.add(player2Box);
        player2Box.add(player2Button);
        player2Box.add(points2);
        player2Box.add(discards2);

    playerSection.add(player3Frame);
      player3Frame.set_label("Player 3");
      player3Frame.add(player3Box);
        player3Box.add(player3Button);
        player3Box.add(points3);
        player3Box.add(discards3);

    playerSection.add(player4Frame);
      player4Frame.set_label("Player 4");
      player4Frame.add(player4Box);
        player4Box.add(player4Button);
        player4Box.add(points4);
        player4Box.add(discards4);

  sections.add(yourHandSection);
    yourHandSection.add(yourHandGrid);
    yourHandSection.set_label("Your hand:");
      // Add all cards to the current player's hand
      for(int i = 0; i < 13; i++) {
        cardButtons[i] = new Gtk::Button();
        handCards[i] = new Gtk::Image(deck.getNullCardImage());
        cardButtons[i]->set_image(*handCards[i]);
        cardButtons[i]->set_sensitive(false);
        yourHandGrid.add( *cardButtons[i] );
      }

  sections.add(cardsOnTheTableSection);
    cardsOnTheTableSection.set_label("Cards on the table:");
      cardsOnTheTableSection.add(cardsOnTheTableGrid);
        cardsOnTheTableGrid.add(cardsOnTheTableClubs);
        cardsOnTheTableGrid.add(cardsOnTheTableDiamonds);
        cardsOnTheTableGrid.add(cardsOnTheTableHearts);
        cardsOnTheTableGrid.add(cardsOnTheTableSpades);
          // Add all clubs as null images
          for(int i = 0; i < 13; i++) {
            card[i] = new Gtk::Image(deck.getNullCardImage());
            cardsOnTheTableClubs.add( *card[i] );
          }
          // Add all diamonds as null images
          for(int i = 13; i < 26; i++) {
            card[i] = new Gtk::Image(deck.getNullCardImage());
            cardsOnTheTableDiamonds.add( *card[i] );
          }
          // Add all hearts as null images
          for(int i = 26; i < 39; i++) {
            card[i] = new Gtk::Image(deck.getNullCardImage());
            cardsOnTheTableHearts.add( *card[i] );
          }
          // Add all spades as null images
          for(int i = 39; i < 52; i++) {
            card[i] = new Gtk::Image(deck.getNullCardImage());
            cardsOnTheTableSpades.add( *card[i] );
          }


	// Associate button "clicked" events with local onButtonClicked() method defined below.
	startGameButton.signal_clicked().connect( sigc::mem_fun( *this, &View::startGameButtonClicked ) );
	endCurrentGameButton.signal_clicked().connect( sigc::mem_fun( *this, &View::endCurrentGameButtonClicked ) );

  player1Button.signal_clicked().connect( sigc::mem_fun( *this, &View::player1ButtonClicked ) );
  player2Button.signal_clicked().connect( sigc::mem_fun( *this, &View::player2ButtonClicked ) );
  player3Button.signal_clicked().connect( sigc::mem_fun( *this, &View::player3ButtonClicked ) );
  player4Button.signal_clicked().connect( sigc::mem_fun( *this, &View::player4ButtonClicked ) );

  for(int i = 0; i < 13; ++i) {
    cardButtons[i]->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &View::cardButtonClicked), i));
  }

	// The final step is to display the buttons (they display themselves)
	show_all();

	// Register view as observer of model
	model_->subscribe(this);

} // View::View

View::~View() {}

/////////////////////////////////////////////////////////
//////////////// Signal Handlers ////////////////////////
/////////////////////////////////////////////////////////

void View::startGameButtonClicked() {
  std::stringstream dialogMsg;
  int seed = (int)std::strtol(this->seedInput.get_text().c_str(), NULL, 10);
  std::string type1 = this->player1Button.get_label();
  std::string type2 = this->player2Button.get_label();
  std::string type3 = this->player3Button.get_label();
  std::string type4 = this->player4Button.get_label();

  std::vector<std::string> playerTypes;

  playerTypes.push_back(type1);
  playerTypes.push_back(type2);
  playerTypes.push_back(type3);
  playerTypes.push_back(type4);

  // Tell the controller to initialize the game
  this->controller_->initGame(seed, playerTypes);

  dialogMsg << "It is player " << (this->model_->activePlayerId() + 1) << "'s turn";

  Gtk::MessageDialog dialog(*this, "Game has started!");
  dialog.set_secondary_text(dialogMsg.str());
  dialog.run();

  // Update the player specific buttons to rage
  this->player1Button.set_label("RAGE!!!");
  this->player2Button.set_label("RAGE!!!");
  this->player3Button.set_label("RAGE!!!");
  this->player4Button.set_label("RAGE!!!");

} // View::nextButtonClicked

void View::endCurrentGameButtonClicked() {
  // Quit the game, that should reset state.
  controller_->quit();
} // View::resetButtonClicked

void View::player1ButtonClicked() {
  std::string label = this->player1Button.get_label();
  if (label == "Human") {
    this->player1Button.set_label("Computer");
  } else if (label == "Computer") {
    this->player1Button.set_label("Human");
  } else if (label == "RAGE!!!") {
    this->controller_->rageQuit();
  }
}
void View::player2ButtonClicked() {
  std::string label = this->player2Button.get_label();
  if (label == "Human") {
    this->player2Button.set_label("Computer");
  } else if (label == "Computer") {
    this->player2Button.set_label("Human");
  } else if (label == "RAGE!!!") {
    this->controller_->rageQuit();
  }
}
void View::player3ButtonClicked() {
  std::string label = this->player3Button.get_label();
  if (label == "Human") {
    this->player3Button.set_label("Computer");
  } else if (label == "Computer") {
    this->player3Button.set_label("Human");
  } else if (label == "RAGE!!!") {
    this->controller_->rageQuit();
  }
}
void View::player4ButtonClicked() {
  std::string label = this->player4Button.get_label();
  if (label == "Human") {
    this->player4Button.set_label("Computer");
  } else if (label == "Computer") {
    this->player4Button.set_label("Human");
  } else if (label == "RAGE!!!") {
    this->controller_->rageQuit();
  }
}

void View::cardButtonClicked(int index) {
  std::vector<Card*> cardsInHand = this->model_->cardsInHand();
  this->controller_->selectCard(cardsInHand[index]);
}

void View::update() {

  // Grab state from the model
  std::vector<Card*> cardsInHand = this->model_->cardsInHand();
  std::vector< std::vector< Card* > > cardsOnTable = this->model_->cardsOnTable();
  std::vector<int> points;
  std::vector<std::vector<Card*> > discards;
  std::vector<Card*> legalPlays;
  bool gameEnded = this->model_->gameEnded();
  bool roundEnded = this->model_->roundEnded();

  // Round ended, display dialog box with some statistics and prompt
  // user to start a new round
  if (roundEnded) {

    this->controller_->startRound();
  }


  if (!gameEnded) {
     points = this->model_->points();
     discards = this->model_->discards();
     legalPlays = this->model_->legalPlays();
  }


  // Update the cards in hand
  for (int i = 0; i < 13; i++) {
    auto cardImage = deck.getNullCardImage();
    cardButtons[i]->set_sensitive(false);
    if (i < cardsInHand.size()) {
      cardImage = deck.getCardImage(cardsInHand[i]->getRank(), cardsInHand[i]->getSuit());
      if (legalPlays.size() == 0 || std::find(legalPlays.begin(), legalPlays.end(), cardsInHand[i]) != legalPlays.end()) {
        cardButtons[i]->set_sensitive(true);
      }
    }
    this->handCards[i]->set(cardImage);
  }

  // Update the cards on table
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 13; j++) {
      if (cardsOnTable[i][j] == NULL) {
        this->card[i*13 + j]->set(this->deck.getNullCardImage());
      } else {
        this->card[i*13 + j]->set(this->deck.getCardImage(j, i));
      }
    }
  }

  // Update player points, in real time.. kinda.
  this->points1.set_label(View::intWithString("Points: ", (gameEnded) ? 0 : points[0]));
  this->points2.set_label(View::intWithString("Points: ", (gameEnded) ? 0 : points[1]));
  this->points3.set_label(View::intWithString("Points: ", (gameEnded) ? 0 : points[2]));
  this->points4.set_label(View::intWithString("Points: ", (gameEnded) ? 0 : points[3]));

  // Update player discard count, in real time... kinda
  this->discards1.set_label(View::intWithString("Discards: ", (gameEnded) ? 0 : discards[0].size()));
  this->discards2.set_label(View::intWithString("Discards: ", (gameEnded) ? 0 : discards[1].size()));
  this->discards3.set_label(View::intWithString("Discards: ", (gameEnded) ? 0 : discards[2].size()));
  this->discards4.set_label(View::intWithString("Discards: ", (gameEnded) ? 0 : discards[3].size()));

  // If game has ended, we want to bring in some buttons to their default state
  if (gameEnded) {
    this->player1Button.set_label("Human");
    this->player2Button.set_label("Human");
    this->player3Button.set_label("Human");
    this->player4Button.set_label("Human");

    this->player1Button.set_sensitive(true);
    this->player2Button.set_sensitive(true);
    this->player3Button.set_sensitive(true);
    this->player4Button.set_sensitive(true);

  } else {

    this->player1Button.set_sensitive(false);
    this->player2Button.set_sensitive(false);
    this->player3Button.set_sensitive(false);
    this->player4Button.set_sensitive(false);

    // Refactor: Do this in a better way
    if (this->model_->activePlayerId() == 0) {
      this->player1Button.set_sensitive(true);
    } else if (this->model_->activePlayerId() == 1) {
      this->player2Button.set_sensitive(true);
    } else if (this->model_->activePlayerId() == 2) {
      this->player3Button.set_sensitive(true);
    } else {
      this->player4Button.set_sensitive(true);
    }
  }
  std::cout << "stuff was updated?" << std::endl;
}

// HELPER FUNCTION
std::string View::intWithString(std::string type, int val) {
  std::stringstream ss;
  ss << type << val;
  return ss.str();
}
