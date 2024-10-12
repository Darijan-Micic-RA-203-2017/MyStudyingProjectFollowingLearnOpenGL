#include "Depth_testing/depth_testing.h"

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
			returnValue = draw_depth_testing();
			std::cout << "\"Advanced OpenGL - Depth testing\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		default:
			break;
		}
	}

	return 0;
}
