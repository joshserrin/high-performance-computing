#!/bin/bash

FILE=$1

# You'll need to set the end point!
for i in {1..999}
do
	#echo "Node ID: $i"
	printf '%07d' `cat $FILE | grep "$i ->" | wc -l`
	printf " edges for node $i"
	echo "" # newline
done
