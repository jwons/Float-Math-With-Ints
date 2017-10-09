#pragma once
using namespace std;
//decleration of functions 
bool subtract(int c1, int n1, int d1, int c2, int n2, int d2, char* result, int len);
void mantissasubtraction(int &n1, int &n2, int &d1, int &d2, int &leftover, int &nfinal, int &dfinal, int &cfinal, bool &ispos);
int powerof10(int num);
bool storevalues(int cfinal, int nfinal, int dfinal, char* result, int len, bool ispos, int inttochar);
int intatpos(int num, int pos);
int tentothepowerof(int exponent);






bool subtract(int c1, int n1, int d1, int c2, int n2, int d2, char* result, int len)
{
	for (int i = 0; i < len; i++)
	{
		result[i] = '0';
	}


	//stores the leftover from the mantissa subtraction
	int leftover = 0;
	//converts int to char
	const int inttochar = 48;
	int nfinal = 0;
	int dfinal = 0;
	bool ispos = true;
	int cfinal = c1 - c2;


	//finds a common denominator 
	dfinal = d1*d2;
	n1 = n1*d2;
	n2 = n2*d1;
	nfinal = n1 - n2;


	//handels the diffrent casses 
	mantissasubtraction(n1, n2, d1, d2, leftover, nfinal, dfinal, cfinal, ispos);


	//now all parts are known 
	//stores the result in result
	bool returnvalue = storevalues(cfinal, nfinal, dfinal, result, len, ispos, inttochar);
	return returnvalue;
}




//determines the final mantissa
void mantissasubtraction(int &n1, int &n2, int &d1, int &d2, int &leftover, int &nfinal, int &dfinal, int &cfinal, bool &ispos)
{
	// diffrent combining cases
	//represents 1.0 in a way to interact with the mantissa eg mantissa = 2/100 -> 2  one = 1 -> 100
	int one = 1 * dfinal;


	// positive chariteristic negitive mantissa
	if (nfinal < 0 && cfinal>0)
	{
		leftover = 1;
		nfinal = one + nfinal;
		cfinal = cfinal - leftover;
		ispos = true;
	}


	// negitive chariteristic positive mantissa
	else if (nfinal > 0 && cfinal < 0)
	{
		leftover = -1;
		nfinal = one - nfinal;
		cfinal = cfinal - leftover;
		cfinal = cfinal * -1;
		ispos = false;
	}


	else if (cfinal == 0 && nfinal < 0)
	{
		nfinal = nfinal * -1;
		ispos = false;
	}


	//both negative
	else if (nfinal<0)
	{
		nfinal = nfinal *-1;
		cfinal = cfinal *-1;
		ispos = false;
	}


	else if (nfinal == 0 && cfinal < 0)
	{
		cfinal = cfinal *-1;
		ispos = false;
	}


	//only other option is that they are positive 
	return;
}




//finds to what power of 10 num is
int powerof10(int num)
{
	int power = 0;
	while (num != 0)
	{
		num = num / 10;
		power++;
	}
	return power;
}




bool storevalues(int cfinal, int nfinal, int dfinal, char* result, int len, bool ispos, int inttochar)
{
	bool returnvalue = true;


	//finding decimal point location
	int decimalpointlocation = 0;


	//used to find how long c is
	int clength = powerof10(cfinal);
	int counter = 0;


	//do we have room?
	if (clength > len)
	{
		returnvalue = false;
	}


	else
	{
		if (ispos == false)
		{
			result[0] = '-';


			//due to '-'
			decimalpointlocation++;
			counter = 1;
		}


		//places the chariteristic into result
		for (int i = 1; i <= clength; i++)
		{
			result[clength - i + counter] = (char)intatpos(cfinal, i) + inttochar;
		}
		decimalpointlocation = decimalpointlocation + clength;


		if (decimalpointlocation < len - 1)
		{
			result[decimalpointlocation] = '.';


			//now dealing with the decimal part
			int dlength = powerof10(dfinal);
			int nlength = powerof10(nfinal);


			//this allows us to find where n should be inserted
			//where the decimal starts eg .000[2]34
			int startlocation = decimalpointlocation + (dlength - nlength);


			//eg .00023[4]
			int end = startlocation + nlength;
			if (end >= len - 1)
				end = len - 2;
			counter = 0;


			//places the mantissa into result
			for (int i = nlength; i >= (end - startlocation); i--)
			{
				if (startlocation + counter > end)
				{
					break;
				}


				else
				{
					result[startlocation + counter] = (char)intatpos(nfinal, i) + inttochar;
					counter++;
				}
			}
		}
	}
	result[len - 1] = '\0';
	return returnvalue;
}




//finds the int at a location eg pos =3 num =1234 will return 2
int intatpos(int num, int pos)
{
	int returnval;


	if (num / 10 == 0)
	{
		returnval = num;
	}


	else
	{
		returnval = (num%tentothepowerof(pos)) / tentothepowerof(pos - 1);
	}
	return returnval;
}




//finds 10^exponent
int tentothepowerof(int exponent)
{
	int returnvalue = 1;


	for (int i = 0; i < exponent; i++)
	{
		returnvalue = returnvalue * 10;
	}


	return returnvalue;
}