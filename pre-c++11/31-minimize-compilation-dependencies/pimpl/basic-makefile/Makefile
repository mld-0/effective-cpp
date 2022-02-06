
CXX = g++ -std=c++17 
CXXFLAGS = -Wall
OBJS = main.o address.o date.o person.o personimpl.o
EXE = a.out

$(EXE): $(OBJS)
	g++ -o $(EXE) $(OBJS)

#main.o: main.cpp 
#person.o: person.cpp 
#personimpl.o: personimpl.cpp 
#address.o: address.cpp 
#date.o: date.cpp 

main.o: main.cpp date.h address.h person.h
person.o: person.cpp person.h personimpl.h date.h address.h
personimpl.o: personimpl.cpp personimpl.h date.h address.h
address.o: address.cpp address.h
date.o: date.cpp date.h

clean:
	rm -f $(EXE) *.o

