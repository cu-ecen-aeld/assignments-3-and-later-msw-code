#!/bin/sh

#
# writer.sh
#
# Argument 1: Full path to a file (inc filename) on the filesystem
# Argument 2: Text string to be written within the file
#

writefile=$1
writestring=$2

# First check we have the right number of arguments
if [ $# -ne 2 ]
then
	echo Error! Incorrect number of arguments specified!
	exit 1
fi

# Attempt to write the string to the file, creating if one doesn't exist, overwriting if it exists
directory=$(dirname "$writefile")
mkdir -p "$directory"
echo "$writestring" > "$writefile"

# Check writefile was created
if [ -w "$writefile" ]
then
	exit 0
else
	echo Error! File could not be created!
	exit 1
fi

