#include <iostream>
#include "fuzzy.h"

int main()
{
	fuzzy magic(5);
	short int en;
	struct membr_set_val x;

	magic.ip_max = 300;
	magic.ip_min = 0;
	magic.crisp_ip=150;

	magic.set_point = 200;

	en = magic.error_calc(magic.crisp_ip);
	std::cout << "error: " << magic.error << std::endl;
	std::cout << "error max: " << magic.error_max << std::endl;
	std::cout << "error min:  "<< magic.error_min << std::endl;
	std::cout << "error percentized: " << en << std::endl;

	x = magic.membership_determiner(5, en);
	std::cout << "1st set: " << x.set_1 << std::endl;
	std::cout << "1st set u: " << x.deg_truth_1 << std::endl;
	std::cout << "2nd set: " << x.set_2 << std::endl;
	std::cout << "2nd set u: " << x.deg_truth_2 << std::endl;



	return 0;
}