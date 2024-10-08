FROM alpine:latest

RUN apk update && apk add --no-cache \
    gcc \
    g++ \
    make \
    sdl2-dev \
    sdl2_ttf-dev \
    sdl2_image-dev \
    mesa-gl \
    mesa-dri-gallium \
    libx11-dev

ENV DISPLAY=:0

WORKDIR /home/polynomial-master

COPY . .

RUN gcc -o polynomial-master src/main.c src/graphic_lib.c src/graphic_mode.c src/logic_lib.c src/terminal_mode.c -Iinclude -lSDL2 -lSDL2_ttf -lSDL2_image -lm

CMD ["./polynomial-master"]