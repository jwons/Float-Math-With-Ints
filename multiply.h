#include <iostream> // For cout
#include <stdlib.h> // For malloc()
#include <limits.h>

using namespace std;

bool multiply(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len);

// ----- C-string functions -----
char numberToChar(int number);
void clearArray(char * array, int len);
void reverseArray(char * array, int len);
bool buildString(int characteristic, int mantissa, char * result, int len);
char * buildCharacteristic(int numerator, int denominator);
char* buildMantissa(int numerator, int denominator, int numberOfCharacters);
int strLen(char * str);
// ------------------------------

// ----- Math Functions -----
int exponent(int base, int power);
int abs(int number);
// --------------------------

// ----- Utility Functions -----
int getNumberOfDigits(int number);
int getValueAtDigit(int number, int placeValue);

int calcMantissa(int numerator, int denominator, int numPlaceValues);
int calcCharacteristic(int numerator, int denominator);

bool isResultNegative(int c1, int c2);
// -----------------------------

// ----- Testing -----
void testFunctions();
// ------------------- 


bool multiply(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
	/*
		Function to multiply two numbers together 
		and then return the result in the c-string
		array variable, result[]

		c1 = characteristic one
		n1 = numerator one
		d1 = denominator one
		... c2, n2, d2 represent the respective 
		secondary values

		Returns - True  if everything was succesful
		        - False if error occoured
		        - False if resulting characteristic 
		                can't fit in the len of result
	*/

	int firstFraction;
	int secondFraction;

	bool retVal;
	bool negative = isResultNegative(c1, c2);

	c1 = abs(c1);
	c2 = abs(c2);

	if (d1 == 0 && c1 != 0)
	{
		// No fractional part
		firstFraction = c1;
	}
	else
	{
		firstFraction = (c1 * d1) + n1;
	}


	if (d2 == 0 && c2 != 0)
	{
		secondFraction = c2;
	}
	else
	{
		secondFraction = (c2 * d2) + n2;
	}


	// Convert the two mixed fractions into two impropper ones
	int totalNumerator   = firstFraction * secondFraction;
	int totalDenominator;

	if (d1 == 0)
	{
		totalDenominator = d2;
	}
	else if (d2 == 0)
	{
		totalDenominator = d1;
	}
	else
	{
		totalDenominator = d1 * d2;
	}

	retVal = buildString(totalNumerator, totalDenominator, result, len);

	if (negative)
	{
		for (int i = len-1; i > 0; i--)
		{
			result[i] = result[i-1];
		}

		result[0] = '-';
	}

	if (c1 != 0 && (c2 > INT_MAX / c1))
	{
		retVal = false;
	}

	return retVal;
}

int getNumberOfDigits(int number)
{
	/*
		This function determines the number of characters 
		needed to store the given number

		number = value to find number of digits

		Returns - Number of digits 
		       

		Ex: 492,239 -> 6
		      9,401 -> 4
		          1 -> 1
	*/

	int currentPlaceValue = 1;
	int retVal = 0;

	number = abs(number);

	while(number >= currentPlaceValue)
	{
		currentPlaceValue *= 10;
		retVal += 1;
	}

	return retVal;
}

bool isResultNegative(int c1, int c2)
{
	// Function to tell if the resulting number
	// will be negative. This function assumes
	// that the negative will be on the characteristic

	bool retVal = false;

	if (c1 > 0 && c2 < 0)
	{
		retVal = true;
	}
	else if (c2 > 0 && c1 < 0)
	{
		retVal = true;
	}

	return retVal;
}

