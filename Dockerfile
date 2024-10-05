FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y \
    gcc \
    build-essential \
    libsdl2-dev \
    libsdl2-ttf-dev \
    libsdl2-image-dev \
    && apt clean

ENV DISPLAY=host.docker.internal:0

WORKDIR /home/polynomial-master

COPY . .

RUN gcc -o polynomial-master src/main.c src/graphic_lib.c src/graphic_mode.c src/logic_lib.c src/terminal_mode.c -Iinclude -lSDL2 -lSDL2_ttf -lSDL2_image -lm

CMD [ "./polynomial-master" ]
