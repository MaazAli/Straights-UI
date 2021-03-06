CXX = g++ -std=c++0x
CXXFLAGS = -Wall -MMD -O -g `pkg-config gtkmm-2.4 --cflags --libs`
OBJECTS = main.o card.o command.o computer.o deck.o human.o player.o model.o controller.o observer.o subject.o deckgui.o view.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = straights

${EXEC} : ${OBJECTS}
	${CXX} ${OBJECTS} ${CXXFLAGS} -o ${EXEC}

clean :
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}
