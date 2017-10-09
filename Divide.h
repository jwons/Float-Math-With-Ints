#pragma once
#include <iostream>

using namespace std;

//function prototypes
char convert(int num);
void addToArray(char num, int counter, char answer[]);
bool divide(int c1, int n1, int d1, int c2, int n2, int d2, char answer[], int num);

bool divide(int c1, int n1, int d1, int c2, int n2, int d2, char answer[], int length)
{
	bool retVal = true;
	if (d1 == 0 || d2 == 0)
	{
		retVal = false;
	}

	if (retVal == true)
	{
		int newNumerator1;
		int newNumerator2;
		int finalNumerator;
		int finalDenominator;
		answer[length - 1] = '\0';

		//account for negatives
		bool isNegative = false;
		if (c1 < 0)
		{
			isNegative = true;
			c1 = c1 * -1;
		}
		if (c2 < 0)
		{
			isNegative = true;
			c2 = c2* -1;
		}

		//calculate improper fractions to find the new numerators
		newNumerator1 = (c1 * d1) + n1;
		newNumerator2 = (c2 * d2) + n2;

		//flip second fraction multiply to get final fraction
		finalNumerator = newNumerator1 * d2;
		finalDenominator = d1 * newNumerator2;

		int num = 1;
		int counter = 0;

		//if negative, add '-' to the array at position 0
		if (isNegative == true)
		{
			addToArray('-', counter, answer);
			++counter;
		}

		//first # 
		num = finalNumerator / finalDenominator;
		addToArray(convert(num), counter, answer);

		++counter;

		//decimal
		addToArray('.', counter, answer);

		++counter;

		int nextNum = num;
		int remainder = finalNumerator;

		//loop to find the next decimal numbers until the length inputed is reached
		for (int i = counter; i < length; i++)
		{
			nextNum = nextNum * finalDenominator;
			nextNum = remainder - nextNum;

			if (counter == length - 1)
			{
				addToArray('\0', length - 1, answer);
				break;
			}

			//test to see if the remainder is 0 
			if (nextNum == 0)
			{
				if (counter == length - 1)
				{
					addToArray('\0', length - 1, answer);
				}
				else
				{
					addToArray('\0', counter, answer);
				}
				break;
				//end array
			}

			nextNum = nextNum * 10;

			//save nextNum to subNum to use in the next calculation
			remainder = nextNum;

			nextNum = nextNum / finalDenominator;

			//add num to array and add 1 to the counter
			addToArray(convert(nextNum), counter, answer);
			++counter;
		}
	}

	return retVal;
}

char convert(int num)
{
	char retVal;
	retVal = num + 48;
	return retVal;
}

void addToArray(char num, int counter, char answer[])
{
	cout << "num " << num << endl;
	answer[counter] = num;
}