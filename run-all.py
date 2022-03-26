#   VIM SETTINGS: {{{3
#   vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
#   vim: set foldlevel=2 foldcolumn=2:
#   }}}1
import sys
import os
import argparse
import logging
import re
import shutil
import time
logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)
#   {{{2
#parser = argparse.ArgumentParser()
#args = parser.parse_args()
#logging.debug("args=(%r)" % args)

bin_gpp = "g++"
cpp_std = "c++17"
build_dir = "/tmp/effective-c++/bin"
item_extension = "cpp"
dry_run = 0

#   Ongoing: 2022-03-27T03:19:24AEDT implementation with re comprehension, and seperate implementation with glob <- does glob support re matching?
def getItemFilenames_inDir_glob(path_dir):
    pass


def getItemFilenames_inDir(path_dir):
    regex_item_name = r'[0-9]\S*\.%s' % item_extension
    logging.debug("regex_item_name=(%s)" % regex_item_name)
    if not os.path.isdir(path_dir):
        raise Exception("not found, path_dir=(%s)" % path_dir)
    item_filenames = sorted( [ str(os.path.join(path_dir, f)) for f in os.listdir(path_dir) if re.match(regex_item_name, f) and os.path.isfile(os.path.join(path_dir, f)) ] )
    logging.debug("item_filenames=(%s)" % item_filenames)
    return item_filenames


def buildItemsFromList(build_dir, input_filenames):
    output_filenames = []
    logging.debug("len(input_filenames)=(%s)" % len(input_filenames))
    for loop_filename in input_filenames:
        if not os.path.isfile(loop_filename):
            raise Exception("not found, loop_filename=(%s)" % loop_filename)
        logging.debug("loop_filename=(%s)" % loop_filename)
        output_path = os.path.join(build_dir, re.sub(r'(.*)\.cpp', r'\1', loop_filename))
        if output_path == build_dir or output_path == loop_filename:
            raise Exception("invalid output_path=(%s)" % output_path)
        if len(output_path) == 0:
            raise Exception("output_path=(%s)" % output_path)
        logging.debug("output_path=(%s)" % output_path)
        output_dir = os.path.dirname(output_path)
		#	Ongoing: 2022-03-27T03:31:45AEDT why are we checking/creating output_dir inside loop?
        if not os.path.isdir(output_dir):
            logging.debug("mkdir(%s)" % output_dir)
            if not dry_run:
                os.mkdir(output_dir)
            else:
                logging.debug("dry-run, skip mkdir")
        loop_cmd = bin_gpp + " -std=" + cpp_std + " -o " + output_path + " " + loop_filename
        output_filenames.append(output_path)
        logging.debug("loop_cmd=(%s)" % loop_cmd)
        if not dry_run:
            os.system(loop_cmd)
            if not os.path.isfile(output_path):
                raise Exception("failed to build loop_filename=(%s), output_path=(%s)" % (loop_filename, output_path))
        else:
            logging.debug("dry-run, skip compile")
    return output_filenames


def runBinariesFromList(binaries_filenames):
    logging.debug("len(binaries_filenames)=(%s)" % len(binaries_filenames))


#   validate bin_gpg and build_dir
#   {{{
if not bin_gpp:
    raise Exception("bin_gpp=(%s)" % bin_gpp)
if not build_dir:
    raise Exception("build_dir=(%s)" % build_dir)
#   }}}
#   validate, delete, and re-create build_dir
#   {{{
if not dry_run:
    if os.path.isdir(build_dir):
        logging.debug("rmtree(%s)" % build_dir)
        shutil.rmtree(build_dir)
    if not os.path.isdir(build_dir):
        logging.debug("makedirs(%s)" % build_dir)
        os.makedirs(build_dir)
else:
    logging.debug("dry-run, skip make/delete dir")
#   }}}

_time_start = time.time()
path_precpp11 = "pre-c++11"
items_precpp11_filenames = getItemFilenames_inDir(path_precpp11)
binaries_precpp11_filenames = buildItemsFromList(build_dir, items_precpp11_filenames)

path_cpp14 = "c++14"

path_extra = "extra"

_time_end = time.time()
_time_elapsed_build = _time_end - _time_start


print("elapsed (build): %s" % _time_elapsed_build)


