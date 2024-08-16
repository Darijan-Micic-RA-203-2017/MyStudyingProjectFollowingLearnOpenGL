#include "1_Getting_started/hello_window.h"
#include "1_Getting_started/hello_triangle.h"
#include "1_Getting_started/hello_triangle_indexed.h"
#include "1_Getting_started/hello_triangle_exercise1.h"
#include "1_Getting_started/hello_triangle_exercise2.h"
#include "1_Getting_started/hello_triangle_exercise3.h"
#include "1_Getting_started/shaders_uniform.h"
#include "1_Getting_started/shaders_interpolation.h"
#include "1_Getting_started/shaders_class.h"
#include "1_Getting_started/textures.h"
#include "1_Getting_started/textures_combined.h"
#include "1_Getting_started/transformations.h"
#include "1_Getting_started/coordinate_systems.h"
#include "1_Getting_started/coordinate_systems_depth.h"
#include "1_Getting_started/coordinate_systems_multiple.h"
#include "1_Getting_started/camera_circle.h"
#include "1_Getting_started/camera_keyboard_dt.h"
#include "1_Getting_started/camera_mouse_zoom.h"
#include "1_Getting_started/camera_class.h"

int main()
{
	int returnValue = 0;

	int argument = 0;
	while (argument != -1)
	{
		std::cout << "Enter argument from set {1, ..., 19}): ";
		std::cin >> argument;

		switch (argument)
		{
			case 1:
				returnValue = draw_hello_window();
				std::cout << "\"StudyingProject - Hello Window\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 2:
				returnValue = draw_hello_triangle();
				std::cout << "\"StudyingProject - Hello Triangle\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 3:
				returnValue = draw_hello_triangle_indexed();
				std::cout << "\"StudyingProject - Hello Triangle, indexed\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 4:
				returnValue = draw_hello_triangle_exercise1();
				std::cout << "\"StudyingProject - Hello Triangle, exercise 1\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 5:
				returnValue = draw_hello_triangle_exercise2();
				std::cout << "\"StudyingProject - Hello Triangle, exercise 2\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 6:
				returnValue = draw_hello_triangle_exercise3();
				std::cout << "\"StudyingProject - Hello Triangle, exercise 3\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 7:
				returnValue = draw_shaders_uniform();
				std::cout << "\"StudyingProject - Shaders, uniform\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 8:
				returnValue = draw_shaders_interpolation();
				std::cout << "\"StudyingProject - Shaders, interpolation\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 9:
				returnValue = draw_shaders_class();
				std::cout << "\"StudyingProject - Shaders, class\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 10:
				returnValue = draw_textures();
				std::cout << "\"StudyingProject - Textures\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 11:
				returnValue = draw_textures_combined();
				std::cout << "\"StudyingProject - Textures, combined\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 12:
				returnValue = draw_transformations();
				std::cout << "\"StudyingProject - Transformations\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 13:
				returnValue = draw_coordinate_systems();
				std::cout << "\"StudyingProject - Coordinate Systems\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 14:
				returnValue = draw_coordinate_systems_depth();
				std::cout << "\"StudyingProject - Coordinate Systems, depth\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 15:
				returnValue = draw_coordinate_systems_multiple();
				std::cout << "\"StudyingProject - Coordinate Systems, multiple cubes\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 16:
				returnValue = draw_camera_circle();
				std::cout << "\"StudyingProject - Camera, circle\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 17:
				returnValue = draw_camera_keyboard_dt();
				std::cout << "\"StudyingProject - Camera, moving smoothly (delta time) with keyboard\" exited with return value "
					<< returnValue << ".\n" << std::endl;
				break;
			case 18:
				returnValue = draw_camera_mouse_zoom();
				std::cout << "\"StudyingProject - Camera, looking around with mouse\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			case 19:
				returnValue = draw_camera_class();
				std::cout << "\"StudyingProject - Camera, class\" exited with return value " 
					<< returnValue << ".\n" << std::endl;
				break;
			default:
				break;
		}
	}

	return 0;
}
