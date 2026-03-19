#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main()
{
	const int row = 6, col = 6;
	srand((int)time(0));
	int field[row][col];

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			field[i][j] = (rand() % 255);
			printf("%d\n", field[i][j]);
		}
	}



	return 0;
}