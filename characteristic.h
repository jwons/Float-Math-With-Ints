#pragma once
#include <iostream>

using namespace std;

//define ascii values
const int ASCII_ZERO = '0';
const int ASCII_NINE = '9';
const int ASCII_DECIMAL = '.';
const int ASCII_DASH = '-';

//characteristic function prototype
bool characteristic(char numString[], int& c);

//helper function prototypes
bool isNumber(char numStr[], int len, int& decimalLen);
int toInt(char numStr[], int len);
int findStringLength(char str[]);


//function to find the characteristic of a float expressed as an array of chars
bool characteristic(char numString[], int& c)
{
	//return false by default
	bool retVal = false;

	//find length of string
	int len = findStringLength(numString);

	//if all chars are either numbers, a decimal, or a negative sign
	if (isNumber(numString, len, len))
	{
		//convert char array to an int
		c = toInt(numString, len);

		retVal = true;
	}

	return retVal;
}

//returns false if a char array holds anything other than a number, a decimal, or a negative sign
bool isNumber(char numStr[], int len, int& decimalLen)
{
	//return true by default
	bool retVal = true;

	//check for a negative sign at the beginning of the char array
	if (numStr[0] != ASCII_DASH && (numStr[0] < ASCII_ZERO || numStr[0] > ASCII_NINE))
	{
		retVal = false;
	}

	//step through char array
	for (int i = 1; i < len; i++)
	{
		//if char is a decimal
		if (numStr[i] == ASCII_DECIMAL)
		{
			decimalLen = i;
		}
		//if char isn't a number
		else if (numStr[i] < ASCII_ZERO || numStr[i] > ASCII_NINE)
		{
			retVal = false;
			break;
		}
	}

	return retVal;
}

//turns a char array with a decimal into an int
int toInt(char numStr[], int len)
{
	int result = 0;
	int digit;
	int orderOfMagnitude = 1;

	//step through array
	for (int i = len - 1; i >= 0; i++)
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