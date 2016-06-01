// testing size of things

#include <stdlib.h>
#include <memory>
#include <functional>
#include <iostream>

int main()
{
	std::cout << "sizeof(char *) == " << sizeof(char*) << std::endl;
	std::cout << "sizeof(std::unique_ptr<char>) == " << sizeof(std::unique_ptr<char>) << std::endl;
	std::cout << "sizeof(std::shared_ptr<char>) == " << sizeof(std::shared_ptr<char>) << std::endl;
	std::cout << "sizeof(std::functional<char* ()> == " << sizeof(std::function<char* ()>) << std::endl;
	char *p = nullptr;
	auto fn = [p]() -> char* {return p;};
	std::cout << "sizeof(<lambda holding char*>) == " << sizeof(decltype(p)) << std::endl;
	return 0;
}