CC = gcc
LIBS = -lncurses -lm

OBJ = app.o game.o utils.o ball.o paddle.o

%.o: %.c %.h
	$(CC) -c -o $@ $<

app: app.o 
	$(CC) app.c -o app.o

game: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o