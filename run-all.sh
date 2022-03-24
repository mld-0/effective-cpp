#   VIM SETTINGS: {{{3
#   vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
#   vim: set foldlevel=2 foldcolumn=2:
#   }}}1
#set -o errexit   # abort on nonzero exitstatus
set -o nounset   # abort on unbound variable
set -o pipefail  # don't hide errors within pipes
#	{{{2

#	Allow subshell 'exit 2' to terminate script
set -E;
trap '[ "$?" -ne 2 ] || exit 2' ERR

bin_gpp=g++
cpp_std=c++17

#	Ongoing: 2022-03-24T22:08:52AEDT use 'cpp_build_flags'
cpp_build_flags=()

#	Ongoing: 2022-03-25T01:36:20AEDT build in '/tmp' to avoid Dropbox client overhead (do not tell me to use <mktemp>(?), the quiteness of /tmp is sublime ... almost as if, by moving the day-to-day job of being a tempdir to '/var/folders/06/9pmr7sss7kg000jj16m89tkh0000gn/T' (which /tmp is *not* linked to) -> 'readlink /tmp' yields '/private/tmp' (which is a real directory)
build_dir=/tmp/effective-c++/bin
item_extension="cpp"
#	Ongoing: 2022-03-24T22:13:50AEDT do not delete/create directories if dry_run!=0
dry_run=0
_delim=$'\n'
_bin_python3=python3

