// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main(int argc, char* argv[])
{
	// global setup...

	int result = Catch::Session().run(argc, argv);
	
	// global clean-up...

	return result;
}

