#!/bin/sh

INPUT=$1
OUTPUT=$2
dot -Tpng -o $OUTPUT $INPUT 
