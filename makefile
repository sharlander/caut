# variables

SRC_MAIN = build/main.o build/factertofile.o build/help.o build/readmanifest.o build/version.o build/caut_getfact.o
SRC_FILE = build/file/file.o build/file/filechown.o build/file/parsetemplate.o build/file/mkdir.o build/file/delete.o
SRC_PACKAGE = build/package/yuminstall.o build/package/package.o build/package/yumuninstall.o build/package/uninstall.o build/package/install.o
SRC_SERVICE = build/service/service.o build/service/start.o  build/service/stop.o build/service/disable.o build/service/enable.o

CC = gcc
OBJ = $(SRC_MAIN) $(SRC_FILE) $(SRC_PACKAGE) $(SRC_SERVICE)
CFLAGS =
DEPS = src/structs.h

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
