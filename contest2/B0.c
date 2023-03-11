#include <stdio.h>
int main(void)
{
	int mark, condition = 0;
	int c = 0;
	long long h = 0;
	do {
		scanf("%d", &mark);
		h = (((int) mark) << 31) | (int) condition;
		switch (h)
		{
		case (((((int) 1 ) << 31) | (int) 1 )) : condition = 0; break;
		case (((((int) 2 ) << 31) | (int) 1 )) : condition = 1; break;
		default: c = 1; break;
		}
		if (c == 1)
			break;
	} while (mark != 0);
	if (c = 0)
		printf("YES");
	else
		printf("NO");
}