#pragma once
// OpenCV Libraries
#include <opencv2/opencv.hpp>

// OpenARK Libraries
#include "../DepthCamera.h"

//using namespace Intel::RealSense;

/**
* Class defining the behavior of an SR300 Camera.
* Example on how to read from sensor and visualize its output
* @include SensorIO.cpp
*/
class TestCamera : public DepthCamera
{
public:

	/**
	* Public constructor initializing the Test Camera.
	* @param FX value is intrinsics matrix
	* @param FY value is intrinsics matrix		
	* @param CX value is intrinsics matrix
	* @param CY value is intrinsics matrix
	*/
	explicit TestCamera(int width, int height, double FX, double FY, double CX, double CY);




	/**
	* Deconstructor for the SR300 Camera.
	*/
	~TestCamera();

	/**
	* Replacement update function for reading from file
	* @param file_name file to read from
	*/
	void update(std::string file_name);

	/**
	* Gets new frame from sensor.
	* Updates xyzMap, ampMap, and flagMap. Resets clusters.
	*/
	
	void fillInZCoords(std::string file_name);

	/**
	* Gracefully closes the SR300 camera.
	*/
	void destroyInstance() override;

private:
	/**
	* Getter method for the x-coordinate at (i,j).
	* @param i ith row
	* @param j jth column
	* @return x-coodinate at (i,j)
	*/
	float getX(int i, int j) const;

	/**
	* Getter method for the x-coordinate at (i,j).
	* @param i ith row
	* @param j jth column
	* @return x-coodinate at (i,j)
	*/
	float getY(int i, int j) const;

	/**
	* Getter method for the x-coordinate at (i,j).
	* @param i ith row
	* @param j jth column
	* @return x-coodinate at (i,j)
	*/
	float getZ(int i, int j) const;


	/**
	* Update the values in the ampMap.
	*/
	void fillInAmps();

	//Private Variables
	float* dists;
	float* amps;
	cv::Mat frame;
	const int depth_fps = 30;
	int depth_width;
	int depth_height;
	cv::Size bufferSize;
	/***
	 *  Intrinsics for Creative Senz3D camera
	 *  for CVAR egocentric dataset
	 */
	const double FX;
	const double FY;
	const double CX;
	const double CY;
	
};