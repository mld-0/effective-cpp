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
build_dir=bin
item_extension="cpp"
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
	echo "len(input_filenames)=(${#input_filenames[@]})"
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
	mkdir $build_dir
fi
#	}}}

_time_start=$( $_bin_python3 -c 'import time; print(int(time.time_ns()))' )
path_precpp11="pre-c++11"
items_precpp11_filenames=( $( getItemFilenames_inDir "$path_precpp11" ) )
binaries_precpp11_filenames=( $( buildItemsFromList "$build_dir" "${items_precpp11_filenames[@]}" ) )
echo ""

path_cpp14="c++14"
items_cpp14_filenames=( $( getItemFilenames_inDir "$path_cpp14" ) )
binaries_cpp14_filenames=( $( buildItemsFromList "$build_dir" "${items_cpp14_filenames[@]}" ) )
echo ""
_time_end=$( $_bin_python3 -c 'import time; print(int(time.time_ns()))' )

_time_elapsed=$( perl -E "say(($_time_end - $_time_start) / 1E9)" )
echo "elapsed: $_time_elapsed"

#	TODO: 2022-03-24T21:12:36AEDT some rule for getting examples from 'extra' to build


