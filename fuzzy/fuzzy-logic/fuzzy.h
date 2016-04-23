/*
	Author Ahmed El Fakharany afakharany93@gmail.com

	This is a library used to calculate the change required for the output by measuring the feedback element using fuzzy logic control
	error input is percentized	en = ((e - emin)*100)/(emax - emin)
	it takes the number of sets as a constructor input then calculates the the fuzzy sets boundaries 	0.5 range of set = 100/(n+1)
	the sets are numbered from 0 to (n-1), 0 is the number of the most negative value while (n-1) is the set number of the most +ve value
*/

#ifndef __FUZ_ZY__
#define __FUZ_ZY__

#define UNDEFINED_SET_NUMBER	150

struct membr_set_val
{
	unsigned char set_1;
	unsigned char deg_truth_1;

	unsigned char set_2;
	unsigned char deg_truth_2;
};


class fuzzy
{
	private:
		unsigned char _n_membr_sets;
	public:
		int set_point = 0;
		
		int crisp_ip = 0;	//crisp input for fuzzy logic i.e. measured value  

		int op_max = 0;		//output max value
		int op_min = 0;		//output minimum value

		int error = 0;
		int error_max = 0;
		int error_min = 0;
		unsigned char error_p = 0;	//percentized error
		unsigned char error_p_1 = 0;	//old percentized error

		unsigned char ch_error_p = 0;

		fuzzy(unsigned char n);

		unsigned char percentizer	 (int val, int val_max, int val_min);
		unsigned char depercentizer  (int val, int val_max, int val_min);
		
		/* calculates error percentized and saves value in error_p*/
		unsigned char error_calc(int val);
		unsigned char ch_error_calc (unsigned char* en, unsigned char* en_1);	//calculate percentized change of error
		struct membr_set_val membership_determiner(unsigned char n, unsigned char val);	//used to determine membership values of a variable
};

#endif