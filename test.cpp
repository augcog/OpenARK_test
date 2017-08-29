// C++ Libraries
#include <stdio.h>
#include <string>
#include <time.h>

// OpenCV Libraries
#include "opencv2/highgui/highgui.hpp"

// OpenARK Libraries
#include "TestCamera.h"
#include "../Visualizer.h"
#include "../Hand.h"
#include "../Plane.h"
#include "../Calibration.h"
#include "../Object3D.h"
#include "../StreamingAverager.h"

using namespace cv;

int main(int argc, char** argv) {

	/***
	 *  Intrinsics for Creative Senz3D camera
	 *  for CVAR egocentric dataset
	 */
	const double FX = 224.501999;
	const double FY = 230.494003;
	const double CX = 160.000000;
	const double CY = 120.000000;

	std::string file_name;// = nullptr;
	std::ofstream os("..\\..\\OpenARK_test\\fingertips_openark.txt");


	String path_P1 = "..\\..\\OpenARK_test\\CVAR\\P1\\*_depth.png";
	String path_P3 = "..\\..\\OpenARK_test\\CVAR\\P3\\*_depth.png";
	String path_P4 = "..\\..\\OpenARK_test\\CVAR\\P4\\*_depth.png";
	String path_P5 = "..\\..\\OpenARK_test\\CVAR\\P5\\*_depth.png";
	String path_P6 = "..\\..\\OpenARK_test\\CVAR\\P6\\*_depth.png";
	String path_P7 = "..\\..\\OpenARK_test\\CVAR\\P7\\*_depth.png";

	std::vector<String> paths = {path_P1, path_P3, path_P4, path_P5, path_P6, path_P7 };


	DepthCamera * camera = new TestCamera();

	for (auto path : paths) 
	{
		std::vector<String> fn;
		glob(path, fn, false);
		auto starttime = clock();
		auto frame = 0;
		//Calibration::XYZToUnity(*pmd, 4, 4, 3);

		auto handAverager = StreamingAverager(4, 0.1);
		auto paleeteAverager = StreamingAverager(6, 0.05);

		for (auto filename : fn)
		{
			file_name = filename;
			((TestCamera*)camera)->update(file_name);

			// Loading image from sensor
			camera->removeNoise();
			if (camera->badInput) {
				waitKey(10);
			}
			
			// Classifying objects in the scene
			camera->computeClusters(0.02, 500);
			auto clusters = camera->getClusters();
			std::vector<Object3D> objects;
			auto handObjectIndex = -1, planeObjectIndex = -1;
			for (auto i = 0; i < clusters.size(); i++) 
			{
				auto obj = Object3D(clusters[i].clone());

				if (obj.hasHand)
				{
					handObjectIndex = i;
				}

				if (obj.hasPlane)
				{
					planeObjectIndex = i;
				}
				objects.push_back(obj);
			}

			// Interprate the relationship between the objects
			auto clicked = false, paletteFound = false;
			Object3D handObject, planeObject;
			Point paletteCenter(-1. - 1);
			Mat mask = Mat::zeros(camera->getXYZMap().rows, camera->getXYZMap().cols, CV_8UC1);

			// if (planeObjectIndex != -1 && handObjectIndex != -1) 
			// {
			// 	planeObject = objects[planeObjectIndex];
			// 	handObject = objects[handObjectIndex];

			// 	clicked = handObject.getHand().touchObject(planeObject.getPlane().getPlaneEquation(), planeObject.getPlane().R_SQUARED_DISTANCE_THRESHOLD * 5);
			// 	auto scene = Visualizer::visualizePlaneRegression(camera->getXYZMap(), planeObject.getPlane().getPlaneEquation(), planeObject.getPlane().R_SQUARED_DISTANCE_THRESHOLD, clicked);
			// 	//scene = Visualizer::visualizeHand(scene, handObject.getHand().pointer_finger_ij, handObject.getHand().shape_centroid_ij);
			// 	if (planeObject.leftEdgeConnected)
			// 	{
			// 		Visualizer::visualizePlanePoints(mask, planeObject.getPlane().getPlaneIndicies());
			// 		auto m = moments(mask, false);
			// 		paletteCenter = Point(m.m10 / m.m00, m.m01 / m.m00);
			// 		circle(scene, paletteCenter, 2, Scalar(0, 0, 255), 2);
			// 		paletteFound = true;
			// 	}
			// 	namedWindow("Results", CV_WINDOW_AUTOSIZE);
			// 	imshow("Results", scene);
			// }
			if (handObjectIndex != -1) 
			{
				handObject = objects[handObjectIndex];
				if (os.is_open())
				{
					os << file_name << " "; 
					int num_fingers = handObject.getHand().fingers_xyz.size();
					for (auto i = 0; i <num_fingers ; i++)
					{
						os << FX * (handObject.getHand().fingers_xyz[i][0] / handObject.getHand().fingers_xyz[i][2]) + CX << " " << FY * (handObject.getHand().fingers_xyz[i][1] / handObject.getHand().fingers_xyz[i][2]) + CY << " " << handObject.getHand().fingers_xyz[i][2] * 1000 << " ";
					}
					os << endl;
				}
			}
	
			/**** Start: Loop Break Condition ****/
			auto c = waitKey(1);
			if (c == 'q' || c == 'Q' || c == 27) {
				break;
			}
			/**** End: Loop Break Condition ****/
			frame++;
		}
	} //for (String path:paths)

	camera->destroyInstance();
	destroyAllWindows();
	return 0;
}