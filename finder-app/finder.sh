#!/bin/sh

# finder.sh
#
# Argument 1: Path to directory on filesystem
# Argument 2: Text string to search for in files
#

# First check we have two arguments
if [ $# -ne 2 ]
then
	echo Error! Incorrect number of arguments used!
	exit 1
else
	filesdir=$1
	searchstr=$2
fi

# Now check if argument 1 is a directory in the filesystem
if [ -d "$filesdir" ]
then
	# Search recursively for the search string starting at the filesdir and count lines for number of results
	linecount=$(grep -r -o "$searchstr" "$filesdir" | wc -l)

	# Combine wc -l with ls to list number of files in a directory (count lines from ls result)
	filecount=$(find "$filesdir" -type f | wc -l)

	echo The number of files are "$filecount" and the number of matching lines are "$linecount"
	exit 0
else
	echo Error! Argument 1 not a valid directory!
	exit 1

fi