#	Ongoing: 2022-03-23T01:57:59AEDT using IFS=$'\0' and -print0 are zsh features, instead we rely on whitespace as a delimitor, since our search is for filenames without whitespace.
getItemFilenames_inDir() {
	local path_dir="$1"
	local regex_item_name="[0-9]\S*\.$item_extension"
	if echo -n "$path_dir" | perl -nE '/\s/ or exit 1'; then
		echo "error, path_dir=($path_dir) contains whitespace" > /dev/stderr
		exit 2
	fi
	if [[ ! -d "$path_dir" ]]; then
		echo "path_dir=($path_dir) not found" > /dev/stderr
		exit 2
	fi
	items_filenames=( $( find "$path_dir" -maxdepth 1 | perl -nE "/\Q$path_dir\E\/$regex_item_name/ and print" | sort -h ) )
	if [[ ${#items_filenames[@]} -eq 0 ]]; then 
		echo "error, path_dir=($path_dir), no items_filenames found" > /dev/stderr
		exit 2
	fi
	echo "${items_filenames[@]}"
}

buildItemsFromList() {
	local build_dir=$1
	shift
	local input_filenames=( "$@" )
	echo "len(input_filenames)=(${#input_filenames[@]})" > /dev/stderr
	local _str_output_filenames=""
	for loop_filename in "${input_filenames[@]}"; do
		#echo "loop_filename=($loop_filename)" > /dev/stderr
		output_path=$( echo "$build_dir/$loop_filename" | perl -pE 's/(.*)\.cpp/\1/' )
		if [[ -z $output_path ]]; then
			echo "error, output_path=($output_path)" > /dev/stderr
			exit 2
		fi
		if echo -n "$output_path" | perl -nE '/\s/ or exit 1'; then
			echo "error, contains whitespace, output_path=($output_path)" > /dev/stderr
			exit 2
		fi
		#echo "output_path=($output_path)" > /dev/stderr
		output_dir=$( dirname $output_path )
		#echo "output_dir=($output_dir)" > /dev/stderr
		if [[ ! -d $output_dir ]]; then
			echo "mkdir $output_dir" > /dev/stderr
			mkdir $output_dir
		fi
		loop_cmd=( $bin_gpp -std=$cpp_std -o $output_path $loop_filename )
		if [[ $dry_run -eq 0 ]]; then
			echo ${loop_cmd[@]} > /dev/stderr
			${loop_cmd[@]}
			#	Ongoing: 2022-03-24T21:15:18AEDT 'exit 2' called within a function terminates the function, not the script? -> problem comes from using $() subshell?
			if [[ ! -e $output_path ]]; then
				echo "failed" > /dev/stderr
				exit 2
			fi
		else
			echo "dry-run" > /dev/stderr
		fi
		_str_output_filenames="$_str_output_filenames$_delim$output_path"
		#echo "" > /dev/stderr
	done
	local output_filenames=( $_str_output_filenames )
	echo "${output_filenames[@]}"
}

#	Ongoing: 2022-03-25T02:09:51AEDT how to exempt binaries from XprotectService befor we run them (current solution being to run them twice)
runBinariesFromList() {
	local binaries_filenames=( "$@" )
	echo "len(binaries_filenames)=(${#binaries_filenames[@]})" > /dev/stderr
	for loop_filename in "${binaries_filenames[@]}"; do
		if [[ ! -e $loop_filename ]]; then
			echo "error, not found, loop_filename=($loop_filename)" > /dev/stderr
			exit 2
		fi
		echo "loop_filename=($loop_filename)" > /dev/stderr
		$loop_filename
	done
}

#	validate $bin_gpg and $build_dir
#	{{{
if ! command -v $bin_gpp &> /dev/null; then
	echo "bin_gpp=($bin_gpp) could not be found" > /dev/stderr
    exit 2
fi
if [[ -z $build_dir ]]; then
	echo "error, build_dir=($build_dir)" > /dev/stderr
	exit 2
fi
#	}}}
#	validate, delete, and re-create $build_dir
#	{{{
if [[ -d $build_dir ]]; then
	echo "rm -r $build_dir" > /dev/stderr
	rm -r $build_dir
fi
if [[ ! -d $build_dir ]]; then
	echo "mkdir $build_dir" > /dev/stderr
	mkdir -p $build_dir
fi
#	}}}

#	TODO: 2022-03-25T01:24:02AEDT effective-c++, run-all.sh, Perl, get system nano-time one-liner (can we do with a vanilla macOS/Linux version) (currently using python, which I am inclined to think is too slow for use in 'casual' one-liners)
_time_start=$( $_bin_python3 -c 'import time; print(int(time.time_ns()))' )
path_precpp11="pre-c++11"
items_precpp11_filenames=( $( getItemFilenames_inDir "$path_precpp11" ) )
binaries_precpp11_filenames=( $( buildItemsFromList "$build_dir" "${items_precpp11_filenames[@]}" ) )
echo ""

path_cpp14="c++14"
items_cpp14_filenames=( $( getItemFilenames_inDir "$path_cpp14" ) )
binaries_cpp14_filenames=( $( buildItemsFromList "$build_dir" "${items_cpp14_filenames[@]}" ) )
echo ""

#	TODO: 2022-03-24T21:12:36AEDT effective-c++, run-all.sh, some rule for getting examples from 'extra/' to build
path_extra="extra"

_time_end=$( $_bin_python3 -c 'import time; print(int(time.time_ns()))' )
_time_elapsed_build=$( perl -E "say(($_time_end - $_time_start) / 1E9)" )
echo ""


_time_start=$( $_bin_python3 -c 'import time; print(int(time.time_ns()))' )
echo "run $path_precpp11"
runBinariesFromList "${binaries_precpp11_filenames[@]}"
echo ""
echo "run $path_cpp14"
runBinariesFromList "${binaries_cpp14_filenames[@]}"
echo ""
_time_end=$( $_bin_python3 -c 'import time; print(int(time.time_ns()))' )
_time_elapsed_run1=$( perl -E "say(($_time_end - $_time_start) / 1E9)" )

_time_start=$( $_bin_python3 -c 'import time; print(int(time.time_ns()))' )
echo "run $path_precpp11"
runBinariesFromList "${binaries_precpp11_filenames[@]}"
echo ""
echo "run $path_cpp14"
runBinariesFromList "${binaries_cpp14_filenames[@]}"
echo ""
_time_end=$( $_bin_python3 -c 'import time; print(int(time.time_ns()))' )
_time_elapsed_run2=$( perl -E "say(($_time_end - $_time_start) / 1E9)" )

echo "elapsed (build): $_time_elapsed_build"
echo "elapsed (run 1): $_time_elapsed_run1"
echo "elapsed (run 2): $_time_elapsed_run2"
echo ""

