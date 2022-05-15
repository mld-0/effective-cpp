#   VIM SETTINGS: {{{3
#   vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
#   vim: set foldlevel=2 foldcolumn=2:
#   }}}1
#	{{{2
#	Ongoings:
#	{{{
#	Ongoing: 2022-05-15T14:40:50AEST <(What does debug build do to produce a crash for <(bad memory usage)> (considering that non-debug build does not (necessarily) crash))>
#	Ongoing: 2022-05-15T01:06:22AEST g++, (how to tell g++ to list all flags that we have enabled implicitly/by-other-flags)
#	Ongoing: 2022-05-15T00:58:37AEST debug mode does something to cause a crash on <bad (memory) touch> (whereas -O2/3/fast do not crash -> that the debug application crashed is a <sure?> sign optimized version is carrening into undefined behaviour
#	Ongoing: 2022-05-15T00:57:19AEST list is not comprehensive
#	Ongoing: 2022-05-15T00:55:52AEST many of these flags are quite useless without an example of usage
#	Ongoing: 2022-05-15T00:50:47AEST (what are) 'debug counters'?
#	}}}
#	TODO: 2022-05-15T01:13:46AEST effective-c++, debug/gcc-optimization, ((read link) (see below) (gcc.gnu.org) (on gcc/g++ 'Debug' mode)) -> what is gcc already doing for us vis-a-vis debugging, and (the point is: are flags like 'lookout for undefined-behaviour/leaks/stackoverflow/bad-stuff' enabled when we -Og or -O0?)


#	Debug flags dump
#	{{{
#	LINK: https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
#		-g<level>			debug info in OS naitive format
#		-ggdb<level>		debug info for use by GDB
#		-gstabs<level>		debug info in stabs foramt (DBX on BSD)
#		-gxcoff<level>		debug info in XCOFF format
#		-gvms<level>		debug info in Alpha/VMS format

#	Debug levels:
#	0	no info
#	1	minimal information, enough for making backtraces in parts of the program that you don’t plan to debug. This includes descriptions of functions and external variables, and line number tables, but no information about local variables
#	2	default
#	3	includes extra information, such as all the macro definitions present in the program. Some debuggers support macro expansion

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

#	Ongoing: 2022-05-15T14:26:42AEST (mostly) useless without examples(?)
#	Other flags:
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
#		-fenable-<kind-pass>							<>
#		-fdisable-<kind-pass>=<range-list>
#		-fdump-rtl-<pass>		<>
#		-frandom-seed=<string>			<(provide a seed where otherwise a random number would be used)>
#		-fsched-verbose=<n>				On targets that use instruction scheduling, this option controls the amount of debugging output the scheduler prints. (.sched1 file)
#		-save-temps						<(Store the usual “temporary” intermediate files permanently; place them in the 
#		-save-temps=<cwd>				current directory and name them based on the source file)>
#		-save-temps=obj					<(Store the usual temporary intemediate files permenantly)>
#		-time[=<file>]					Report the CPU time taken by each subprocess in the compilation sequence. 
#		-print-search-dirs				Print the name of the configured installation directory and a list of program and library directories gcc will search—and don't do anything else.
#		-dumpmachine
#		-dumpversion
#	{{{
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
#	}}}

#	}}}

#	'g++ --help' output is ... unfortunately verbose

#	g++ debug-related options:
#			-g<level>	Generate source-level debug info <(optionally specify debug level?)>
#			-v			Show commands to run and use verbose output
#			-O<level>	Optimization level
	

#	g++ other options:
#			-###                    Print (but do not run) the commands to run for this compilation
#			-L <dir>                Add directory to library search path
#			-I <dir>                Add directory to the end of the list of include search paths
#			-F <value>              Add directory to framework include search path
#			-E                      Only run the preprocessor
#			-D <macro>=<value>      Define <macro> to <value> (or 1 if <value> omitted)
#			-C                      Include comments in preprocessed output
#			-c                      Only run preprocess, compile, and assemble steps
#			-CC                     Include comments from within macros in preprocessed output
#			-x <language>           Treat subsequent input files as having type <language>
#			-z <arg>                Pass -z <arg> to the linker
#			-Wa,<arg>               Pass the comma separated arguments in <arg> to the assembler
#			-Wl,<arg>               Pass the comma separated arguments in <arg> to the linker
#			-Wp,<arg>               Pass the comma separated arguments in <arg> to the preprocessor
#			-U <macro>              Undefine macro <macro>
#			-T <script>             Specify <script> as linker script
#			-time                   Time individual commands
#			-S                      Only run preprocess and compilation steps
#			--target=<value>        Generate code for the given target
#			--verify-debug-info     Verify the binary representation of debug output
#			-stdlib=<value>         C++ standard library to use
#			-std=<value>            Language standard to compile for
#			-R<remark>              Enable the specified remark
#			-pthread                Support POSIX threads in generated code
#			-fsycl                  Enables SYCL kernels compilation for device
#			-H                      Show header includes and nesting depth
#			-o <file>               Write output to <file>
#			-pedantic               Warn on language extensions

