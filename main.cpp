#include "controller.h"
#include "card.h"
#include "model.h"
#include "command.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <cassert>

#include <gtkmm/main.h>
#include "view.h"

int main( int argc, char * argv[] ) {
	Gtk::Main  kit( argc, argv );         // Initialize gtkmm with the command line arguments, as appropriate.
	Model model;                          // Create model
  Controller controller( &model );      // Create controller
	View view(&controller, &model);       // Create the view -- is passed handle to controller and model
	Gtk::Main::run( view );               // Show the window and return when it is closed.

	return 0;
} // main
