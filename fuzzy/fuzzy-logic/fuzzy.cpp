/*
	Author Ahmed El Fakharany afakharany93@gmail.com

	This is a library used to calculate the change required for the output by measuring the feedback element using fuzzy logic control
	error input is percentized	en = ((e - emin)*100)/(emax - emin)
	it takes the number of sets as a constructor input then calculates the the fuzzy sets boundaries 	0.5 range of set = 100/(n+1)
	the sets are numbered from 0 to (n-1), 0 is the number of the most negative value while (n-1) is the set number of the most +ve value
*/

#include "fuzzy.h"

fuzzy::fuzzy(unsigned char n)
{
	if(n%2 == 0)	//number of sets must be odd
	{
		_n_membr_sets = n+1;
	}
	else
	{
		_n_membr_sets = n;
	}
}


unsigned char fuzzy::percentizer (int val, int val_max, int val_min)
{
	return(((val -val_min)*100) / (val_max - val_min));
}

unsigned char fuzzy::depercentizer (int val, int val_max, int val_min)
{
	return( ((val*(val_max - val_min))/100) + val_min );
}

unsigned char fuzzy::error_calc(int val)
{
	unsigned char res;
	error = set_point - crisp_ip;
	error_max = set_point - op_min;
	error_min = set_point - op_max;

	res = percentizer (error, error_max, error_min);
	return (res);
}

unsigned char fuzzy::ch_error_calc (unsigned char* en, unsigned char* en_1)	//calculate percentized change of error
{
	unsigned char res;
	res = *en - *en_1 ;
	*en_1 = *en;	//make current error old error for next round
	return (res);
}

struct membr_set_val fuzzy::membership_determiner(unsigned char n, unsigned char val)
{
	struct membr_set_val u = {UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER};	//used to hold membership values, initialized with undefined value

	unsigned char count;
	unsigned char b_range = 100/(n+1);

	unsigned char a;
	unsigned char b;
	unsigned char c;

	unsigned char u_val;

	for (count = 0; count < n; count ++)	//loop all fuzzy sets, number of sets is determined by the user
	{
		b = b_range + (count * b_range);	//calculate center of every set, each set at a time
		if (count == 0)						//if its the most negative set
		{
			if (val <= b)	//if its the most negative set and the value is less that the center of it, then its a full member of the most negative set and get out of the loop
			{
				u.set_1 = count;
				u.deg_truth_1 = 100;
				u.set_2 = UNDEFINED_SET_NUMBER;
				u.deg_truth_2 = UNDEFINED_SET_NUMBER;
				break;
			}
		}
		else if (count == (n-1))	//if it is the most +ve set
		{
			if (val >= b)		//if it is the most +ve set and the value is more that the center, then its a full member of that set and get out of the loop 
			{
				u.set_1 = count;
				u.deg_truth_1 = 100;
				u.set_2 = UNDEFINED_SET_NUMBER;
				u.deg_truth_2 = UNDEFINED_SET_NUMBER;
				break;
			}
		}
		else	//if value belongs to the rest of the sets
		{
			a = b - b_range;	//get both a and c
			c = b + b_range;
			if(val <= b && val > a) //if val is between both a and b
			{
				u_val = (val - a)/(b - a);
				if(u_val > 0 && u_val <= 100)	//if the degree of truth is within range
				{
					if(u.set_1 == UNDEFINED_SET_NUMBER)	//if set_1 is not taken, take it
					{
						u.set_1 = count;
						u.deg_truth_1 = u_val;
					}
					else if (set_2 == UNDEFINED_SET_NUMBER) //if set_1 is taken and if set_2 is not taken, take it
					{
						u.set_2 = count;
						u.deg_truth_2 = u_val;
					}
				}
			}
			else if(val > b && val < c) //if val is between both c and b
			{
				u_val = (c - val) / (c - b);	//like before but with c and b
				if(u_val > 0 && u_val <= 100)
				{
					if(u.set_1 == UNDEFINED_SET_NUMBER)
					{
						u.set_1 = count;
						u.deg_truth_1 = u_val;
					}
					else if (set_2 == UNDEFINED_SET_NUMBER)
					{
						u.set_2 = count;
						u.deg_truth_2 = u_val;
					}
				}
			}
		}
		if(u.set_2 != UNDEFINED_SET_NUMBER && u.set_2 != UNDEFINED_SET_NUMBER)	//variable will have values in at most 2 sets, no need to continue looping if they are obtained
		{
			break;
		}
	}
	return (u);
}