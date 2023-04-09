#include <stdexcept>
#include <iostream>
#include <locale>

#include "processHacker.h"
#include "myjson.h"
#include "processHackerCommandLine.h"


int main()
{
	std::locale::global(std::locale(".UTF-8"));

	try {
		ProcessHackerCommandLine phCmd{};
		while (1)
			phCmd.getCommand();
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << "\n";
		std::cout << "error code " << GetLastError();
		return -1;
	}

	return 0;
}