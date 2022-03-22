#   VIM SETTINGS: {{{3
#   vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
#   vim: set foldlevel=2 foldcolumn=2:
#   }}}1
#set -o errexit   # abort on nonzero exitstatus
set -o nounset   # abort on unbound variable
set -o pipefail  # don't hide errors within pipes
#	{{{2

bin_gpp=g++
cpp_std=c++17
binary_name=a.out


#	Ongoing: 2022-03-23T01:57:59AEDT using IFS=$'\0' and -print0 are zsh features, instead we rely on whitespace as a delimitor, since our search is for filenames without whitespace.
getItemFilenames_inDir() {
	local path_dir="$1"
	local regex_item_name="[0-9]\S*\.cpp"
	if echo -n "$path_dir" | perl -nE '/\s/ or exit 1'; then
		echo "error, path_dir=($path_dir) contains whitespace" > /dev/stderr
		exit 2
	fi
	if [[ ! -d "$path_dir" ]]; then
		echo "path_dir=($path_dir) not found" > /dev/stderr
		exit 2
	fi
	items_filenames=( $( find "$path_dir" | perl -nE "/\Q$path_dir\E\/$regex_item_name/ and print" | sort -h ) )
	if [[ ${#items_filenames[@]} -eq 0 ]]; then 
		echo "error, path_dir=($path_dir), no items_filenames found" > /dev/stderr
		exit 2
	fi
	echo "${items_filenames[@]}"
}

if ! command -v $bin_gpp &> /dev/null; then
	echo "bin_gpp=($bin_gpp) could not be found"
    exit 2
fi

path_cpp14="c++14"
items_cpp14_filenames=( $( getItemFilenames_inDir "$path_cpp14" ) )

#	Continue: 2022-03-23T03:06:30AEDT create a different file for each binary, the name taken from the source filename, all placed in some directory ./bin or ../bin -> we want one time for how long it takes to compile everything, and another for how long to run everything
for loop_filename in "${items_cpp14_filenames[@]}"; do
	if [[ -e $binary_name ]]; then
		echo "rm $binary_name" > /dev/stderr
		rm $binary_name
	fi
	echo "loop_filename=($loop_filename)" > /dev/stderr
	$bin_gpp -std=$cpp_std $loop_filename -o $binary_name
	if [[ -e $binary_name ]]; then
		./$binary_name
	else
		echo "failed"
	fi
done


