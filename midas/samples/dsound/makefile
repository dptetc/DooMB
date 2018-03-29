INCDIR = ..\\..\\src\\midas
LIBRARY = ..\\..\\lib\\win32\\wcretail\\midasdll.lib

all :		dsmod.exe

clean distclean cvsclean :

dsmod.exe :	dsmod.c
	wcl386 -w3 -d2 -i$(INCDIR) dsmod.c $(LIBRARY)

