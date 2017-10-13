#pragma once

const int ASCII_ZERO = 48;
const int ASCII_NINE = 57;
const int ASCII_DECIMAL = 46;
const int ASCII_NEGATIVE_SIGN = 45;

bool add(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len);
bool isPowerOf10(int d);
void convertDenominatorToPowerOf10(int& c, int& n, int& d, int lengthOfDecimalPrecision);
int determineDifferenceInPower(int& n1, int& d1, int& n2, int& d2);
void convertIntegerToCharacterArray(char arrayOfConvertedNumbers[], int &index, int testNumber);
void reverseArrayIntoResultArray(char arrayToReverse[], char resultArray[], int counter, bool isMantisa, bool isNegative, bool isTerminating, int len);

using namespace std;

bool add(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
	//'x' will be the flag i will check for to help determine if all values were set in an array or not
	for (int i = 0; i < len; i++)
	{
		result[i] = 'x';
	}

	//holds the flag if a decimal is terminating. Even if a number like 2/3 is non terminating, if the result array length is less than the INT_MAX / 10 then it is fine 
	bool isTerminating = true;

	//Before attempting to add two numbers together. Make sure that every number that enters the function has a denominator that is a power of 10
	if (!isPowerOf10(d1))
	{
		//calling a function that will set the numerator and denominator to their power of 10 equivalent (ie. 1/8 = 0.125 = 125/1000)
		convertDenominatorToPowerOf10(c1, n1, d1, len);

		//if the denominator was forced to stop before the array end, then remaining values in the array must be set as it is a repeating decimal
		if (d1 > INT_MAX / 10)
		{
			isTerminating = false;
		}
	}

	if (!isPowerOf10(d2))
	{
		//calling a function that will set the numerator and denominator to their power of 10 equivalent (ie. 1/8 = 0.125 = 125/1000)
		convertDenominatorToPowerOf10(c2, n2, d2, len);

		//if the denominator was forced to stop before the array end, then remaining values in the array must be set as it is a repeating decimal
		if (d2 > INT_MAX / 10)
		{
			isTerminating = false;
		}
	}

	bool retval = true;

	//hold the value of the differenceInPower between the highest order denominator and lowest order denominator
	int differenceInPower = determineDifferenceInPower(n1, d1, n2, d2);

	//if determineDifferenceInPower sets the return value to 0, send a return value of false because this value would cause the program to crash when dividing by zero
	if (differenceInPower == 0)
	{
		retval = false;
	}

	//setting gcd (greatest common denominator) to use as it can be read in person more easily than d1 or d2
	int gcd = d1;

	//add the numerators together
	int newNumerator = n1 + n2;

	//store the newNumerator as having a wholeInteger and a decimal remainder. 
	//If the numerator is greater than the denominator, resolve this mixed fraction by adding the leftover to wholeInteger
	int wholeInteger = newNumerator / gcd;
	int remainder = newNumerator % gcd;

	bool isNegative = false;
	//if either c1 or c2 is negative then there is a chance the sum will be negative
	if (c1 < 0 || c2 < 0)
	{
		if (c1 - c2 < 0 || (c1 + c2 == 0 && c2 < 0) || (c1 + c2 == 0 && c1 < 0))
		{
			isNegative = true;
		}
	}

	//add the characteristics of 1 and 2 and the value of the wholeInteger to get the value of the number to the left of the decimal
	int leftOfDecimal = c1 + c2 + wholeInteger;
	int rightOfDecimal = remainder;

	//creating an array to store the values of the converted numbers
	char* arrayOfConvertedNumbers = new char[len];

	//setting up to convert the integer into the result array of characters
	int leftOfDecimalCounter = 0;
	int testNumber = leftOfDecimal;

	//calling a function that will convert the obtained characteristic integer into an array of character representation of the number
	convertIntegerToCharacterArray(arrayOfConvertedNumbers, leftOfDecimalCounter, testNumber);

	//after obtaining the result from the conversion, the numbers will be reversed and need to be swapped.
	reverseArrayIntoResultArray(arrayOfConvertedNumbers, result, leftOfDecimalCounter, false, isNegative, isTerminating, len);

	//delete the array used to assist with the setting of result
	delete[] arrayOfConvertedNumbers;

	//use the same tempArray pointer to create a new array 
	arrayOfConvertedNumbers = new char[len];

	//take care of the right of the decimal now
	testNumber = rightOfDecimal;
	int rightOfDecimalCounter = 0;

	//calling a function that will convert the obtained characteristic integer into an array of character representation of the number
	convertIntegerToCharacterArray(arrayOfConvertedNumbers, rightOfDecimalCounter, testNumber);

	//because the function takes in an int& index, the rightOfDecimalCounter is still set and must be tallied up into the total number of characters
	leftOfDecimalCounter += rightOfDecimalCounter;

	//place the reversed contents of the array into result
	reverseArrayIntoResultArray(arrayOfConvertedNumbers, result, leftOfDecimalCounter, true, isNegative, isTerminating, len);
	
	//endOfNumbersInArray will be changed to true once the last digit of a non terminating number is found
	bool endOfNumbersInArray = false;

	//this integer will be set to the last digit of the non terminating number once it is found so it can be copied to the rest of the array values that are still have 'x's
	int lastNumberInArray = 0;

	//if the number does not terminate then proceed to fill the rest of the array with the last decimal place value that was found in the result array
	if (!isTerminating)
	{
		for (int i = 0; i < len - 1; i++)
		{
			//while there are still only numbers found in the array...
			if (!endOfNumbersInArray)
			{
				if (result[i] == 'x')
				{
					//if the end of the numbers in the decimal were found then signal the end was found and decrement i so the previous array value can be set to to lastNumberInArray
					endOfNumbersInArray = true;
					i--;
					lastNumberInArray = result[i];
				}
			}
			//once the end of the numbers was found in the array then set the remaining values in the array to the last decimal place value found
			else
			{
				result[i] = lastNumberInArray;
			}

		}
	}

	//delete the array used to assist with the setting of result
	delete[] arrayOfConvertedNumbers;

	return retval;
}

