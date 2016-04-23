
#include "fuzzy.h"

fuzzy::fuzzy(unsigned char n)
{
	_n_membr_sets = n;
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

fuzzy::membership_determiner(unsigned char n, unsigned char val)
{
	unsigned char count;
	unsigned char b_range = 100/(n+1);

	unsigned char a;
	unsigned char b;
	unsigned char c;

	for (count = 0; count < n; count ++)
	{
		b = b_range + (count * b_range);
		if (count == 0)
		{
			if (val <= b)
			{

			}
		}
		else if (count == (n-1))
		{
			if (val >= b)
			{

			}
		}
		else
		{
			
		}
	}
}