// Console.cpp : main project file.

#include "stdafx.h"

#include "Geos.h"

#include <iostream>
using namespace std;


int main( int argc, char* argv[] )
{
	Geos g;

	g.Process( argv[1], argv[2], argv[3], atoi( argv[4] ), atoi( argv[5] ) );

	cout << "end4" << endl;

	int end;
	cin >> end;

    return 0;
}
