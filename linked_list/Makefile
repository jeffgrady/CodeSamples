# Author:  Jeff Grady <jeff.grady@gmail.com>
# Date:    3/17/2013
# 
# Description:  This is a Makefile to build my List code sample
#           
#               To run, type:
#
#               $ make
#
# Copyright (c) 2013 Jeffrey T. Grady, All Rights Reserved
#

# The program will be called "sample"
prog_name := sample

# Show all warnings, and give us debugging symbols, just in case
cflags := -Wall -ggdb

# Change this to whatever C++ compiler you have installed
compiler := clang++

cc := $(compiler) $(cflags)

# Our program needs its own object file
$(prog_name) : sample.o
	$(cc) -o $(prog_name) $^

# To work, the sample program needs its own code, plus the List object
sample.o : sample.cc List.o
	$(cc) -c -o $@ -x c++ $<

# The List object depends on its own code (which is in a .h file
# because of its use of C++ templates), plus the Node object.
#
# By the way, for those not familiar with linking issues around C++
# templates, the g++ I was using doesn't support using templates and
# separate .h and .cc files, compiling each object separately, and
# then linking them.  This is because the compiler doesn't know what
# type of methods to generate until it is used.  The workaround was to
# put the interface and implementation in the same .h file.
List.o : List.h Node.o
Node.o : Node.h

# We may not necessarily need these in building the final product, but
# it's nice to know sooner rather than later when mistakes are made.
%.o : %.h
	$(cc) -c -o $@ -x c++ $<

clean :
	rm -f $(prog_name) && \
	rm -f sample.o && \
	rm -f Node.o && \
	rm -f List.o