bool isPowerOf10(int d)
{
	//boolean value that will only be toggled true if the denominator of a given fraction is a power of 10
	bool retval = false;

	//note that 0 is not a power of 10 but it is important to keep this as 0 because it needs to be handled and set properly in add()
	if (d == 0)
	{
		retval = true;
	}

	//if the denominator is any integer greater than 0 then determine if it is a power of 10
		//this block of code wont know how to handle negative numbers which shouldnt occur in this function but may still need to be handled
	else
	{
		//this integer will be incremented to its following value in power of 10. ie 1(10^0), 10(10^1), 100(10^2)...
		int powerOfTen = 1;

		//continue until a powerOfTen is found that may be equal the denominator
		while (d > powerOfTen)
		{
			powerOfTen *= 10;
		}
		
		//after the powerOfTen is set to a value that could show whether or not the denominator is a power of 10, divide it by 'powerOfTen'
		//and if it the result is equivalent to 1, then the denominator is a power of ten
		if (d / powerOfTen == 1)
		{
			retval = true;
		}
	}

	return retval;
}

void convertDenominatorToPowerOf10(int& c, int& n, int& d, int lengthOfDecimalPrecision)
{
	//performing long division to retrieve the quotient (will serve as the decimal) of a fraction 

	//used to keep track of the running decimal being found through long division
	int quotient = 0;

	//will be used to make sure that the algorithm does not attempt to find decimal place values greater than the size of the array because it would be pointless
	int numIterations = 0;

	//will be used to build an integer starting from the 1's place to a maximum of one power of 10 lower than the INT_MAX
	int integerBuilder = 1;

	//left side of division line when doing long division
	int divisor = d;
	//right side of division line when doing long division
	int dividend = n;

	//array that will store all of the numbers being found for the quotient so the array can be traversed and begin building the quotient from front to back
	int* arrayOfQuotientNumbers = new int[lengthOfDecimalPrecision];

	//this will give decimal place values up to the size of the array for the answer, not all values found in the quotient and remainder may be shown due to lack of array space
	while (numIterations < lengthOfDecimalPrecision)
	{
		//cannot divide by zero. no need for exception because it will be caught in add()
		if (dividend == 0)
		{
			break;
		}

		//if the divisor cannot go into the divident, then add a zero to the dividend so that for the next iteration, the divisor may be able to go into the dividend
		else if (divisor > dividend)
		{
			dividend *= 10;
		}

		//if the divisor can go into the dividend
		else if (divisor <= dividend)
		{
			//make sure the place holder indicator doesn't overflow its integer
			if (integerBuilder > INT_MAX / 10)
			{
				break;
			}
			//obtain the current quotient digit
			quotient = dividend / divisor;

			//then set the quotient digit into the arrayOfQuotientNumbers
			arrayOfQuotientNumbers[numIterations] = quotient;

			//change the dividend accoring to the rules of long division
			dividend = dividend - ((dividend / divisor) * divisor);

			//increase the place value indicator by 10
			integerBuilder *= 10;

			//increase numIterations because another decimal point was found for the quotient
			numIterations++;
		}
	}

	//new base ten denominator has been found and needs to be set now because integerBuilder will be changed
	d = integerBuilder;

	//resetting the quotient so it can be used to build itself up from the arrayOfQuotientNumbers
	quotient = 0;

	//remove one power of 10 from IntegerBuilder to account for it being incremented once extra before breaking from the loop
	integerBuilder /= 10;

	//convert numbers in arrayOfQuotientNumbers to a built up integer variable quotient and set it to n
	for(int i = 0; i < numIterations; i++)
	{
		//build up quotient by adding itself and the current digit multiplied by the place value
		quotient += arrayOfQuotientNumbers[i] * integerBuilder;
		//decrease the integerBuilder so that the next quotient digit will be set to one place value lower than this current digit
		integerBuilder /= 10;
	}

	//set the numerator to the found quotient
	n = quotient;
	
	delete[] arrayOfQuotientNumbers;
}

