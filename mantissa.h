#pragma once

const int ASCII_ZERO_DECIMAL = 48;
const int ASCII_NINE_DECIMAL = 57;

//This functions loops through the numString and has three features:
//1. Will return false if there is an invalid character
//2. Stores the start point of the mantissa so that it can be extracted
//3. Stores the length from the mantissa so that the right amount of numbers are converted 
bool verifyCharsAndLength(char numString[], int& pointOfMantissa, int& length);

//This function will take the numString variable and extract the mantissa from it
//The value of the numerator will be the interger version of what is after the decimal
//The denominator will be what place value the numerator is to
//This function will return false if an invalid c string is passed
bool mantissa(char numString[], int& numerator, int& denominator)
{
	bool retVal = false;
	//This integer will be the index to the first element in the mantissa
	int startOfMantissa;
	//This integer will be the length of the mantissa NOT including trailing zeros
	int length = 0;

	//If the c string is valid use the information from the verification to convert to an int
	if (verifyCharsAndLength(numString, startOfMantissa, length))
	{
		retVal = true;

		//In case something is passed that is not intialized or is set to something other than 0, 
		//assign zero to the numerator as values will be added to it
		numerator = 0;
		//The denominator will be multiplied by 10 to get the correct power of 10 for the corresponding
		//place value, it will have to start at 1 for the multiplication to work correctly
		denominator = 1;
		
		//Move backwards from the last value of the mantissa (NOT including trailing zeros) to the
		//to the first value of the mantissa 
		for (int i = startOfMantissa + length - 1; i >= startOfMantissa; i--)
		{
			//Each iteration add whatever number you're on after being multplied by its respective place value
			numerator += (numString[i] - ASCII_ZERO_DECIMAL) * denominator;

			//increase by a power of 10 to keep track of place values each iteration
			denominator *= 10;
		}

		//In case the mantissa is empty, the denominator should be 10
		if (denominator == 1)
		{
			denominator = 10;
		}
	}

	return retVal;
}

bool verifyCharsAndLength(char numString[], int& startOfMantissa, int& length)
{
	const int DECIMAL_POINT = '.';
	bool retVal = true;

	//This number represents how many trailing zeros are in the c string
	int numOfTrailingZeros = 0;

	//Represents the element currently at while looping through the characteristic
	int characteristicIndex = 0;

	//The mantissa is not found until after the decimal point, ignore everything up until then
	while (numString[characteristicIndex] != DECIMAL_POINT)
	{
		//In case there is no mantissa in the array, just end the function
		if (numString[length] == '\0')
		{
			retVal = false;
			break;
		}
		//If nothing happens, increment the index so that the next element can be examined
		characteristicIndex++;
	}

	//If we reached the end before finding a decimal, do not increment
	//This way the next loop is skipped
	if (!numString[characteristicIndex] == '\0')
	{
		characteristicIndex++;
	}

	//Move to the next element of the array now that the start of mantissa has been found
	//Record the mantissa start as well
	int mantissaIndex = characteristicIndex;
	startOfMantissa = characteristicIndex;

	//The function should stop searching at the end of the array marked by the zero terminator
	while (numString[mantissaIndex] != '\0')
	{
		//Now that the mantissa is found, start checking the validity of the chars
		//Make sure it is within range of the ASCII char values for 0 through 9
		if (!(numString[mantissaIndex] >= ASCII_ZERO_DECIMAL && numString[mantissaIndex] <= ASCII_NINE_DECIMAL))
		{
			retVal = false;
			break;
		}

		//If the number is a zero, it could be a trailing zero so increment the trailing zeros variable
		if (numString[mantissaIndex] == ASCII_ZERO_DECIMAL)
		{
			numOfTrailingZeros++;
		}

		//If it's not a zero set the number back to 0 just in case zeros were incrmented before
		else if (numOfTrailingZeros > 0)
		{
			numOfTrailingZeros = 0;
		}

		//Continue on to the next element of the array
		mantissaIndex++;
	}

	//We do not want to count the null terminator as part of the length
	//However, we would have to add one to the lenght to count for array start at 0
	//Subtract the start of mantissa and number of zeros to find the legnth of significant numbers
	length = mantissaIndex - (startOfMantissa + numOfTrailingZeros);

	return retVal;
}
