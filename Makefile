all: snake-app.out

CC = g++
SRC = *.cpp
OBJ = *.o
LIB = -lsfml-graphics -lsfml-window -lsfml-system -lpthread 
LFLAGS =
CFLAGS = -Wall -std=c++17

snake-app.out: $(OBJ)
	$(CC) -o snake-app.out $(OBJ) $(LFLAGS) $(LIB)
$(OBJ): $(SRC)
	$(CC) -c $(SRC) $(CFLAGS)

clean:
	rm -f $(OBJ) snake-app.out

