#!/usr/bin/make -f
# Filename: Makefile
# Date: November 20, 2020
# Author: Anh Nguyen
# Email Your UTD: nxn190023
# Course CS 3377.002 Fall 2020
# Version 1.0 (or correct version)
# Copyright 2020, All Rights Reserved

CXX = g++
CXXFLAGS = -Wall -g 
CPPFLAGS = -I /scratch/perkins/include
LDFLAGS = -L /scratch/perkins/lib
LDLIBS = -lcdk -lcurses 
PROJECTNAME = Assignment6

SRCS = CDKMatrix.cc 

EXEC = assignment6


OBJS = $(SRCS:cc=o)

all: $(EXEC)

clean:
	rm -f $(OBJS) *.d *~ \#* $(EXEC)

Makefile: $(SRCS:.cc=.d)

# Pattern for .d files.
%.d:%.cc
	@echo Updating .d Dependency File
	@set -e; rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

backup:	clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!


# Include the dependency files
-include $(SRCS:.cc=.d)
