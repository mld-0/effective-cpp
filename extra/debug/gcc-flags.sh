#   VIM SETTINGS: {{{3
#   vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
#   vim: set foldlevel=2 foldcolumn=2:
#   }}}1
#	{{{2
#	Ongoings:
#	{{{
#	Ongoing: 2022-05-15T01:06:22AEST g++, (how to tell g++ to list all flags that we have enabled implicitly/by-other-flags)
#	Ongoing: 2022-05-15T00:58:37AEST debug mode does something to cause a crash on <bad (memory) touch> (whereas -O2/3/fast do not crash -> that the debug application crashed is a <sure?> sign optimized version is carrening into undefined behaviour
#	Ongoing: 2022-05-15T00:57:19AEST list is not comprehensive
#	Ongoing: 2022-05-15T00:55:52AEST many of these flags are quite useless without an example of usage
#	Ongoing: 2022-05-15T00:50:47AEST (what are) 'debug counters'?
#	}}}

#	TODO: 2022-05-15T01:13:46AEST effective-c++, debug/gcc-optimization, ((read link) (see below) (gcc.gnu.org) (on gcc/g++ 'Debug' mode)) -> what is gcc already doing for us vis-a-vis debugging, and (the point is: are flags like 'lookout for undefined-behaviour/leaks/stackoverflow/bad-stuff' enabled when we -Og or -O0?)



#	Debug flags:
#	LINK: https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
#		-g<level>			debug info in OS naitive format
#		-ggdb<level>		debug info for use by GDB
#		-gstabs<level>		debug info in stabs foramt (DBX on BSD)
#		-gxcoff<level>		debug info in XCOFF format
#		-gvms<level>		debug info in Alpha/VMS format
#	<level>:
#	0	no info
#	1	minimal information, enough for making backtraces in parts of the program that you don’t plan to debug. This includes descriptions of functions and external variables, and line number tables, but no information about local variables
#	2	default
#	3	includes extra information, such as all the macro definitions present in the program. Some debuggers support macro expansion

#		-p					generate extra code to write profile information suitible for 'prof'
#		-pg					generate extra code to write profile information suitible for 'gprof'

#		-Q					make compiler print each function name as it is compiled, with compilation statistics

#		-fstack-usage		output stack usage info, on per function basis (.su file)

#		-ftime-report		report time consumed by each <(optimization?)> pass
#		-fmem-report		report memory consumed by each <(optimization?)> pass

#		-fpre-ipa-mem-report		print statistics about permenant memory allocation before/after
#		-fpost-ipa-mem-report		interprocedural optimization

#		-fprofile-arcs		Add code so that program flow arcs are instrumented. (.gcda file)

#		--coverage			This option is used to compile and link code instrumented for coverage analysis

#		-fdbg-cnt-list		Print the name and the counter upper bound for all debug counters.

#	-fenable-<kind-pass>							<>
#	-fdisable-<kind-pass>=<range-list>


#	-fdump-rtl-<pass>		<>

#	-frandom-seed=<string>			<(provide a seed where otherwise a random number would be used)>

#	-fsched-verbose=<n>				On targets that use instruction scheduling, this option controls the amount of debugging output the scheduler prints. (.sched1 file)

#	-save-temps						<(Store the usual “temporary” intermediate files permanently; place them in the 
#	-save-temps=<cwd>				current directory and name them based on the source file)>

#	-save-temps=obj					<(Store the usual temporary intemediate files permenantly)>

#	-time[=<file>]					Report the CPU time taken by each subprocess in the compilation sequence. 


#	-print-search-dirs				Print the name of the configured installation directory and a list of program and library directories gcc will search—and don't do anything else.

#	-dumpmachine
#	-dumpversion


#	<(AArch64 only?)>
#	-fsanitize=address
#	-fsanitize=kernel-address
#	-fsanitize=hwaddress
#	-fsanitize=kernel-hwaddress
#	-fsanitize=pointer-compare
#	-fsanitize=pointer-subtract
#	-fsanitize=shadow-call-stack
#	-fsanitize=thread
#	-fsanitize=leak
#	-fsanitize=undefined
#	<(lots more)> <(does some debug flag turn most/all these on?)>


#	<(non-AArch64?)>
#	-fstack-protector
#	-fstack-protector-all
#	-fstack-limit-register=reg
#	-fstack-limit-symbol=sym
#	-fno-stack-limit
#	-fvtv-debug
#	-fvtv-counts
#	-finstrument-functions



#	Optimization: -O<>
#	LINK: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
#		-O0		default					<(make debugging produce the expected results)>
#		-Os		code-size				<>
#		-O1		level 1 				all optimizations that are not time-consuming
#		-O2		level 2					all optimizations that do not involve speed-space tradeoff
#		-O3		level 3					<>
#		-Ofast	fastest					<>
#		-Og		debug					standard edit-compile-debug cycle
#		-Oz		aggressive code-size




#	<(What does debug build do to produce a crash for <(bad memory usage)> (considering that non-debug build does not (necessarily) crash))>


#	g++ Debug mode 
#	LINK: https://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode.html
#	{{{
#	}}}

#	LINK: http://sourceware.org/gdb/current/onlinedocs/gdb/
#	{{{
#	}}}

