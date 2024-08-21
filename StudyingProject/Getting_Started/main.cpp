#include "Hello_Window/hello_window.h"
#include "Hello_Triangle/hello_triangle.h"
#include "Hello_Triangle/hello_triangle_indexed.h"
#include "Hello_Triangle/hello_triangle_exercise1.h"
#include "Hello_Triangle/hello_triangle_exercise2.h"
#include "Hello_Triangle/hello_triangle_exercise3.h"
#include "Shaders/shaders_uniform.h"
#include "Shaders/shaders_interpolation.h"
#include "Shaders/shaders_class.h"
#include "Textures/textures.h"
#include "Textures/textures_combined.h"
#include "Transformations/transformations.h"
#include "Coordinate_Systems/coordinate_systems.h"
#include "Coordinate_Systems/coordinate_systems_depth.h"
#include "Coordinate_Systems/coordinate_systems_multiple.h"
#include "Camera/camera_circle.h"
#include "Camera/camera_keyboard_dt.h"
#include "Camera/camera_mouse_zoom.h"
#include "Camera/camera_class.h"

int main()
{
	int returnValue = 0;

	int argument = 0;
	while (argument != -1)
	{
		std::cout << "Enter argument from set {1, ..., 19}: ";
		std::cin >> argument;

		switch (argument)
		{
		case 1:
			returnValue = draw_hello_window();
			std::cout << "\"Getting Started - Hello Window\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 2:
			returnValue = draw_hello_triangle();
			std::cout << "\"Getting Started - Hello Triangle\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 3:
			returnValue = draw_hello_triangle_indexed();
			std::cout << "\"Getting Started - Hello Triangle, indexed\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 4:
			returnValue = draw_hello_triangle_exercise1();
			std::cout << "\"Getting Started - Hello Triangle, exercise 1\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 5:
			returnValue = draw_hello_triangle_exercise2();
			std::cout << "\"Getting Started - Hello Triangle, exercise 2\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 6:
			returnValue = draw_hello_triangle_exercise3();
			std::cout << "\"Getting Started - Hello Triangle, exercise 3\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 7:
			returnValue = draw_shaders_uniform();
			std::cout << "\"Getting Started - Shaders, uniform\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 8:
			returnValue = draw_shaders_interpolation();
			std::cout << "\"Getting Started - Shaders, interpolation\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 9:
			returnValue = draw_shaders_class();
			std::cout << "\"Getting Started - Shaders, class\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 10:
			returnValue = draw_textures();
			std::cout << "\"Getting Started - Textures\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 11:
			returnValue = draw_textures_combined();
			std::cout << "\"Getting Started - Textures, combined\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 12:
			returnValue = draw_transformations();
			std::cout << "\"Getting Started - Transformations\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 13:
			returnValue = draw_coordinate_systems();
			std::cout << "\"Getting Started - Coordinate Systems\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 14:
			returnValue = draw_coordinate_systems_depth();
			std::cout << "\"Getting Started - Coordinate Systems, depth\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 15:
			returnValue = draw_coordinate_systems_multiple();
			std::cout << "\"Getting Started - Coordinate Systems, multiple cubes\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 16:
			returnValue = draw_camera_circle();
			std::cout << "\"Getting Started - Camera, circle\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 17:
			returnValue = draw_camera_keyboard_dt();
			std::cout << "\"Getting Started - Camera, moving smoothly (delta time) with keyboard\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 18:
			returnValue = draw_camera_mouse_zoom();
			std::cout << "\"Getting Started - Camera, looking around with mouse\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		case 19:
			returnValue = draw_camera_class();
			std::cout << "\"Getting Started - Camera, class\" exited with return value "
				<< returnValue << ".\n" << std::endl;
			break;
		default:
			break;
		}
	}

	return 0;
}
