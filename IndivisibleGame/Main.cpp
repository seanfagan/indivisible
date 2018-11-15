#include <iostream>
#include <sstream>
#include <vector>

int main()
{
	std::cout << "Hi world." << std::endl;

	std::stringstream ss("1a,2a, 3b, a1, aa ,");
	std::vector<std::string> result;

	while (ss.good())
	{
		std::string substr;
		std::getline(ss, substr, ',');
		result.push_back(substr);
	}

	std::cout << "Beginning" << std::endl;
	for (auto i : result)
	{
		std::cout << "|" << i << "|" << std::endl;
	}
	std::cout << "End" << std::endl;

	std::cin.get();
}
