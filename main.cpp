#include "app.hpp"

int main(int argc, char** argv)
{

	if (cli(argc, argv))
	{
		return 0;
	}

	init_data_model();

	return interactive();
}
