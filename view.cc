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
  player1TypeButton("Human"),
  player2TypeButton("Human"),
  player3TypeButton("Human"),
  player4TypeButton("Human"),
  rage1Button("RAGE!!!"),
  rage2Button("RAGE!!!"),
  rage3Button("RAGE!!!"),
  rage4Button("RAGE!!!"),
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
  // set_size_request(200, 100);
	set_default_size(800, 100);
  set_border_width( 10 );



	// Add sections to the window
	add(sections);

  sections.add(gameControlSection);
    gameControlSection.add(startGameButton);
    gameControlSection.add(seedInput);
      seedInput.set_max_length(10);
      seedInput.set_text("0");
    gameControlSection.add(endCurrentGameButton);

  sections.add(playerSection);
    playerSection.add(player1Frame);
      player1Frame.set_label("Player 1");
      player1Frame.add(player1Box);
        player1Box.add(player1TypeButton);
        player1Box.add(points1);
        player1Box.add(discards1);

    playerSection.add(player2Frame);
      player2Frame.set_label("Player 2");
      player2Frame.add(player2Box);
        player2Box.add(player2TypeButton);
        player2Box.add(points2);
        player2Box.add(discards2);

    playerSection.add(player3Frame);
      player3Frame.set_label("Player 3");
      player3Frame.add(player3Box);
        player3Box.add(player3TypeButton);
        player3Box.add(points3);
        player3Box.add(discards3);

    playerSection.add(player4Frame);
      player4Frame.set_label("Player 4");
      player4Frame.add(player4Box);
        player4Box.add(player4TypeButton);
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

  rage1Button.signal_clicked().connect( sigc::mem_fun( *this, &View::rage1ButtonClicked ) );
  rage2Button.signal_clicked().connect( sigc::mem_fun( *this, &View::rage2ButtonClicked ) );
  rage3Button.signal_clicked().connect( sigc::mem_fun( *this, &View::rage3ButtonClicked ) );
  rage4Button.signal_clicked().connect( sigc::mem_fun( *this, &View::rage4ButtonClicked ) );

  player1TypeButton.signal_clicked().connect( sigc::mem_fun( *this, &View::player1TypeButtonClicked ) );
  player2TypeButton.signal_clicked().connect( sigc::mem_fun( *this, &View::player2TypeButtonClicked ) );
  player3TypeButton.signal_clicked().connect( sigc::mem_fun( *this, &View::player3TypeButtonClicked ) );
  player4TypeButton.signal_clicked().connect( sigc::mem_fun( *this, &View::player4TypeButtonClicked ) );



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
  int seed = (int)std::strtol(this->seedInput.get_text().c_str(), NULL, 10);
  std::string type1 = this->player1TypeButton.get_label();
  std::string type2 = this->player1TypeButton.get_label();
  std::string type3 = this->player1TypeButton.get_label();
  std::string type4 = this->player1TypeButton.get_label();

  std::vector<std::string> playerTypes;

  playerTypes.push_back(type1);
  playerTypes.push_back(type2);
  playerTypes.push_back(type3);
  playerTypes.push_back(type4);

  this->controller_->initGame(seed, playerTypes);

  std::vector<Card*> cardsInHand = this->model_->cardsInHand();

  for (int i = 0; i < cardsInHand.size(); i++) {
    this->handCards[i]->set(deck.getCardImage(cardsInHand[i]->getRank(), cardsInHand[i]->getSuit()));
  }
} // View::nextButtonClicked

void View::endCurrentGameButtonClicked() {
  std::cout << "end current game bois" << std::endl;
  // controller_->resetButtonClicked();
} // View::resetButtonClicked

void View::rage1ButtonClicked() {
  std::stringstream ss;
  ss << "Points: " <<  1;
  this->points1.set_text(ss.str());
  this->card[0]->set(this->deck.getCardImage(0, 1));
  std::cout << "Player 1 tried to rage quit" << std::endl;

}

void View::rage2ButtonClicked() {
  std::cout << "Player 2 tried to rage quit" << std::endl;
}

void View::rage3ButtonClicked() {
  std::cout << "Player 3 tried to rage quit" << std::endl;
}

void View::rage4ButtonClicked() {
  std::cout << "Player 4 tried to rage quit" << std::endl;
}

void View::player1TypeButtonClicked() {
  if (this->player1TypeButton.get_label() == "Human") {
    this->player1TypeButton.set_label("Computer");
  } else {
    this->player1TypeButton.set_label("Human");
  }
}
void View::player2TypeButtonClicked() {
  if (this->player2TypeButton.get_label() == "Human") {
    this->player2TypeButton.set_label("Computer");
  } else {
    this->player2TypeButton.set_label("Human");
  }
}
void View::player3TypeButtonClicked() {
  if (this->player3TypeButton.get_label() == "Human") {
    this->player3TypeButton.set_label("Computer");
  } else {
    this->player3TypeButton.set_label("Human");
  }
}
void View::player4TypeButtonClicked() {
  if (this->player4TypeButton.get_label() == "Human") {
    this->player4TypeButton.set_label("Computer");
  } else {
    this->player4TypeButton.set_label("Human");
  }
}

void View::update() {
  std::cout << "Shit was updated?" << std::endl;
}