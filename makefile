compile: writer.c semaphone.c
	gcc semaphone.c
	gcc writer.c
clean:
	rm a.out
	rm story
	rm *~
