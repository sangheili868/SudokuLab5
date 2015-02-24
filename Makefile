CMP = g++
MAIN = main
FUNC = Puzzle
EXEC = $(MAIN)
EXEC2 = $(MAIN2)
EXEC3 = $(MAIN3)
FLAGS = -lm -lX11

all:	$(MAIN)

$(EXEC): $(MAIN).o $(FUNC).h
	$(CMP) $(MAIN).o $(FUNC).h $(FLAGS) -o $(EXEC)

$(MAIN).o: $(MAIN).cpp $(FUNC).h
	$(CMP) -c $(MAIN).cpp

clean:
	rm *.o $(EXEC)
 
