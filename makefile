# variables

CC = gcc
OBJ = build/main.o build/factertofile.o	build/filechown.o build/help.o build/parsetemplate.o build/readmanifest.o build/version.o
CFLAGS = 
DEPS =

# make

caut: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

build/%.o: src/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# make install

install:
	install -m 0755 -o root -g root caut /usr/bin/caut

# make clean

clean:
	rm -f caut
	find build -name *.o -exec rm -f {} \;
