# syntax=docker/dockerfile:1
FROM gcc:4.9 AS compiler
ENV SRC_NAME="Source.c"
COPY ./src /usr/src/myapp
WORKDIR /usr/src/myapp
RUN apt-get update -y \
    && apt-get install -y mingw-w64 \
    && mkdir out 
RUN gcc -o out/myapp ${SRC_NAME} -std=c99 -w -lm \
    && x86_64-w64-mingw32-gcc -o out/main64.exe ${SRC_NAME} -std=c99 -w -lm \
    && i686-w64-mingw32-gcc -o out/main32.exe ${SRC_NAME} -std=c99 -w -lm \
    && mkdir /out \
    && mv out/* /out/

