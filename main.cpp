#include <iostream>
#include <string>

#include "get_opt.h"

using namespace std;

const string VERSION = "0.1";

int main(int argc, char* argv[])
{
	int c;

	//1. Init flags
	get_opt_set_flags("dva", NULL, "00+");
	
	//2. Loop through functions
	while ((c = get_opt(argc, argv)) != -1)
	{
		switch(c)
		{
			case 'd':
				cout << "Debug enabled" << endl;
				break;
			case 'v':
				cout << "Version: " << VERSION << endl;
				break;
			case 'a':
				string a{getoptarg};
				cout << "A parameter is " << a << endl;
				break;
		}
	}
	//3. Free resources
	get_opt_free();


	//End program
	return 0;
}
