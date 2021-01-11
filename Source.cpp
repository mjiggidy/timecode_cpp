#include <iostream>
#include "timecode.h"

int main()
{

	try
	{
		//std::cout << "Made timecode " << Timecode("04:03:04:02") << std::endl;
		std::cout << Timecode("04:03:02:23") + Timecode(1) << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << "Nope: " << e.what() << std::endl;
	}

	return 0;
}