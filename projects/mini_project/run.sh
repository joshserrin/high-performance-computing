#!/bin/sh

ROOMY_INSTALL=/home/jserrin/roomy-install

rm a.out
#gcc $1 -I/home/jserrin/roomy-install/include 
mpicc $1 -o a.out -I$ROOMY_INSTALL/include -L$ROOMY_INSTALL/lib $ROOMY_INSTALL/lib/libroomy.a -lmpich -lpthread -lm
./a.out

exit 0;