bool buildString(int numerator, int denominator, char * result, int len)
{
	/*
		This function calls the appropriate subroutines
		to create the mantissa and the characteristic 
		and then joins them together into one c-string
		and stores it into result

		Returns - True if everything was succesful
				- False if something went wrong
	*/

	char * characteristic = buildCharacteristic(numerator, denominator);
	char * mantissa       = buildMantissa(numerator, denominator, len);

	int characteristicLen = strLen(characteristic);

	bool retVal = true;

	if (characteristicLen > len)
	{
		// Too large
		retVal = false;
	}
	else
	{
		// I is being declared outside of the scope
		// of these for loops so that I can keep
		// track of where we are in the string without
		// using a secondary variable
		int i = 0;
		for (; i < characteristicLen; i++)
		{
			result[i] = characteristic[i];
		}

		// If we have room to add in a '.'
		if (characteristicLen != len)
		{
			result[i] = '.';
		}

		// Make sure that we dont overwrite the '.'
		i++;

		// Add on the mantissa
		for (; i < len; i++)
		{
			result[i] = mantissa[abs(i-characteristicLen)-1];
		}

	}

	// NO LEAKS
	free(characteristic);
	free(mantissa);

	return retVal;

}
char * buildCharacteristic(int numerator, int denominator)
{
	/*
		numerator,
		denominator,

		Returns - pointer to array containing
		          the characteristic as a
		          c-string

	*/

	int characteristic = calcCharacteristic(numerator, denominator);

	// Doing this here to avoid multiple unneeded calls
	int characteristicDigitCount = getNumberOfDigits(characteristic);

	// +1 to account for the \0
	char * retVal = (char*)malloc(characteristicDigitCount+1);
	clearArray(retVal, characteristicDigitCount+1);

	// Fill the return array with converted numbers
	for (int i = 0; i <= characteristicDigitCount; i++)
	{
		retVal[i] = numberToChar(getValueAtDigit(characteristic, i+1));
	}

	// The array is in reverse order because we grabed numbers
	// right to left rather than left to right
	reverseArray(retVal, characteristicDigitCount+1);

	return retVal;
}

char* buildMantissa(int numerator, int denominator, int numberOfCharacters)
{
	// Get rid of the characteristic
	int dividor = numerator / denominator;
	numerator -= dividor * denominator;

	// Again +1 to account for the \0
	char* retVal = (char*)malloc(numberOfCharacters + 1);

	// Code version of long division
	for (int i = 0; i <= numberOfCharacters; i++)
	{
		int nextVal = numerator / denominator;
		retVal[i] = numberToChar(nextVal);

		numerator -= denominator * nextVal;
		numerator *= 10;
	}

	// Shift over characters to remove the 0 from the characteristic
	for (int i = 0; i <= numberOfCharacters + 1; i++)
	{
		retVal[i] = retVal[i+1];
	}

	// Make sure to add that null terminator
	retVal[numberOfCharacters] = '\0';

	return retVal;
}

int strLen(char * str)
{
	// Function to deturmine the length of a null
	// terminated string
	char * start = str;
	while(*str != '\0')
	{
		str++;
	}
	return (str - start);
}

int getValueAtDigit(int number, int placeValue)
{
	/*
		number = number to get a place value from
		placeValue = location from number to get

			ex: 4582673, 5 = 8

		Returns - Value at the given place value
	*/

	int retVal = 0;

	// Doesnt matter if the number is positive or
	// negative so might as well make it positive
	number = abs(number);

	retVal = number % (exponent(10, placeValue));
	retVal = retVal / exponent(10, placeValue-1);
	
	return retVal;
}

int exponent(int base, int power)
{
	// Calculates the value of this exponential
	int retVal = 1;

	for (int i = 0; i < power; i++)
	{
		retVal = retVal * base;
	}

	return retVal;
}

// Absolute value
int abs(int number)
{
	if (number < 0)
	{
		number *= -1;
	}

	return number;
}

// Calculates the mantissa, this function isnt needed as build mantissa
// has taken its place
int calcMantissa(int numerator, int denominator, int numPlaceValues)
{
	// Get rid of the characteristic
	int dividor = numerator / denominator;
	numerator -= dividor * denominator;

	int retVal = 0;

	for (int i = 0; i <= numPlaceValues; i++)
	{
		int nextVal = numerator / denominator;
		retVal += nextVal;

		if (i != numPlaceValues)
		{
			retVal *= 10;
		}

		numerator -= denominator * nextVal;
		numerator *= 10;
	}

	return retVal;
}

// Calculates the characteristic for the given number
int calcCharacteristic(int numerator, int denominator)
{
	return (numerator / denominator);
}

void clearArray(char * array, int len)
{
	/*
		Helper function to clear an array with ' '
		 also adds a null character
	*/
	const char NULL_TERMINATOR = '\0';

	for (int i = 0; i < len; i++)
	{
		array[i] = ' ';
	}

	array[len-1] = NULL_TERMINATOR;
}

// Reverses an array
void reverseArray(char * array, int len)
{
	// In-place Algorithm
	// https://en.wikipedia.org/wiki/In-place_algorithm
	for (int ix = 0; ix < len / 2; ix++)
	{
	    int t = array[ix];
	    array[ix] = array[len - ix - 1];
	    array[len - ix - 1] = t;
	}

	// The null terminator is now the first character
	// this needs to be moved to the end and the rest
	// shifted over to take its place
	for (int i = 0; i < len; i++)
	{
		array[i] = array[i+1];
	}

	// Add the terminator to the end
	array[len-1] = '\0';
}

