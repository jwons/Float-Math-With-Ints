#pragma once
#include <iostream>

using namespace std;

//define ascii values
const int ASCII_ZERO = '0';
const int ASCII_NINE = '9';
const int ASCII_DECIMAL = '.';
const int ASCII_DASH = '-';
const int ASCII_PLUS = '+';
const int ASCII_SPACE = ' ';

//characteristic function prototype
bool characteristic(char numString[], int& c);

//helper function prototypes
bool isNumber(char numStr[], int len, int& startIndex, int& decimalLen);
int toInt(char numStr[], int startIndex, int len);
int findStringLength(char str[]);


//function to find the characteristic of a float expressed as an array of chars
bool characteristic(char numString[], int& c)
{
	//return false by default
	bool retVal = false;

	//find length of string
	int len = findStringLength(numString);

	//initialize index to start converting to int at (in case there are valid but non-number digits in front)
	int startIndex = 0;

	//if all chars are either numbers, a decimal, or a negative sign
	if (isNumber(numString, len, startIndex, len))
	{
		//convert char array to an int
		c = toInt(numString, startIndex, len);

		retVal = true;
	}

	return retVal;
}

//returns false if a char array holds anything other than a number, a decimal, or a negative sign
bool isNumber(char numStr[], int len, int& startIndex, int& decimalLen)
{
	//return true by default
	bool retVal = true;

	//create index variable
	int index = 0;

	//skip through any spaces at the beginning
	while (numStr[index] == ASCII_SPACE)
	{
		index++;
	}

	//if the first proper char is invalid (the first proper char is the only one that can be a positive or negative sign)
	if (numStr[index] != ASCII_DASH && numStr[index] != ASCII_PLUS && numStr[index] != ASCII_DECIMAL &&
		(numStr[index] < ASCII_ZERO || numStr[index] > ASCII_NINE))
	{
		retVal = false;
	}
	//if the first proper char is a decimal there's no need for the loop
	else if (numStr[index] == ASCII_DECIMAL)
	{
		startIndex = index;
		decimalLen = index;
	}
	else
	{
		//if the first proper char is a number or a negative sign toInt should start here
		if ((numStr[index] > ASCII_ZERO && numStr[index] < ASCII_NINE) || numStr[index] == ASCII_DASH)
		{
			startIndex = index;
		}
		else
		{
			startIndex = index + 1;
		}

		//increment index to start loop on the second proper char
		index++;

		//step through the rest of the char array
		for (; index < len; index++)
		{
			//if char is a space, skip it
			if (numStr[index] == ASCII_SPACE)
			{
				continue;
			}
			//if char is a decimal
			else if (numStr[index] == ASCII_DECIMAL)
			{
				//we've found the end of the characteristic
				decimalLen = index;
				break;
			}
			//if char isn't a number
			else if (numStr[index] < ASCII_ZERO || numStr[index] > ASCII_NINE)
			{
				//the input is invalid
				retVal = false;
				break;
			}
		}
	}

	return retVal;
}

//turns a char array with a decimal into an int
int toInt(char numStr[], int startIndex, int len)
{
	int result = 0;
	int digit;
	int orderOfMagnitude = 1;

	//step through array
	for (int i = len - 1; i >= startIndex; i--)
	{
		//if char is a space, skip it
		if (numStr[i] == ASCII_SPACE)
		{
			continue;
		}
		//if char is a negative sign, make result negative
		else if (numStr[i] == ASCII_DASH)
		{
			result = result * -1;
		}
		else
		{
			//turn char into an int
			digit = numStr[i] - ASCII_ZERO;

			//put int at the correct order of magnitude
			digit = digit * orderOfMagnitude;

			//add digit to result
			result = result + digit;

			//increase the order of magnitude
			orderOfMagnitude = orderOfMagnitude * 10;
		}
	}

	return result;
}

//finds the length of an array of chars
int findStringLength(char str[])
{
	int len = 0;

	//look through array until ending '\0'
	while (str[len] != '\0')
	{
		len++;
	}

	return len;
}