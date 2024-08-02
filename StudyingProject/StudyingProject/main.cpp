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

int main()
{
	int argument = 0;
	while (argument != -1)
	{
		std::cout << "Enter argument: ";
		std::cin >> argument;

		switch (argument)
		{
			case 1:
				draw_hello_window();
				break;
			case 2:
				draw_hello_triangle();
				break;
			case 3:
				draw_hello_triangle_indexed();
				break;
			case 4:
				draw_hello_triangle_exercise1();
				break;
			case 5:
				draw_hello_triangle_exercise2();
				break;
			case 6:
				draw_hello_triangle_exercise3();
				break;
			case 7:
				draw_shaders_uniform();
				break;
			case 8:
				draw_shaders_interpolation();
				break;
			case 9:
				draw_shaders_class();
				break;
			case 10:
				draw_textures();
				break;
			default:
				break;
		}
	}

	return 0;
}
