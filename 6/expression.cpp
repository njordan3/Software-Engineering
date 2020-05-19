//author: mystery programmer
//date: unknown
//
//
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(void)
{
	printf("\n\n3350 sloppy program.\n\n");
	int a,b,c,d,e,f;
	srand((unsigned)time(NULL));
	a = rand() % 100;
	b = rand() % 100;
	c = rand() % 100;
	d = ((a + b) | c);
	e = ((b % (a+b)) | c);
	f = (((c % 3) << !1) | ~d);
	printf("values: %i %i %i %i %i %i\n", a, b, c, d, e, f);
	//-----------------------------------------------------------------
	if (a == b) {
		if (c == d) {
			printf("---hello---\n");
        }
    } else {
		e = 1;
    }
	//-----------------------------------------------------------------
	if (a > (b & c))
		printf("a is > b bitwise and c\n");
	//-----------------------------------------------------------------
	e = ((a > b) ? a : d);
	//-----------------------------------------------------------------
	if (((a == b) || (b == c)) && ((c == d) || (d == e)))
		printf("success!\n");
	//-----------------------------------------------------------------
	printf("final output:\n");
	printf("a: %i\n", a);
	printf("b: %i\n", b);
	printf("c: %i\n", c);
	printf("d: %i\n", d);
	printf("e: %i\n", e);
	printf("f: %i\n", f);
	printf("\n");
	return 0;
}
// yes even though i didn't even do it because i completely forgot about it until last night and i assume i won't get points for it even if i do it now because it's several days late now
