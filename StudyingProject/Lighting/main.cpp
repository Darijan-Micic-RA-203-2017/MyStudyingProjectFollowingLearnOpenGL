#include "Colors/colors.h"
#include "Basic_Lighting/basic_lighting_diffuse.h"
#include "Basic_Lighting/basic_lighting_specular.h"
#include "Basic_Lighting/basic_lighting_exercise2.h"
#include "Basic_Lighting/basic_lighting_exercise3.h"

int main()
{
	int returnValue = 0;

	int argument = 0;
	while (argument != -1)
	{
		std::cout << "Enter argument from set {1, ..., 5}: ";
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
		case 3:
			returnValue = draw_basic_lighting_specular();
			std::cout << "\"Lighting - Basic Lighting, specular\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 4:
			returnValue = draw_basic_lighting_exercise2();
			std::cout << "\"Lighting - Basic Lighting, exercise 2\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 5:
			returnValue = draw_basic_lighting_exercise3();
			std::cout << "\"Lighting - Basic Lighting, exercise 2\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		default:
			break;
		}
	}

	return 0;
}
