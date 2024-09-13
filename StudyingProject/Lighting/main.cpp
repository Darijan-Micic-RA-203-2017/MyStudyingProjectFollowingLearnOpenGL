#include "Colors/colors.h"
#include "Basic_Lighting/basic_lighting_diffuse.h"
#include "Basic_Lighting/basic_lighting_specular.h"
#include "Basic_Lighting/basic_lighting_exercise2.h"
#include "Basic_Lighting/basic_lighting_exercise3.h"
#include "Materials/materials.h"
#include "Materials/materials_exercise1.h"
#include "Lighting_maps/lighting_maps_diffuse.h"
#include "Lighting_maps/lighting_maps_specular.h"
#include "Lighting_maps/lighting_maps_exercise2.h"
#include "Lighting_maps/lighting_maps_exercise4.h"

int main()
{
	int returnValue = 0;

	int argument = 0;
	while (argument != -1)
	{
		std::cout << "Enter argument from set {1, ..., 11}: ";
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
			std::cout << "\"Lighting - Basic Lighting, exercise 3\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 6:
			returnValue = draw_materials();
			std::cout << "\"Lighting - Materials\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 7:
			returnValue = draw_materials_exercise1();
			std::cout << "\"Lighting - Materials, exercise 1\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 8:
			returnValue = draw_lighting_maps_diffuse();
			std::cout << "\"Lighting - Lighting maps, diffuse\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 9:
			returnValue = draw_lighting_maps_specular();
			std::cout << "\"Lighting - Lighting maps, specular\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 10:
			returnValue = draw_lighting_maps_exercise2();
			std::cout << "\"Lighting - Lighting maps, exercise 2\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 11:
			returnValue = draw_lighting_maps_exercise4();
			std::cout << "\"Lighting - Lighting maps, exercise 4\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		default:
			break;
		}
	}

	return 0;
}
