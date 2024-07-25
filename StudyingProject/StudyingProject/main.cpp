#include "1_Getting_started/hello_window.h"
#include "1_Getting_started/hello_triangle.h"

int main()
{
	int argument = 0;
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
		default:
			break;
	}

	return 0;
}
