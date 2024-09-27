#include "Model/model_loading.h"

int main()
{
	int returnValue = 0;

	int argument = 0;
	while (argument != -1)
	{
		std::cout << "Enter number from set {1}: ";
		std::cin >> argument;

		switch (argument)
		{
		case 1:
			returnValue = draw_model_loading();
			std::cout << "\"Model Loading - Backpack\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		default:
			break;
		}
	}

	return 0;
}
