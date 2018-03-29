#*      makefile
#*
#* Make script, used just for cleanup
#*
#* $Id: makefile,v 1.2 1997/07/31 10:56:29 pekangas Exp $
#*
#* Copyright 1996,1997 Housemarque Inc.
#*
#* This file is part of MIDAS Digital Audio System, and may only be
#* used, modified and distributed under the terms of the MIDAS
#* Digital Audio System license, "license.txt". By continuing to use,
#* modify or distribute this file you indicate that you have
#* read the license and understand and accept it fully.
#*

# Include build options:
BLDOPTSDIR=../../src/bldopts
include $(BLDOPTSDIR)/bldopts.mak


# Default target:
all :
	$(REM) Targets available here: clean, distclean, cvsclean
	$(REM) To compile the graphical player, load gplay.dpr to Delphi
	$(REM) To compile the command line player, load miniplay.pas

# Cleanup:
clean distclean cvsclean :
	$(CLEANALL)
	-$(RM) *.~*
	-$(RM) *.dcu

.PHONY : clean distclean cvsclean all
