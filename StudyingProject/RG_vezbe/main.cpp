#include "Vezbe_03/Zadatak_01/vezbe_03_zadatak_01.h"

int main()
{
	int returnValue = 0;

	int argument = 0;
	while (argument != -1)
	{
		std::cout << "Enter argument from set {1}: ";
		std::cin >> argument;

		switch (argument)
		{
		case 1:
			returnValue = draw_vezbe_03_zadatak_01();
			std::cout << "\"Vebe 3 - zadatak 1\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		default:
			break;
		}
	}

	return 0;
}
