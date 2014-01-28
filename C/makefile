CC = gcc
CFLAGS = -g -Wall

all: puzzle

puzzle: puzzle_solver.o
	${CC} ${CFLAGS} -o puzzle puzzle_backend.o puzzle_solver.o

puzzle_solver.o: P1-1.c
	${CC} ${CFLAGS} -c P1-1.c
