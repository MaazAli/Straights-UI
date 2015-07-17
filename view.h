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


#ifndef MVC_VIEW_H
#define MVC_VIEW_H

#include <gtkmm.h>
#include "deckgui.h"
#include "controller.h"
#include "model.h"
#include "observer.h"

class View : public Gtk::Window, public Observer {
public:
        View(Controller*, Model*);
	virtual ~View();
	virtual void update();	// Observer Pattern: concrete update() method

private:

  // Observer Pattern: to access Model accessors without having to downcast subject
	Model *model_;

	// Strategy Pattern member (plus signal handlers)
	Controller *controller_;


  DeckGUI deck;

	// Member widgets:
  Gtk::VBox sections;    // Main window divided into sections

  Gtk::HBox gameControlSection;
    Gtk::Button startGameButton;
    Gtk::Entry seedInput;
    Gtk::Button endCurrentGameButton;

  Gtk::HBox playerSection;
    Gtk::Frame player1Frame;
      Gtk::VBox player1Box;
        Gtk::Label points1;
        Gtk::Label discards1;
        Gtk::Button player1Button;
    Gtk::Frame player2Frame;
      Gtk::VBox player2Box;
        Gtk::Label points2;
        Gtk::Label discards2;
        Gtk::Button player2Button;
    Gtk::Frame player3Frame;
      Gtk::VBox player3Box;
        Gtk::Label points3;
        Gtk::Label discards3;
        Gtk::Button player3Button;
    Gtk::Frame player4Frame;
      Gtk::VBox player4Box;
        Gtk::Label points4;
        Gtk::Label discards4;
        Gtk::Button player4Button;

  Gtk::Frame yourHandSection;
    Gtk::HBox yourHandGrid;
      Gtk::Button *cardButtons[13];
      Gtk::Image *handCards[13];


  Gtk::Frame cardsOnTheTableSection;
    Gtk::VBox cardsOnTheTableGrid;
      Gtk::HBox cardsOnTheTableHearts;
      Gtk::HBox cardsOnTheTableDiamonds;
      Gtk::HBox cardsOnTheTableClubs;
      Gtk::HBox cardsOnTheTableSpades;
      Gtk::Image *card[52];



	// Signal handlers:
	void startGameButtonClicked();
	void endCurrentGameButtonClicked();

  void player1ButtonClicked();
  void player2ButtonClicked();
  void player3ButtonClicked();
  void player4ButtonClicked();


  void cardButtonClicked(int index);

  static std::string intWithString(std::string, int);
  void roundEndedDialog();
  void gameEndedDialog();

}; // View

#endif
