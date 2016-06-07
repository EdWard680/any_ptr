#include "func_ptr.hpp"
#include <stdlib.h>
#include <iostream>

namespace std
{
	template<class T, class... Args>
	unique_ptr<T> make_unique(Args... args)
	{
		return unique_ptr<T>(new T(args...));
	}
}

int main(void)
{
	using namespace edward;
	using std::cout;
	using std::endl;
	
	func_ptr<int> shared(std::make_shared<int>(10));
	func_ptr<int> unique(std::make_unique<int>(8));
	func_ptr<int> raw(shared.get());
	func_ptr<int> internal(3);
	
	cout << "shared = " << *shared << endl;
	cout << "unique = " << *unique << endl;
	cout << "raw = " << *raw << endl;
	cout << "Changing shared..." << endl;
	*shared = 11;
	cout << "raw = " << *raw << endl;
	cout << "internal = " << *internal << endl;
	
	return 0;
}