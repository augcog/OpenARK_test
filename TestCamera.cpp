//C++ include files
#include <iostream>
#include <math.h>

//OpenARK include files
#include "../Visualizer.h"

//OpenARK_test files
#include "TestCamera.h"


using namespace std;
using namespace cv;


/***
Private constructor for the tester
***/
TestCamera::TestCamera(int width, int height, double FX, double FY, double CX, double CY): 
DepthCamera(),
depth_width(width),depth_height(height),
FX(FX),FY(FY),CX(CX),CY(CY)
{
}

/***
Public deconstructor for the SR300 Camera depth sensor
***/

TestCamera::~TestCamera()
{
	
}

void TestCamera::destroyInstance()
{
	printf("closing sensor\n");

	printf("sensor closed\n");
}

/***
Create xyzMap, zMap, ampMap, and flagMap from sensor input
***/
void TestCamera::update(std::string file_name)
{
	initilizeImages();
	fillInAmps();
	fillInZCoords(file_name);
}

/***
Reads the depth data from the sensor and fills in the matrix
***/
void TestCamera::fillInZCoords(std::string file_name)
{	
	vector<Point3f>  xyzBuffer;
	auto depth_image = imread(file_name, IMREAD_ANYDEPTH);

	for (auto v = 0; v < depth_image.rows; v++)
	{
		for (auto u = 0; u < depth_image.cols; u++) 
		{
			auto depth_value = depth_image.at<uint16_t>(v, u);
			Point3f p;
			if (depth_value > 0 && depth_value!= 32001) { //32001 is invalid depth
				p.x = ((u - CX)*depth_value*(1.0f / FX)) / 1000.0f;
				p.y = ((v - CY)*depth_value*(1.0f / FY)) / 1000.0f;
				p.z = (depth_value) / 1000.0f;
			}
			else {
				p.x = p.y = p.z = 0;
			}
			xyzBuffer.emplace_back(p);
		}
	}

	xyzMap = Mat(xyzBuffer, true).reshape(3, depth_image.rows);
	
}

/***
Reads the amplitude data from the sensor and fills in the matrix
***/
void TestCamera::fillInAmps()
{
	ampMap.data = nullptr;
}

/***
Returns the X value at (i, j)
***/
float TestCamera::getX(int i, int j) const
{
	auto flat = j * depth_width * 3 + i * 3;
	return dists[flat];
}

/***
Returns the Y value at (i, j)
***/
float TestCamera::getY(int i, int j) const
{
	auto flat = j * depth_width * 3 + i * 3;
	return dists[flat + 1];
}

/***
Returns the Z value at (i, j)
***/
float TestCamera::getZ(int i, int j) const
{
	auto flat = j * depth_width * 3 + i * 3;
	return dists[flat + 2];
}

