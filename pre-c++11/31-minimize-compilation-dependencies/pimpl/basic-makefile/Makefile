
CXX = g++ -std=c++17 
CXXFLAGS = -Wall
OBJS = main.o address.o date.o person.o personimpl.o
EXE = a.out

$(EXE): $(OBJS)
	g++ -o $(EXE) $(OBJS)

#	One *.o per *.cpp
#	Dependencies as specified by 'g++ -MM *.cpp'
#	Note that only one 'cpp' source file is a dependency (the others items are all headers)
main.o: main.cpp date.h address.h person.h	# <(if we only use headers, do we only need to recompile when headers change)>
person.o: person.cpp person.h personimpl.h date.h address.h
personimpl.o: personimpl.cpp personimpl.h date.h address.h
address.o: address.cpp address.h
date.o: date.cpp date.h

clean:
	rm -f $(EXE) *.o

