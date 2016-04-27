#include <iostream>
#include "fuzzy.h"

int main()
{
	fuzzy magic(9, 300, 0);
	struct membr_set_val x;

	
	magic.set_point = 200;
for(int i=0; i < 10; i++)
{
	magic.crisp_ip=27*i;
	std::cout << "input: " << magic.crisp_ip << std::endl;
	std::cout << "set point: " << magic.set_point << std::endl;

	std::cout << " " << std::endl;
	
	magic.error_p = magic.error_calc(magic.crisp_ip, magic.set_point);
	std::cout << "error: " << magic.error << std::endl;
	std::cout << "error max: " << magic.error_max << std::endl;
	std::cout << "error min:  "<< magic.error_min << std::endl;
	std::cout << "error percentized:  "<< magic.error_p << std::endl;
	std::cout << "error n-1: " << magic.error_1 << std::endl;
	
	std::cout << " " << std::endl;

	magic.ch_error_p = magic.ch_error_calc (magic.error, &magic.error_1);
	std::cout << "ch error: " << magic.ch_error << std::endl;
	std::cout << "ch error max: " << magic.ch_error_max << std::endl;
	std::cout << "ch error min:  "<< magic.ch_error_min << std::endl;
	std::cout << "ch error percentized:  "<< magic.ch_error_p << std::endl;
	
	std::cout << " " << std::endl;
	
	x = magic.membership_determiner(9, magic.error_p);
	std::cout << "1st set: " << x.set_1 << std::endl;
	std::cout << "1st set u: " << x.deg_truth_1 << std::endl;
	std::cout << "2nd set: " << x.set_2 << std::endl;
	std::cout << "2nd set u: " << x.deg_truth_2 << std::endl;

	std::cout << " " << std::endl;
	
	x = magic.membership_determiner(9, magic.ch_error_p);
	std::cout << "ch 1st set: " << x.set_1 << std::endl;
	std::cout << "ch 1st set u: " << x.deg_truth_1 << std::endl;
	std::cout << "ch 2nd set: " << x.set_2 << std::endl;
	std::cout << "ch 2nd set u: " << x.deg_truth_2 << std::endl;

	std::cout << "***********************************" << std::endl;
}


	return 0;
}