#!/bin/bash

FILE=$1
OUTPUT=$2

echo "/* Graph of $FILE */" > $OUTPUT // clears the output
echo "digraph {" >> $OUTPUT 
cat $FILE | while read line; do
	#echo $line 
	echo $line | sed s/.$// | awk '{ print $1 " -> " $2 ";" }' >> $OUTPUT
done
echo "}" >> $OUTPUT

exit 0;
