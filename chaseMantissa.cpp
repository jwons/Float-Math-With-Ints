#include <iostream>
#include <stdlib.h>
#include <"chaseMantissa.h">

using namespace std;

/*
	this function takes in an array of characters that represent the number
	the user entered, a reference to the numerator, and a reference to the
	denominator. 

	It first will check the array to make sure that it is a valid number and
	from there it shall pull out the numerator from the number. It will also 
	create the denominator and change the references. 

	The function will return true if the character array passed contained
	a valid number for finding the mantissa, or it will return false if no
	valid mantissa could be found
*/

bool mantissa(char numString[], int& numerator, int& denominator) {
	
	//assume that it is a valid mantissa
	bool isValid = false;

	//these will be used later in the code for readability
	const char ZERO = '0';
	const char NINE = '9';
	const char DECIMAL = '.';

	int currentPosition = 0;
	

	//go through the array and find the decimal's location
	//also go through and validate that this is a valid mantissa
	while(numString[currentPosition] != '\0') {

		//if the current cell in the array is a decimal point check whether or not
		//the following character is a numarical value. If the second value is not
		//a numerical value then break out of the for loop and end the function
		if (numString[currentPosition] == DECIMAL) {
			if (numString[currentPosition + 1] >= ZERO && numString[currentPosition + 1] <= NINE) {
				isValid = true;

				//increment the decimal position. the point that this increment is called
				//will be the last time and we will have found the position of the decimal
				currentPosition++;
				break;
			}
		}

		//increment the position of the decimal until we find it
		currentPosition++;
	}









	//if the entered character array is a mantissa
	//then go through the array and figure out what the numerator
	//and denominator are for the mantissa
	if (isValid == true) {
		
		//this value is used to keep track of the trailing zeros
		int trailingZeros = 0;

		//this keeps track of the length of the mantissa
		int lengthOfMantissa = 0;

		/*	go through the char array and find all of
			the digits of the numerator. Also keep track
			of the trailing zeros and the length of the
			mantissa. If it is the null terminator then
			break out of the loop	*/
		while (numString[currentPosition] != '\0') {
			
	

			//if the current char is a zero, keep track of it
			//because it might be a trailing zero. If it turns
			//out not to be, then we will reset it later
			if (numString[currentPosition] == ZERO) {
				trailingZeros++;
			}

			//if it is a number somewhere from 1-9, increment 
			//then change the trailing zeros back to zero
			else {
				trailingZeros = 0;
				
			}

			//increase the lenght of the current mantissa
			lengthOfMantissa++;

			//now that we have increased the counters, add the
			//current char to the numerator
			int tempDigit = numString[currentPosition] - '0';


			/*	now that we have converted the current character into
				an int, add it to the numerator.

				To add a new digit onto the numerator, take what ever
				is in the numerator and multiply it by 10. Then add
				on the new digit.
			*/
			numerator = (numerator * 10) + tempDigit;

			//increase the current position
			currentPosition++;
		}

		/*	now that we have the number of trailing zeros
			we have to fix the size of the mantissa to 
			adjust for the trailing zeros
		*/
		lengthOfMantissa = lengthOfMantissa - trailingZeros;
		
		//now we have to pop off the trailing zeros from the numerator
		//divide by 10 for each trailing zero
		for (int i = 0; i < trailingZeros; i++) {
			numerator = numerator / 10;
		}

		/*	the last thing we have to do is figure out the denominator.
			to do this we have to multiply the denominator (which will
			be set to 10 to begin with) by 10^length of mantissa		
		*/

		denominator = 10;
		for (int i = 1; i < lengthOfMantissa; i++) {
			denominator = denominator * 10;
		}	
	}

	//return whether or not the entered value is a valid mantissa
	return isValid;
}