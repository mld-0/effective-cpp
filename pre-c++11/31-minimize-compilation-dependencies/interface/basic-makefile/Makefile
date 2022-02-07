
CXX = g++ -std=c++17 
CXXFLAGS = -Wall
OBJS = main.o address.o date.o personconcrete.o personinterface.o
EXE = a.out

$(EXE): $(OBJS)
	g++ -o $(EXE) $(OBJS)

#	One *.o per *.cpp
#	Dependencies as specified by 'g++ -MM *.cpp'
#	Note that only one 'cpp' source file is a dependency (the others items are all headers)
main.o: main.cpp date.h address.h personinterface.h # <(if we only use headers, do we only need to recompile when headers change)>
personinterface.o: personinterface.cpp personinterface.h personconcrete.h date.h address.h
personconcrete.o: personconcrete.cpp personconcrete.h date.h address.h personinterface.h
address.o: address.cpp address.h
date.o: date.cpp date.h

clean:
	rm -f $(EXE) *.o