// Converts a number to its character equivilant 
char numberToChar(int number)
{
	static const int NUMBER_BASE = 48;

	number = abs(number);

	return (char)(number+NUMBER_BASE);
}

void testFunctions()
{
	// Test of the getNumberOfDigits function
	cout << "----- getNumberOfDigits() -----"<< endl;
	cout << "(4)  1000          ->"<< getNumberOfDigits(1000)       << endl;
	cout << "(5)  45681         ->"<< getNumberOfDigits(45681)      << endl;
	cout << "(8)  73928492      ->"<< getNumberOfDigits(73928492)   << endl;
	cout << "(1)  0             ->"<< getNumberOfDigits(0)          << endl;
	cout << "(10) 1,284,619,360 ->"<< getNumberOfDigits(1284619360) << endl;
	cout << "(5) -1000          ->"<< getNumberOfDigits(-1000)      << endl;
	cout << "(6) -45681         ->"<< getNumberOfDigits(-45681)     << endl;
	cout << "(9) -73928492      ->"<< getNumberOfDigits(-73928492)  << endl;

	cout << endl;

	// Test exponent function
	cout << "----- exponent() -----" << endl;
	cout << "(144) 12^2 ->" << exponent(12, 2) << endl;
	cout << "(16)  2^4  ->" << exponent(2,  4) << endl;
	cout << "(2744)14^3 ->" << exponent(14, 3) << endl;
	cout << "(18)  18^1 ->" << exponent(18, 1) << endl;
	cout << "(1)   27^0 ->" << exponent(27, 0) << endl;
	cout << "(16)  2^4  ->" << exponent(2,  4) << endl;
	cout << "(2744)14^3 ->" << exponent(14, 3) << endl;
	cout << "(1)   0^0  ->" << exponent(0,  0) << endl;

	cout << endl;

	cout << "----- abs() -----" << endl;
	cout << "(1)  1 ->" <<  abs(1) << endl;
	cout << "(1) -1 ->" << abs(-1) << endl;

	cout << endl;

	cout << "----- getValueAtDigit() -----" << endl;
	cout << "(8)  728476, 4 ->" << getValueAtDigit(728476 , 4) << endl;
	cout << "(2)  827   , 2 ->" << getValueAtDigit(827    , 2) << endl;
	cout << "(3)  8273  , 1 ->" << getValueAtDigit(8273   , 1) << endl;
	cout << "(8) -728476, 4 ->" << getValueAtDigit(-728476, 4) << endl;
	cout << "(2) -827   , 2 ->" << getValueAtDigit(-827   , 2) << endl;
	cout << "(3) -8273  , 1 ->" << getValueAtDigit(-8273  , 1) << endl;
	cout << "(0)  12    , 4 ->" << getValueAtDigit(12     , 0) << endl;

	cout << endl;

	cout << "----- numberToChar() -----" << endl;
	cout << "(0)  0 -> " << numberToChar(0)  << endl;
	cout << "(9)  9 -> " << numberToChar(9)  << endl;
	cout << "(1) -1 -> " << numberToChar(-1) << endl;

	cout << endl;

	cout << "----- calcMantissa() -----"   << endl;
	cout << "--Result--Numer---Denom-Place"<< endl;
	cout << "(6472222)  149930, 3600, 7 ->" <<calcMantissa(149930,3600,7) << endl;
	cout << "(3333333)  1,      3,    7 ->" <<calcMantissa(1,3,7)         << endl;
	cout << "(0)        1,      1,    7 ->" <<calcMantissa(1,1,7)         << endl;
	cout << "(2222222)235,     45,    7 ->" <<calcMantissa(235,45,7)      << endl;

	cout << endl;

	char test[20];
	clearArray(test, 20);

	cout << "----- Multiply -----" << endl;
	// Both positive
	multiply(5, 10, 45, 7, 78, 80, test, 20);
	cout << "(41.64722) 5 10/45 * 7 78/80 -> " << test << endl;
	clearArray(test, 20);

	// One Negative
	multiply(5, 10, 45, -1, 0, 0, test, 20);
	cout << "(-5.222)   5 10/45 * -1      -> " << test << endl;
	clearArray(test, 20);

	// Both Negative
	multiply(-5, 10, 45, -1, 0, 0, test, 20);
	cout << "(5.222)   -5 10/45 * -1      -> " << test << endl;
	clearArray(test, 20);

	// One Zero
	multiply(5, 10, 45, 0, 0, 0, test, 20);
	cout << "(0)        5 10/45 * 0       -> " << test << endl;
	clearArray(test, 20);

	cout << endl;

	
}