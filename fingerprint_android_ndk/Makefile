DEFINES=\
	-DRS232_SERIAL_COMM

INCLUDES=

HEADERS=\
	config.h \
	controller.h \
	helper.h \
	utils.h \
	commbase.h \
	oem.h \
	oemp.h \
	commbase_rs232.h

SOURCES=\
	controller.cpp \
	helper.cpp \
	main.cpp \
	utils.cpp \
	commbase.cpp \
	oem.cpp \
	oemp.cpp \
	commbase_rs232.cpp

all: controller.o helper.o main.o utils.o commbase.o oem.o oemp.o commbase_rs232.o lodepng.o rs232.o
	g++ controller.o helper.o main.o utils.o commbase.o oem.o oemp.o commbase_rs232.o lodepng.o rs232.o -o fingerprint

controller.o: controller.h controller.cpp
	g++ -c controller.cpp $(DEFINES)  -o controller.o

helper.o: helper.h helper.cpp
	g++ -c helper.cpp $(DEFINES)  -o helper.o

main.o: main.cpp
	g++ -c main.cpp $(DEFINES) -o main.o

utils.o: utils.h utils.cpp
	g++ -c utils.cpp $(DEFINES) -o utils.o

commbase.o: commbase.h commbase.cpp
	g++ -c commbase.cpp $(DEFINES) -o commbase.o

oem.o: oem.h oem.cpp
	g++ -c oem.cpp $(DEFINES) -o oem.o

oemp.o: oemp.h oemp.cpp
	g++ -c oemp.cpp $(DEFINES) -o oemp.o

commbase_rs232.o: commbase_rs232.h commbase_rs232.cpp
	g++ -c commbase_rs232.cpp $(DEFINES) -o commbase_rs232.o

rs232.o: rs232.h rs232.c
	g++ -x c++ -c rs232.c $(DEFINES) -o rs232.o

lodepng.o: lodepng.h lodepng.cpp
	g++ -c lodepng.cpp -o lodepng.o

clean:
	rm -rf fingerprint *.o

