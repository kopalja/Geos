// Console.cpp : main project file.

#include "stdafx.h"

#include "Geos.h"


int main( int argc, char* argv[] )
{
	Geos g;

	g.Process( argv[1], argv[2] );

    return 0;
}
