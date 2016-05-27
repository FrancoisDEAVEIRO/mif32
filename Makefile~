EXECS= proj
MPICC?=mpiCC

all: ${EXECS}

proj: main.cpp
	${MPICC} -o proj main.cpp Grille.cpp Noeud.cpp

clean:
	rm ${EXECS}
