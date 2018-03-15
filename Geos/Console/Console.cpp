// Console.cpp : main project file.

#include "stdafx.h"

#include "SourceGeos\Geos.h"

#include <iostream>
using namespace std;


int main( int argc, char* argv[] )
{
	Geos g;

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\football.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\football.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\out1.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\koza.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\koza.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\out2.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\e4k.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\e4k.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\out3.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\bird2.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\bird2.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\out4.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\plane.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\plane.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\out5.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\monkey2.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\monkey2.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\out6.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\face.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\face.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\results\\out7.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\motor.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\motor.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\results\\out8.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\jezko.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\jezko.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\results\\out9.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\girl.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\girl.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\results\\out10.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\birds.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\birds.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\results\\out11.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\vrtak.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\vrtak.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\results\\out12.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\flower.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\flower.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\results\\out13.jpg", 0, 0 );

	//g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\bird3.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\bird3.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\results\\out14.jpg", 0, 0 );

	g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\flower2.jpg",  nullptr, nullptr, 2, 1, 1, 1 );



	cout << "end4" << endl;

	int end;
	cin >> end;

    return 0;
}
