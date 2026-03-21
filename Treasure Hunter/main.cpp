#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main()
{
	
	int gold = 1, i = 0, j = 0;
	const int row = 6, col = 6;
	srand((int)time(0));
	int field[row][col];
	int score = 0;

	//Each box getting its values
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			field[i][j] = (rand() % 255) + 1;
			printf("%d\t", field[i][j]); 
		}
		printf("\n");
	}

	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			if (field[i][j] & gold)
			{
				score += field[i][j] >> 4;
				printf("%d is gold! \t The score is: %d\t\n", field[i][j], score);
			}
			else
			{
				printf("%d is a trap!\t", field[i][j]);
			}
		}
		printf("\n");
	}
	//Checking the bits

	return 0;
}