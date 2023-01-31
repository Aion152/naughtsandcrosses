CC = gcc
CCFLAGS = -std=c99 -lm -Wall -Werror -Wpedantic
AL = $$(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)

SOURCE = naughtsandcrosses.c
BIN = naughtsandcrosses.exe

all: naughtsandcrosses

clean:
	-rm naughtsandcrosses.exe

naughtsandcrosses: naughtsandcrosses.c
	${CC} ${SOURCE} ${CCFLAGS} ${AL} -o ${BIN}
