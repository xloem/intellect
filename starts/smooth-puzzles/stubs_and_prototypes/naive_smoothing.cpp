#include <stdlib.h>

int main()
{
	FILE * inspiration = fopen("confusing_numbers", "rb");

	double idea;

	ssize_t size;

	while (fread(inspiration, &idea, sizeof(idea)) == sizeof(idea)) {
		// makes random smooth numbers without bound.  most will be either gigantic or broken.
	}
}
