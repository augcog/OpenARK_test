//C++ include files
#include <iostream>
#include <math.h>

//OpenARK include files
#include "../Visualizer.h"
#include "../global.h"

//OpenARK_test files
#include "TestCamera.h"


using namespace std;
using namespace cv;


/***
Private constructor for the tester
***/
TestCamera::TestCamera(bool use_live_sensor): DepthCamera()
{
	depth_width = X_DIMENSION;
	depth_height = Y_DIMENSION;
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
void TestCamera::update()
{
	initilizeImages();
	fillInAmps();
	fillInZCoords();
}

/***
Reads the depth data from the sensor and fills in the matrix
***/
void TestCamera::fillInZCoords()
{	
	vector<Point3f>  xyzBuffer;
	auto depth_image = imread(file_name, IMREAD_ANYDEPTH);
	//if you want to test only a single image uncomment below line
	//auto depth_image = imread("C:\\OpenARK_test\\CVAR\\P4\\000100_depth.png", IMREAD_ANYDEPTH);

	String xyzMap_file_name = file_name.substr(0, 36) + "_modified.png";
	namedWindow("depth", WINDOW_AUTOSIZE);
	imshow("depth", depth_image);

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
	namedWindow("xyzMap", WINDOW_AUTOSIZE);
	imshow("xyzMap", xyzMap);
	//saving the depth heatmap for improving the manual annotation process
	imwrite(xyzMap_file_name, xyzMap);
	//cv::imshow("Depth Image by OpenARK", Visualizer::visualizeDepthMap(xyzMap));
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

