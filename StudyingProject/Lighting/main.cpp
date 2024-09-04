#include "Colors/colors.h"
#include "Basic_Lighting/basic_lighting_diffuse.h"

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
			returnValue = draw_colors();
			std::cout << "\"Lighting - Colors\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 2:
			returnValue = draw_basic_lighting_diffuse();
			std::cout << "\"Lighting - Basic Lighting, diffuse\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		default:
			break;
		}
	}

	return 0;
}