#			-S						(output assembly) only run preprocess/compilation steps

#			-B <prefix>             Search $prefix/$triple-$file and $prefix$file for executables, libraries, includes, and data files used by the compiler. $prefix may or may not be a directory

#			-save-temps			save intermediate compilation results

#			-save-stats

#			<(and many more)>


#	g++ Warnings:
#			-Werror
#			-Wall
#			-Wextra



#	g++ Debug mode 
#	LINK: https://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode.html
#	{{{
#	When used incorrectly, libstdc++ may give results that are not predictable. 
#	To detect (some) of these errors, it offers a debug mode.
#	<(among other things, checks whether iterators are used safely, and validates inputs to std-algorithms)>
#	Program behaviour is the same in debug mode, but some performance guarantees are not offered.

#	eg:	erasing an element in a std::list is constant-time normally, but in debug mode is is linear-time to the number of iterators that reference that list.

#	'-pedantic' <(that attempts to emulate the sematics guaranteed by the C++ standard?)>

#	the following std types provide extra capabilities in debug mode:
#			std::array					<(no safe iterators?)>
#			std::basic_string			<(no safe iterators?)>
#			std::bisect
#			std::deque
#			std::list
#			std::map
#			std::multimap
#			std::multiset
#			std::set
#			std::vector
#			std::unordered_map
#			std::unordered_multimap
#			std::unordered_set
#			std::unordered_multiset

#	Debug mode usage:
#		-D_GLIBCXX_DEBUG

#	std containers must not be passed between code compiled with/without debug mode

#	Specific debug containers:
#	{{{
#		Container	Header	Debug container	Debug header
#		std::bitset	bitset	__gnu_debug::bitset	<debug/bitset>
#		std::deque	deque	__gnu_debug::deque	<debug/deque>
#		std::list	list	__gnu_debug::list	<debug/list>
#		std::map	map	__gnu_debug::map	<debug/map>
#		std::multimap	map	__gnu_debug::multimap	<debug/map>
#		std::multiset	set	__gnu_debug::multiset	<debug/set>
#		std::set	set	__gnu_debug::set	<debug/set>
#		std::string	string	__gnu_debug::string	<debug/string>
#		std::wstring	string	__gnu_debug::wstring	<debug/string>
#		std::basic_string	string	__gnu_debug::basic_string	<debug/string>
#		std::vector	vector	__gnu_debug::vector	<debug/vector>
#	<C++11>
#		std::forward_list	forward_list	__gnu_debug::forward_list	<debug/forward_list>
#		std::unordered_map	unordered_map	__gnu_debug::unordered_map	<debug/unordered_map>
#		std::unordered_multimap	unordered_map	__gnu_debug::unordered_multimap	<debug/unordered_map>
#		std::unordered_set	unordered_set	__gnu_debug::unordered_set	<debug/unordered_set>
#		std::unordered_multiset	unordered_set	__gnu_debug::unordered_multiset	<debug/unordered_set>
#	}}}

#	Any valid C++ program should behave in the same manner regardless of whether it is compiled with debug mode or release mode.
#	The additional of the libstdc++ debug mode must not affect the performance of the library when it is compiled in release mode.

#	Wrappers: layer on top of release versions of containers/iterators, validate inputs/operation before passing off values to underlying implementation.
#	__gnu_debug::_Safe_iterator<Iterator, Sequence>

#	debug mode enables checking of preconditions for all supported standard library operations.


#	}}}


#	LINK: http://sourceware.org/gdb/current/onlinedocs/gdb/
#	{{{
#	}}}

