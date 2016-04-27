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
	short int set_1;
	short int deg_truth_1;

	short int set_2;
	short int deg_truth_2;
};


class fuzzy
{
	private:
		short int _n_membr_sets;
	public:
		int set_point ;
		
		int crisp_ip ;	//crisp input for fuzzy logic i.e. measured value  
		int ip_max ;
		int ip_min ;

		int op_max ;		//output max value
		int op_min ;		//output minimum value

		int error ;
		int error_1 ;	//old error
		int error_max ;
		int error_min ;
		short int error_p ;	//percentized error
		

		int ch_error ;
		int ch_error_max ;
		int ch_error_min ;
		short int ch_error_p;
		
		fuzzy(short int n, int max, int min);

		short int percentizer	 (int val, int val_max, int val_min);
		short int depercentizer  (int val, int val_max, int val_min);
		
		/* calculates error percentized and saves value in error_p*/
		short int error_calc(int val, int set_val);
		short int ch_error_calc (int en, int *en_1);	//calculate percentized change of error
		struct membr_set_val membership_determiner(short int n, short int val);	//used to determine membership values of a variable
};

#endif