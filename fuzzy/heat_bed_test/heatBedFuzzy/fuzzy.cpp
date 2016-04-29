/*
	Author Ahmed El Fakharany afakharany93@gmail.com

	This is a library used to calculate the change required for the output by measuring the feedback element using fuzzy logic control
	error input is percentized	en = ((e - emin)*100)/(emax - emin)
	it takes the number of sets as a constructor input then calculates the the fuzzy sets boundaries 	0.5 range of set = 100/(n+1)
	the sets are numbered from 0 to (n-1), 0 is the number of the most negative value while (n-1) is the set number of the most +ve value
*/

#include "fuzzy.h"


fuzzy::fuzzy(short int n, int imax, int imin, int omax, int omin)
{
	if(n%2 == 0)	//number of sets must be odd
	{
		_n_membr_sets = n+1;
	}
	else
	{
		_n_membr_sets = n;
	}

	set_point = 0;
		
	crisp_ip = 0;	//crisp input for fuzzy logic i.e. measured value  
	ip_max = imax;
	ip_min = imin;

	op_max = 0;		//output max value
	op_min = 0;		//output minimum value

	error = 0;
	error_1 = 0;	//old error
	error_max = ip_max - ip_min;
	error_min = ip_min - ip_max;
	error_p = 0;	//percentized error
	

	ch_error = 0;
	ch_error_max = error_max - error_min;
	ch_error_min = error_min - error_max;
	ch_error_p = 0;

	ch_op_max = omax - omin;
	ch_op_min = omin - omax;
	ch_op = 0;
	ch_op_p = 0;
}


short int fuzzy::percentizer (int val, int val_max, int val_min)
{
	short int res =  ((val -val_min)*100) / (val_max - val_min);
	return(res);
}

short int fuzzy::depercentizer (int val, int val_max, int val_min)
{
	return( ((val*(val_max - val_min))/100) + val_min );
}

short int fuzzy::error_calc(int val, int set_val)
{
	short int res;
	error = set_val - val;
	res = percentizer (error, error_max, error_min);
	return (res);
}

//honey here is a mistake, don't calculate the change of error by using normalized en and en-1, calculate it before normalization
short int fuzzy::ch_error_calc (int en, int *en_1)	//calculate percentized change of error
{
	short int res;
	ch_error = en - *en_1;
	*en_1 = en;	//make current error old error for next round
	res = percentizer (ch_error, ch_error_max, ch_error_min);
	return (res);
}

struct membr_set_val fuzzy::membership_determiner(short int n, short int val)
{
	struct membr_set_val u = {UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER};	//used to hold membership values, initialized with undefined value

	short int count;
	short int b_range = 100/(n+1);

	short int a;
	short int b;
	short int c;

	short int u_val;

	for (count = 0; count < n; count ++)	//loop all fuzzy sets, number of sets is determined by the user
	{
		b = b_range + (count * b_range);	//calculate center of every set, each set at a time
		if (count == 0 && val <= b)						//if its the most negative set & if its the most negative set and the value is less that the center of it, then its a full member of the most negative set and get out of the loop
		{	
			u.set_1 = count;
			u.deg_truth_1 = 100;
			u.set_2 = UNDEFINED_SET_NUMBER;
			u.deg_truth_2 = UNDEFINED_SET_NUMBER;
			break;
		}
		else if (count == (n-1) && val >= b)	//if it is the most +ve set & if it is the most +ve set and the value is more that the center, then its a full member of that set and get out of the loop 
		{
			u.set_1 = count;
			u.deg_truth_1 = 100;
			u.set_2 = UNDEFINED_SET_NUMBER;
			u.deg_truth_2 = UNDEFINED_SET_NUMBER;
			break;
		}
		else	//if value belongs to the rest of the sets
		{
			a = b - b_range;	//get both a and c
			c = b + b_range;
			if(val <= b && val > a) //if val is between both a and b
			{
				u_val = (val - a)*100/(b - a);

				if(u_val > 0 && u_val <= 100)	//if the degree of truth is within range
				{
					if(u.set_1 == UNDEFINED_SET_NUMBER)	//if set_1 is not taken, take it
					{
						u.set_1 = count;
						u.deg_truth_1 = u_val;
					}
					else if (u.set_2 == UNDEFINED_SET_NUMBER) //if set_1 is taken and if set_2 is not taken, take it
					{
						u.set_2 = count;
						u.deg_truth_2 = u_val;
					}
				}
			}
			else if(val > b && val < c) //if val is between both c and b
			{
				u_val = (c - val)*100 / (c - b);	//like before but with c and b
				if(u_val > 0 && u_val <= 100)
				{
					if(u.set_1 == UNDEFINED_SET_NUMBER)
					{
						u.set_1 = count;
						u.deg_truth_1 = u_val;
					}
					else if (u.set_2 == UNDEFINED_SET_NUMBER)
					{
						u.set_2 = count;
						u.deg_truth_2 = u_val;
					}
				}
			}
		}
		if(u.set_1 != UNDEFINED_SET_NUMBER && u.set_2 != UNDEFINED_SET_NUMBER)	//variable will have values in at most 2 sets, no need to continue looping if they are obtained
		{
			break;
		}
	}
	return (u);
}

