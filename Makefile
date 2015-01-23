default: threestacks

threestacks: threestacks.c
	gcc -o $@ $+

clean: threestacks
	rm $+