int determineDifferenceInPower(int & n1, int & d1, int & n2, int & d2)
{
	int differenceInPower = 0;

	if (d1 < d2)
	{
		//set differenceInPower
		differenceInPower = d2 / d1;
		//set the new numerator of n1 to accomodate the differenceInPower
		n1 = n1 * differenceInPower;
		//set d1 to be the same as d2
		d1 = d2;
	}

	else if (d1 > d2)
	{
		//set the differenceInPower
		differenceInPower = d1 / d2;
		//set the new numerator of n2 to accomodate the differenceInPower
		n2 = n2 * differenceInPower;
		//set d2 to be the same as d1
		d2 = d1;
	}

	//if the denominators are the same, there is no need to manipulate the numerators
	else if (d1 == d2)
	{
		//set the differenceInPower
		differenceInPower = 1;
	}

	return differenceInPower;
}

void convertIntegerToCharacterArray(char arrayOfConvertedNumbers[], int &index, int testNumber)
{
	//if the number is negative, then set it to be the absolute value of itself
	if (testNumber < 0)
	{
		testNumber = testNumber * -1;
	}

	//loop through until the testNumber cannot be used anymore
	while (testNumber > 0)
	{
		//get the last digit from testNumber and convert it into an ascii value and place it into tempArray
		arrayOfConvertedNumbers[index] = testNumber % 10 + ASCII_ZERO;
		//remove the last digit from the number by dividing testNumber by 10
		testNumber = testNumber / 10;
		index++;
	}
}

//takes in an array with backwards elements and an array to place the elements in the right way, 
//a counter to iterate through the arrays, and a boolean value to increment past a decimal point character
void reverseArrayIntoResultArray(char arrayToReverse[], char resultArray[], int counter, bool isMantisa, bool isNegative, bool isTerminating, int len)
{
	//if working with the mantisa, use this technique
	if (isMantisa)
	{
		int reverseIndex = 0;

		//loop through until the decimal point is found and keep track of the iterations with a counter
		while (resultArray[reverseIndex] != ASCII_DECIMAL)
		{
			reverseIndex++;
		}

		//increment the counter to move past the decimal point
		reverseIndex += 1;

		//set a difference int to signal the amount of characters needing to be swapped
		int difference = counter - reverseIndex;

		//set difference to be the absolute value of itself
		if (difference < 0)
		{
			difference = difference * -1;
			difference -= 1;
		}
	
		while (difference >= 0)
		{
			//make sure only enough digits are reversed into the result that the array size allows for
			if (reverseIndex >= len - 1)
			{
				break;
			}
			//place the elements into result Array from arrayToReverse from back to front
			resultArray[reverseIndex] = arrayToReverse[difference];
			difference--;
			reverseIndex++;
		}

		

		//add a null terminator to the next element of resultArray if it is terminating. If it is not, leave it open ended so extra digits can be written to the array
		if (isTerminating)
		{
			resultArray[reverseIndex] = '\0';
		}
	}

	//if it is the characteristic, use this technique
	else if (!isMantisa)
	{
		int reverseIndex = 0;

		//if the result is negative, then add a negative sign to the beginning of the number
		if (isNegative)
		{
			resultArray[0] = '-';
			reverseIndex += 1;
		}

		//loop through until the counter reaches the end of arrayToReverse
		while (counter > 0)
		{
			//place the elements into resultArray from arrayToReverse from back to front
			resultArray[reverseIndex] = arrayToReverse[counter - 1];
			counter--;
			reverseIndex++;
		}

		//add a decimal point to the next element of resultArray
		resultArray[reverseIndex] = '.';
	}

}
