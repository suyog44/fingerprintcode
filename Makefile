DEFINES=\
	-DRS232_SERIAL_COMM

INCLUDES=\
	-Iinclude/ \
	-Iinclude/oem/ \
	-Iinclude/oem/impl/ \
	-ILodePNG/ \
	-IRS232

HEADERS=\
	include/config.h \
	include/controller.h \
	include/helper.h \
	include/utils.h \
	include/oem/commbase.h \
	include/oem/oem.h \
	include/oem/oemp.h \
	include/oem/impl/commbase_rs232.h

SOURCES=\
	src/controller.cpp \
	src/helper.cpp \
	src/main.cpp \
	src/utils.cpp \
	src/oem/commbase.cpp \
	src/oem/oem.cpp \
	src/oem/oemp.cpp \
	src/oem/impl/commbase_rs232.cpp

all: controller.o helper.o main.o utils.o commbase.o oem.o oemp.o commbase_rs232.o lodepng.o rs232.o
	g++ controller.o helper.o main.o utils.o commbase.o oem.o oemp.o commbase_rs232.o lodepng.o rs232.o -o fingerprint

controller.o: include/controller.h src/controller.cpp
	g++ -c src/controller.cpp $(DEFINES) $(INCLUDES) -o controller.o

helper.o: include/helper.h src/helper.cpp
	g++ -c src/helper.cpp $(DEFINES) $(INCLUDES) -o helper.o

main.o: src/main.cpp
	g++ -c src/main.cpp $(DEFINES) $(INCLUDES) -o main.o

utils.o: include/utils.h src/utils.cpp
	g++ -c src/utils.cpp $(DEFINES) $(INCLUDES) -o utils.o

commbase.o: include/oem/commbase.h src/oem/commbase.cpp
	g++ -c src/oem/commbase.cpp $(DEFINES) $(INCLUDES) -o commbase.o

oem.o: include/oem/oem.h src/oem/oem.cpp
	g++ -c src/oem/oem.cpp $(DEFINES) $(INCLUDES) -o oem.o

oemp.o: include/oem/oemp.h src/oem/oemp.cpp
	g++ -c src/oem/oemp.cpp $(DEFINES) $(INCLUDES) -o oemp.o

commbase_rs232.o: include/oem/impl/commbase_rs232.h src/oem/impl/commbase_rs232.cpp
	g++ -c src/oem/impl/commbase_rs232.cpp $(DEFINES) $(INCLUDES) -o commbase_rs232.o

rs232.o: RS232/rs232.h RS232/rs232.c
	g++ -x c++ -c RS232/rs232.c $(DEFINES) $(INCLUDES) -o rs232.o

lodepng.o: LodePNG/lodepng.h LodePNG/lodepng.cpp
	g++ -c LodePNG/lodepng.cpp -ILodePNG/ -o lodepng.o

clean:
	rm -rf fingerprint *.o

