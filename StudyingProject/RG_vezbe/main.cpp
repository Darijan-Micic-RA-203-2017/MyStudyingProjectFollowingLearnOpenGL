#include "Vezbe_03/Zadatak_01/vezbe_03_zadatak_01.h"
#include "Vezbe_03/Zadatak_02/vezbe_03_zadatak_02.h"
#include "Vezbe_03/Zadatak_03/vezbe_03_zadatak_03.h"
#include "Vezbe_03/Zadatak_04/vezbe_03_zadatak_04.h"
#include "Vezbe_03/Zadatak_05/vezbe_03_zadatak_05.h"
#include "Vezbe_03/Zadatak_06/vezbe_03_zadatak_06.h"
#include "Vezbe_03/Zadatak_07/vezbe_03_zadatak_07.h"
#include "Vezbe_03/Zadatak_08/vezbe_03_zadatak_08.h"

int main()
{
	int returnValue = 0;

	int argument = 0;
	while (argument != -1)
	{
		std::cout << "Enter argument from set {1, ..., 8}: ";
		std::cin >> argument;

		switch (argument)
		{
		case 1:
			returnValue = draw_vezbe_03_zadatak_01();
			std::cout << "\"Vežbe 3 - zadatak 1\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 2:
			returnValue = draw_vezbe_03_zadatak_02();
			std::cout << "\"Vežbe 3 - zadatak 2\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 3:
			returnValue = draw_vezbe_03_zadatak_03();
			std::cout << "\"Vežbe 3 - zadatak 3\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 4:
			returnValue = draw_vezbe_03_zadatak_04();
			std::cout << "\"Vežbe 3 - zadatak 4\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 5:
			returnValue = draw_vezbe_03_zadatak_05();
			std::cout << "\"Vežbe 3 - zadatak 5\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 6:
			returnValue = draw_vezbe_03_zadatak_06();
			std::cout << "\"Vežbe 3 - zadatak 6\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 7:
			returnValue = draw_vezbe_03_zadatak_07();
			std::cout << "\"Vežbe 3 - zadatak 7\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		case 8:
			returnValue = draw_vezbe_03_zadatak_08();
			std::cout << "\"Vežbe 3 - zadatak 8\" exited with return value " 
				<< returnValue << ".\n" << std::endl;
			break;
		default:
			break;
		}
	}

	return 0;
}
