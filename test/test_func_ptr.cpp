#include "func_ptr.hpp"
#include <stdlib.h>

int main(void)
{
	using namespace edward;
	
	func_ptr<int> ptr_to_int(std::make_shared<int>(10));
	
	return 0;
}