struct op_membr_val fuzzy::ch_op_determiner(short int n, struct membr_set_val ip_mem_1, struct membr_set_val ip_mem_2)
{
	//short int dia_n = (2*n)-1;	//number of diagonals in the table, which is also the number of ch op sets, always an odd number
	short int dia = 0;	//used to hold the number of the current diagonal
	short int dia_val = 0;

	struct op_membr_val u = {UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER, UNDEFINED_SET_NUMBER};

	if((ip_mem_1.set_1 != UNDEFINED_SET_NUMBER) && (ip_mem_2.set_1 != UNDEFINED_SET_NUMBER))
	{
		dia = ip_mem_1.set_1 + ip_mem_2.set_1;
		if(ip_mem_1.deg_truth_1 < ip_mem_2.deg_truth_1)
		{
			dia_val = ip_mem_1.deg_truth_1;
		}
		else
		{
			dia_val = ip_mem_2.deg_truth_1;
		}

		u.set_1 = dia;
		u.deg_truth_1 = dia_val;
	}
	if((ip_mem_1.set_1 != UNDEFINED_SET_NUMBER) && (ip_mem_2.set_2 != UNDEFINED_SET_NUMBER))
	{
		dia = ip_mem_1.set_1 + ip_mem_2.set_2;
		if(ip_mem_1.deg_truth_1 < ip_mem_2.deg_truth_2)
		{
			dia_val = ip_mem_1.deg_truth_1;
		}
		else
		{
			dia_val = ip_mem_2.deg_truth_2;
		}

		if(dia == u.set_1)
		{
			if(dia_val > u.deg_truth_1)
			{
				u.deg_truth_1 = dia_val;
			}
		}
		else
		{
			u.set_2 = dia;
			u.deg_truth_2 = dia_val;
		}
	}
	if((ip_mem_1.set_2 != UNDEFINED_SET_NUMBER) && (ip_mem_2.set_1 != UNDEFINED_SET_NUMBER))
	{
		dia = ip_mem_1.set_2 + ip_mem_2.set_1;
		if(ip_mem_1.deg_truth_2 < ip_mem_2.deg_truth_1)
		{
			dia_val = ip_mem_1.deg_truth_2;
		}
		else
		{
			dia_val = ip_mem_2.deg_truth_1;
		}

		if(dia == u.set_1)
		{
			if(dia_val > u.deg_truth_1)
			{
				u.deg_truth_1 = dia_val;
			}
		}
		else if(dia == u.set_2)
		{
			if(dia_val > u.deg_truth_2)
			{
				u.deg_truth_2 = dia_val;
			}
		}
		else
		{
			u.set_3 = dia;
			u.deg_truth_3 = dia_val;
		}
	}
	if((ip_mem_1.set_2 != UNDEFINED_SET_NUMBER) && (ip_mem_2.set_2 != UNDEFINED_SET_NUMBER))
	{
		dia = ip_mem_1.set_2 + ip_mem_2.set_2;
		if(ip_mem_1.deg_truth_2 < ip_mem_2.deg_truth_2)
		{
			dia_val = ip_mem_1.deg_truth_2;
		}
		else
		{
			dia_val = ip_mem_2.deg_truth_2;
		}

		if(dia == u.set_1)
		{
			if(dia_val > u.deg_truth_1)
			{
				u.deg_truth_1 = dia_val;
			}
		}
		else if(dia == u.set_2)
		{
			if(dia_val > u.deg_truth_2)
			{
				u.deg_truth_2 = dia_val;
			}
		}
		else
		{
			u.set_3 = dia;
			u.deg_truth_3 = dia_val;
		}
	}

	return (u);
}

short int fuzzy::defuzzifier (short int n, struct op_membr_val u)
{
	short int dia_n = (2*n)-1;	//number of diagonals in the table, which is also the number of ch op sets, always an odd number
	
	short int b_range = 100/(dia_n+1);
	short int b;
	
	int den = 0;
	int nom = 0;

	short int res = 0;

	if(u.set_1 != UNDEFINED_SET_NUMBER)
	{
		b = b_range + (u.set_1 * b_range);
		nom = nom + (b * u.deg_truth_1);
		den = den + u.deg_truth_1;	
	}
	if(u.set_2 != UNDEFINED_SET_NUMBER)
	{
		b = b_range + (u.set_2 * b_range);
		nom = nom + (b * u.deg_truth_2);
		den = den + u.deg_truth_2;	
	}
	if(u.set_3 != UNDEFINED_SET_NUMBER)
	{
		b = b_range + (u.set_3 * b_range);
		nom = nom + (b * u.deg_truth_3);
		den = den + u.deg_truth_3;	
	}

	res = nom / den;
	return (res);
}

int fuzzy::fuzzy_controller(int input, int s_point)
{
	set_point = s_point;
	crisp_ip = input;

	error_p = error_calc(crisp_ip, set_point);
	ch_error_p = ch_error_calc(error, &error_1);

	err_set = membership_determiner(_n_membr_sets, error_p);
	ch_err_set = membership_determiner(_n_membr_sets, ch_error_p);

	ch_op_set = ch_op_determiner(_n_membr_sets, err_set, ch_err_set);

	ch_op_p = defuzzifier(_n_membr_sets, ch_op_set);

	ch_op = depercentizer(ch_op_p, ch_op_max, ch_op_min);	

	return (ch_op);
}