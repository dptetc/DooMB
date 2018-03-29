#*      bldrules.mak
#*
#* Build pattern rules common for all make scripts
#*
#* $Id: bldrules.mak,v 1.10 1997/07/31 10:56:33 pekangas Exp $
#*
#* Copyright 1997 Housemarque Inc.
#*
#* This file is part of MIDAS Digital Audio System, and may only be
#* used, modified and distributed under the terms of the MIDAS
#* Digital Audio System license, "license.txt". By continuing to use,
#* modify or distribute this file you indicate that you have
#* read the license and understand and accept it fully.
#*


# C source files:
%.$(O) :	%.c
	$(COMPILEC)

$(OTARGETDIR)%.$(O) : %.c
	$(COMPILEC)

# According to the documentation the second one shouldn't be necessary...


# C++ source files:
%.$(O) :	%.cc
	$(COMPILECPP)

$(OTARGETDIR)%.$(O) : %.cc
	$(COMPILECPP)

%.$(O) :	%.cpp
	$(COMPILECPP)

$(OTARGETDIR)%.$(O) : %.cpp
	$(COMPILECPP)


# Assembler source files:
ifdef _DJGPP_TARGET
%.$(O) :	%.asm
	$(ASM) $(ASMOPTS) $<
	o2c $(patsubst %.$(O),%.obj,$@)

$(OTARGETDIR)%.$(O) : %.asm
	$(ASM) $(ASMOPTS) $<,temppi.obj
	o2c temppi.obj
	$(RM) temppi.obj
	$(CP) $*.o $(subst /,\,$@)
	$(RM) $*.o

%.$(O) :	%.S

$(OTARGETDIR)%.$(O) : %.S

%.$(O) :	%.s

$(OTARGETDIR)%.$(O) : %.s

else

%.$(O) :	%.asm
	$(COMPILEASM)

$(OTARGETDIR)%.$(O) : %.asm
	$(COMPILEASM)

%.$(O) :        %.S
	$(COMPILEC)

$(OTARGETDIR)%.$(O) : %.D
	$(COMPILEC)

endif



# NASM source files:

%.$(O) :	%.nas
	$(COMPILENASM)

$(OTARGETDIR)%.$(O) : %.nas
	$(COMPILENASM)


# Resource files:
%.res :		%.rc
	$(BUILDRES)


#* $Log: bldrules.mak,v $
#* Revision 1.10  1997/07/31 10:56:33  pekangas
#* Renamed from MIDAS Sound System to MIDAS Digital Audio System
#*
#* Revision 1.9  1997/07/28 11:16:57  pekangas
#* Fixed to compile TASM files for DJGPP properly
#* Some changes to DJGPP options
#*
#* Revision 1.8  1997/07/22 13:45:43  pekangas
#* Added support for keeping object modules in a different directory
#* from the source
#*
#* Revision 1.7  1997/06/10 19:58:28  jpaana
#* Added nasm rule to Linux
#*
#* Revision 1.6  1997/06/04 15:27:25  pekangas
#* Added NASM support
#*
#* Revision 1.5  1997/03/05 16:54:15  pekangas
#* Fixed compiling assembler files for DJGPP when not running in DOS
#*
#* Revision 1.4  1997/02/27 16:22:44  pekangas
#* Added DJGPP support. Changed INCDIR to INCPATH. Other minor fixes.
#*
#* Revision 1.3  1997/02/07 15:16:32  pekangas
#* Added GCC/Linux support
#*
#* Revision 1.2  1997/02/05 22:53:55  pekangas
#* Several small changes to get the new make script system work better
#*
#* Revision 1.1  1997/02/05 17:25:54  pekangas
#* Initial revision
#*
