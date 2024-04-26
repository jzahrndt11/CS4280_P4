CC      = g++
CFLAGS  = -g -Wall -std=c++11
TARGET  = P4
OBJS    = main.o scanner.o parser.o staticSemantics.o buildTree.o codeGeneration.o

all: $(TARGET)

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o:	main.cpp
	$(CC) $(CFLAGS) -c main.cpp

scanner.o: scanner.cpp scanner.h
	$(CC) $(CFLAGS) -c scanner.cpp

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

staticSemantics.o: staticSemantics.cpp staticSemantics.h
	$(CC) $(CFLAGS) -c staticSemantics.cpp

buildTree.o: buildTree.cpp buildTree.h
	$(CC) $(CFLAGS) -c buildTree.cpp

codeGeneration.o: codeGeneration.cpp codeGeneration.h
	$(CC) $(CFLAGS) -c codeGeneration.cpp

clean:
	/bin/rm -f *.o $(TARGET)