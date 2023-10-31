# Numele executabilului
EXEC = program

# Compiler
CC = g++

# Opțiuni pentru compiler
CFLAGS = -Wall -std=c++11

# Fișiere sursă și header
SRC = main.cpp LexicalAnalyzer.cpp
HEADERS = LexicalAnalyzer.h utils.h

# Regula pentru compilarea programului
$(EXEC): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

# Regula pentru ștergerea executabilului și a fișierelor obiect
clean:
	rm -f $(EXEC) *.o

# Regula pentru rularea programului
run: $(EXEC)
	./$(EXEC)
