CC = gcc -Wall
OBJETS = sacDos.o probleme.o simplexeGLPK.o

sacDos : $(OBJETS)
	$(CC) -lglpk -o sacDos $(OBJETS)

sacDos.o: sacDos.c sacDos.h probleme.h types.h
	$(CC) -c sacDos.c
	
probleme.o: probleme.c probleme.h types.h
	$(CC) -c probleme.c

pivot.o: simplexeGLPK.c simplexeGLPK.h types.h
	$(CC) -c simplexeGLPK.c

clean:
	rm sacDos $(OBJETS) *~
