CXX = g++ -std=c++0x
CXXFLAGS = -g -Wall -MMD
OBJECTS = main.o card.o command.o computer.o deck.o human.o player.o model.o controller.o observer.o subject.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = straights

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

clean :
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}
