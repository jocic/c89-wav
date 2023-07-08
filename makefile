GCC=gcc

OUTPUT=./assets/test.exe

STD=c89

all:
	
	gcc -I source -o $(OUTPUT) -std=$(STD)\
		source/bin.h \
		source/format.h \
		source/tones.h \
		tests/binary-io.h \
		tests/wav-format.h \
		source/bin.c \
		source/format.c \
		source/tones.c \
		tests/binary-io.c \
		tests/wav-format.c \
		main.c