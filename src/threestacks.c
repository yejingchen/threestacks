/*
 * Threestacks
 */

#include <stdio.h>
#include <stdlib.h>
#include "argtable3.h"

#if HAVE_CONFIG_H
# include "config.h"
#endif

#define PROGVERS VERSION
#define PROGNAME PACKAGE_NAME

void
prtiarray(int a[], int len)
{
	int i;
	for (i = 0; i < len-1; i++)
		printf("%d ", a[i]);

	printf("%d\n", a[i]);
}

int
threestacks(int nballs, int nboxes, int box[], int verbose, int prevsum, int lv)
{
	static int i = 0;

	if (lv == nboxes) {
		for (box[lv] = box[lv-1] + 1; box[lv] <= nballs; ++(box[lv])) {
			if ((prevsum + box[lv]) == nballs) {
				i++;
				if (verbose)
					prtiarray(box+1, nboxes);
			}
		}
	} else if (lv >= 1) {
		for (box[lv] = box[lv-1] + 1;
				box[lv] <= (nballs / (nboxes - lv + 1));
				box[lv]++) {
			threestacks(nballs, nboxes, box, verbose, prevsum + box[lv], lv+1);
		}
	}
	
	return i;
}

int
main(int argc, char *argv[])
{
	/* Begin command line argument parsing */
	struct arg_int *b, *x;
	struct arg_lit *help, *ver, *verb;
	struct arg_end *end;

	b = arg_intn("b", "ball", "<n>", 0, 1, "The number of balls");
	x = arg_intn("x", "box", "<n>", 0, 1, "The number of boxes");

	help = arg_litn("h", "help", 0, 1, "Print this help and exit");
	ver = arg_litn(NULL, "version", 0, 1, "Print version and exit");
	verb = arg_litn("v", "verbose", 0, 1, "Print every possibilities");

	end = arg_end(20);

	void *argtable[] = { b, x, verb, ver, help, end };
	int nerrors = 0;

	if (arg_nullcheck(argtable) != 0) {
		fprintf(stderr, "%s: memory allocation failed, NULL argtable entry\n", PROGNAME);
		return 1;
	}

	b->ival[0] = 0;
	x->ival[0] = 0;

	nerrors = arg_parse(argc, argv, argtable);
	/* End command line argument parsing */

	/* "--help" detected: print help and exit. Don't do anything. */
	if (help->count > 0) {
		printf("%s - calculates the number of possibilities \n"
			   "of dividing some balls into some boxes\n\n %s", PROGNAME, PROGNAME);
		arg_print_syntax(stdout, argtable, NULL);
		printf("\n");
		arg_print_glossary_gnu(stdout, argtable);

		return 0;
	}

	/* "--version" detected: print version and exit. Don't do anything else */
	if (ver->count > 0) {
		printf("%s %s\n", PROGNAME, PROGVERS);
		return 0;
	}

	/* If any error occurs while parsing */
	if (nerrors > 0) {
		arg_print_errors(stderr, end, PROGNAME);
		fprintf(stderr, "Type \"%s --help\" for infomation\n", PROGNAME);
		return 1;
	}

	/* Check whether number of ball and box is correctly set */
	if ((b->ival[0] <= 0) || (x->ival[0] <= 0)) {
		fprintf(stderr, "%s: invalid number of ball or box\n", PROGNAME);
		return 1;
	}

	/* Start calculation */
	int *boxarray = calloc((x->ival[0]) + 1, sizeof(*boxarray));
	if (boxarray == NULL) {
		fprintf(stderr, "%s: memory allocation failed, calloc failed\n", PROGNAME);
		return 1;
	}

	int result;
	result = threestacks(b->ival[0], x->ival[0], boxarray, verb->count, 0, 1);

	printf("%d\n", result);

	if (boxarray)
		free(boxarray);

	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

	return 0;
}
