#include <stdio.h>
#include <string.h>

void swap(char* x, char* y) {
	char temp = *x;
	*x = *y;
	*y = temp;
}

void permut(char* str, int currentIndex, int maxIndex) {
	if (currentIndex == maxIndex)
		puts(str);
	else {
		int i;
		for (i = currentIndex; i <= maxIndex; ++i) {
			swap(str + currentIndex, str + i);
			permut(str, currentIndex + 1, maxIndex);
			swap(str + i, str + currentIndex);
		}
	}
}

int main(int argc, char const *argv[])
{
	char src[11];

	printf("Enter a string (max length 10): ");
	scanf("%10s",src);

	printf("The permutations are as follows: \n");

	int len = strlen(src);
	
	permut(src, 0, len - 1);

	return 0;
}