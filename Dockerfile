FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y \
    gcc \
    libsdl2-dev \
    libsdl2-ttf-dev \
    libsdl2-image-dev \
    && apt clean

ENV DISPLAY=host.docker.internal:0

WORKDIR /home/program

COPY . .

RUN gcc -o program src/Programme_Principale.c -lSDL2 -lSDL2_ttf -lSDL2_image -lm

CMD [ "./program" ]
