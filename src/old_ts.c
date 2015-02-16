/*
 * threestacks.c
 * Version 1.1
 *
 * Calculate the number of all possibilities
 * of dividing some balls into some boxes.
 *
 * Written by 叶靖琛.
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#define TS_VER "1.1"

/* used in getarg() */
#define ARG_NOTHING 0
#define ARG_BALL 1
#define ARG_BOX 2

/* used in struct argresult */
#define MODE_VERSION (1)
#define MODE_HELP (2)
#define MODE_ARGERR (3)

/* store command line arguments */
struct argresult {
	int ball;
	int box;

	int mode;
	bool verbose;
};

/*
 * get command line arguments and store
 * them to struct ar
 */
void
getarg(int argc, char *argv[], struct argresult *ar)
{
	int i, state = ARG_NOTHING;
	
	for (i = 1; i < argc; i++) {
		if (state == ARG_NOTHING) {
			if (strcmp(argv[i], "-b") == 0) {
				state = ARG_BALL;
			} else if (strcmp(argv[i], "-x") == 0) {
				state = ARG_BOX;
			} else if ((char *) strstr(argv[i], "--ball=") == argv[i]) {
				ar->ball = atoi(argv[i] + strlen("--ball="));
			} else if ((char *) strstr(argv[i], "--box=") == argv[i]) {
				ar->box = atoi(argv[i] + strlen("--box="));
			} else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
				ar->mode = MODE_HELP;
			} else if ((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--version") == 0)) {
				ar->mode = MODE_VERSION;
			} else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--verbose") == 0)) {
				ar->verbose = true;
			} else {
				ar->mode = MODE_ARGERR;
			}

			continue;
		}

		switch (state) {
		case ARG_BALL:
			ar->ball = atoi(argv[i]);
			break;

		case ARG_BOX:
			ar->box = atoi(argv[i]);
			break;
		}
		
		state = ARG_NOTHING;
	}
}

void
printuse(char *pname)
{
	printf("Usage:\n\t%s -b number -x number\n", pname);
	printf("Or:\n\t%s --box=number --ball=number\n", pname);
}

void
printver(void)
{
	printf("threestacks %s\n", TS_VER);
}

void
printhlp(char *pname)
{
	printver();
	printuse(pname);
	
	printf("\nOptions:\n");
	printf("\t-b num, --ball=num\tSpecify the number of the balls\n");
	printf("\t-x num, --box=num\tSpecify the number of boxes\n");
	printf("\t-l, --verbose\tDisplay every possible result to stdout\n");
	
	printf("\t-h, --help\tDisplay this help\n");
	printf("\t-v, --version\tDisplay version infomation\n");
}

void
prtarray(int *a, int len)
{
	int i;
	for (i = 0; i < (len-1); i++)
		printf("%3d ", a[i]);
	printf("%3d\n", a[i]);
}

int
threes(struct argresult *arg, int *box, int prevsum, int lv)
{
	static int i = 0;

	if (lv == arg->box) {
		for (box[lv] = box[lv-1] + 1; box[lv] <= arg->ball; ++(box[lv])) {
			if ((prevsum + box[lv]) == arg->ball) {
				i++;
				if (arg->verbose)
					prtarray(box+1, arg->box);
			}
		}
	} else if (lv >= 1) {
		for (box[lv] = box[lv-1] + 1;
				box[lv] <= (arg->ball / (arg->box - lv + 1));
				box[lv]++) {
			threes(arg, box, prevsum + box[lv], lv+1);
		}
	}
	
	return i;
}

int
main(int argc, char *argv[])
{
	if (argc == 1) {
		printuse(argv[0]);
		return 1;
	}

	struct argresult ar = { 0, 0, 0, false };

	getarg(argc, argv, &ar);

	/* message mode */
	switch (ar.mode) {
	case MODE_ARGERR:
		printuse(argv[0]);
		return 1;
		break;
	
	case MODE_HELP:
		printhlp(argv[0]);
		return 0;
		break;
	
	case MODE_VERSION:
		printver();
		return 0;
		break;
	}

	if (ar.box <= 0) {
		fprintf(stderr, "Invalid number of box\n");
		return 1;
	}

	if (ar.ball <= 0) {
		fprintf(stderr, "Invalid number of ball\n");
		return 1;
	}

	int *boxes = calloc(ar.box+1, sizeof(*boxes));
	if (boxes == NULL) {
		fprintf(stderr, "%s: Error creating array boxes[], "
				"calloc failed\n", argv[0]);
		return 1;
	}

	boxes[0] = 0;

	int result = threes(&ar, boxes, 0, 1);

	free(boxes);
	printf("%d possibilities in total\n", result);
	printf("%d balls and %d boxes\n", ar.ball, ar.box);

	return 0;
}