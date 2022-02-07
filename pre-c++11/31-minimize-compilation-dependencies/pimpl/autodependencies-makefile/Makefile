
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


#	TODO: 2022-02-07T21:26:42AEDT effective-c++/31*/*/autodependencies-makefile, is a better solution where we generate dependency rules to use for clean-compiles as well as re-compiles? (can you find someone else to explain that dependencies are not needed for a clean-compile)
#	Ongoing: 2022-02-07T17:08:25AEDT in what order does: compilation, creation of dependencies files, include-ing of dependencies files happen? 
#	<(This is executed before any compiling takes place)>, we read in the dependency rules only if they have been previously generated (hence the '-' include), (they are not used for a clean build, not needed since the only purpouse of these dependency files serve is in making the decision whether to re-compile)
#	leading '-', no error if any of the filenames do not exist
#	(must come after <build rules>?)
-include $(deps)


.PHONY: clean
clean:
	rm -f $(objs) $(deps) $(EXE)

.PHONY: cleandep
cleandep:
	rm -f $(deps)



