# Define right paths for the wxWidgets library below

CC = mingw32-g++
LIB = C:\wxWidgets-3.1.0\include
LIBDLL = C:\wxWidgets-3.1.0\lib\gcc_dll\mswu
LIBA = C:\wxWidgets-3.1.0\lib\gcc_dll
FLAGS = -pipe -Wall -O2

all:sample

sample: main.o OdtCreator.o OdtCreatorArch.o OdtCreatorDriver.o OdtCreatorTemplates.o OdtFormula.o OdtImages.o OdtTable.o 
	$(CC) main.o OdtCreator.o OdtCreatorArch.o OdtCreatorDriver.o OdtCreatorTemplates.o OdtFormula.o OdtImages.o OdtTable.o -s $(LIBA)\libwxexpat.a $(LIBA)\libwxjpeg.a $(LIBA)\libwxmsw31u.a $(LIBA)\libwxmsw31u_gl.a $(LIBA)\libwxpng.a $(LIBA)\libwxregexu.a $(LIBA)\libwxscintilla.a $(LIBA)\libwxtiff.a $(LIBA)\libwxzlib.a -o sample

main.o: main.cpp 
	$(CC) main.cpp $(FLAGS) -I$(LIB) -I$(LIBDLL) -c

OdtCreator.o:
	$(CC) ../../class/src/OdtCreator.cpp  $(FLAGS) -I$(LIB) -I$(LIBDLL) -c

OdtCreatorArch.o:
	$(CC) ../../class/src/OdtCreatorArch.cpp  $(FLAGS) -I$(LIB) -I$(LIBDLL) -c

OdtCreatorDriver.o:
	$(CC) ../../class/src/OdtCreatorDriver.cpp  $(FLAGS) -I$(LIB) -I$(LIBDLL) -c

OdtCreatorTemplates.o:
	$(CC) ../../class/src/OdtCreatorTemplates.cpp  $(FLAGS) -I$(LIB) -I$(LIBDLL) -c

OdtFormula.o:
	$(CC) ../../class/src/OdtFormula.cpp  $(FLAGS) -I$(LIB) -I$(LIBDLL) -c

OdtImages.o:
	$(CC) ../../class/src/OdtImages.cpp  $(FLAGS) -I$(LIB) -I$(LIBDLL) -c

OdtTable.o:
	$(CC) ../../class/src/OdtTable.cpp  $(FLAGS) -I$(LIB) -I$(LIBDLL) -c

clean:
	del *.o sample.exe