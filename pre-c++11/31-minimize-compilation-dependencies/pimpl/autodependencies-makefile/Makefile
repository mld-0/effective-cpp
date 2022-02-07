
#	Special variables:
#		$^		all the dependencies of a given rule
#		$@		target of a given rule
#		$<		first element of the dependencies of a given rule

CXX = g++ -std=c++17
CXXFLAGS = -Wall

srcs = $(wildcard *.cpp)	#	all *.cpp files
objs = $(srcs:.cpp=.o)		#	*.o file for each *.cpp file
deps = $(srcs:.cpp=.d)		#	*.d file for each *.cpp file

EXE = a.out 				#	output executable

#	Linking
$(EXE): $(objs)
	$(CXX) $^ -o $@ $(CXXFLAGS)

#	Compiling with automatic dependencies
#		-MMD	list user header file (dependencies) (which get used how?)
#		-MP		add a phony target for each dependency other than the main file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@ 

#	Ongoing: 2022-02-07T17:08:25AEDT in what order does: compilation, creation of dependencies files, include-ing of dependencies files happen?
#	leading '-' on include -> no error if any of the filenames do not exist
-include $(deps)

.PHONY: clean
clean:
	rm -f $(objs) $(deps) $(EXE)

.PHONY: cleandep
cleandep:
	rm -f $(deps)

