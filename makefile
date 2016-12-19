#------------------------------------------------------------------------------

CC=g++ -w 
BIN=./obj
SRC=./src
EXE=./output
LIBS= -lpthread -lGL -lGLU -lglut
DOC_SRC=./Doc/
DOC_DST=./
SRC_PDF=./Doc/main.pdf
DST_PDF=./design.pdf

override NUM_THREADS ?= 5

#------------------------------------------------------------------------------
# make execute NUM_THREADS=... for building a new exe. make doc for design document. make clean to remove all .obj and .exe files 


execute: CC+= -DCOUNT=$(NUM_THREADS)

execute: $(EXE)/run
	./$(EXE)/run

debug: CC+= -DDEBUG

debug: execute

random: CC+= -DRANDOM

random: execute

$(EXE)/run: $(BIN)/main.o $(BIN)/display.o $(BIN)/balls.o $(BIN)/physics.o $(BIN)/buttons.o
	$(CC) $(BIN)/main.o $(BIN)/display.o $(BIN)/balls.o $(BIN)/physics.o $(BIN)/buttons.o -o $(EXE)/run $(LIBS)

run1:
	mkdir obj

run2: 
	mkdir output

$(BIN)/main.o: $(SRC)/main.cpp run1 run2
	$(CC) -c $(SRC)/main.cpp -o $(BIN)/main.o $(LIBS)

$(BIN)/display.o: $(SRC)/display.cpp
	$(CC) -c $(SRC)/display.cpp -o $(BIN)/display.o $(LIBS)

$(BIN)/balls.o: $(SRC)/balls.cpp
	$(CC) -c $(SRC)/balls.cpp -o $(BIN)/balls.o $(LIBS)

$(BIN)/physics.o: $(SRC)/physics.cpp
	$(CC) -c $(SRC)/physics.cpp -o $(BIN)/physics.o $(LIBS)

$(BIN)/buttons.o: $(SRC)/buttons.cpp
	$(CC) -c $(SRC)/buttons.cpp -o $(BIN)/buttons.o $(LIBS)

doc: $(DOC_SRC)main.tex $(DOC_SRC)flow.jpg $(DOC_SRC)final.jpg $(DOC_SRC)final2.jpg $(DOC_SRC)final3.jpg
	cd Doc && pdflatex main.tex && cd .. ; mv $(SRC_PDF) $(DST_PDF)

clean: clean2
	rm $(DOC_SRC)/main.aux $(DOC_SRC)/main.toc $(DOC_SRC)/main.log $(DOC_DST)/log.txt

clean2: 
	rm -r obj output
