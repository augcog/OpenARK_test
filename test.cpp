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

	std::string dataset_dir;
	if(argc==2){
		dataset_dir = std::string(argv[1]);
	}else if(argc>2){
		printf("Usage: \n %s <dataset_dir>\n",argv[0]);
		return 0;
	}else{
		dataset_dir = "..\\..\\OpenARK_test\\CVAR";
	}

	/***
	 *  Intrinsics for Creative Senz3D camera
	 *  for CVAR egocentric dataset
	 */
	const double FX = 224.501999;
	const double FY = 230.494003;
	const double CX = 160.000000;
	const double CY = 120.000000;

	/**
	 * The image width resolution (pixels) that the depth sensor produces.
	 */
	const int X_DIMENSION = 321;


	/**
	 * The image height resolution (pixels) that the depth sensor produces.
	 */
	const int Y_DIMENSION = 240;

	const double VISIBLE_THRESHOLD = 25;
	const double ACCEPTED_THRESHOLD = 20;


	std::string file_name;
	std::ofstream os("..\\..\\OpenARK_test\\fingertips_openark.txt");


	std::string path_P1 = dataset_dir+"\\P1\\";
	std::string path_P3 = dataset_dir+"\\P3\\";
	std::string path_P4 = dataset_dir+"\\P4\\";
	std::string path_P5 = dataset_dir+"\\P5\\";
	std::string path_P6 = dataset_dir+"\\P6\\";
	std::string path_P7 = dataset_dir+"\\P7\\";

	std::vector<String> paths = {path_P1, path_P3, path_P4, path_P5, path_P6, path_P7 };

	//setup test camera
	DepthCamera * camera = new TestCamera(X_DIMENSION, Y_DIMENSION, FX,FY,CX,CY);

	for (auto path : paths) 
	{
		int detected_fingertips(0), total_fingertips(0);

		//The fingertips file contains the images and fingertips 
		//that we want to compare to for each folder
		std::ifstream fingertips_file;
		fingertips_file.open(path+"fingertips.txt");
		if(!fingertips_file.is_open()){
			continue;
		}
		// Setup OpenARK
		auto starttime = clock();
		auto frame = 0;
		auto handAverager = StreamingAverager(4, 0.1);
		auto paleeteAverager = StreamingAverager(6, 0.05);
		
		// Read each line in the fingertips file
		char buffer[1024];
		fingertips_file.getline(buffer,1024);
		while(!fingertips_file.eof())
		{
			// Extract image name from line
			std::vector<std::string> elems = Util::split(buffer," \n");
			file_name = path+elems[0];

			// Create fingertips vector from remaining elements
			std::vector<cv::Vec3f> fingertips(5);
			for(int i =0; i <5; i++){
				fingertips[i] = cv::Vec3f(stod(elems[1+3*i]),stod(elems[2+3*i]),stod(elems[3+3*i]));
			}

			// Load Image
			((TestCamera*)camera)->update(file_name);

			// Clean
			camera->removeNoise();
			if (camera->badInput) {
				waitKey(10);
			}
			
			// Classify objects in the scene
			camera->computeClusters(0.02, 500);
			auto clusters = camera->getClusters();
			std::vector<Object3D> objects;
			auto handObjectIndex = -1;
			for (auto i = 0; i < clusters.size(); i++) 
			{
				auto obj = Object3D(clusters[i].clone());

				if (obj.hasHand)
				{
					handObjectIndex = i;
				}

				objects.push_back(obj);
			}

			Object3D handObject;
			cv::Mat xyz = camera->getXYZMap();
			if (handObjectIndex != -1) 
			{
				handObject = objects[handObjectIndex];
				int num_fingers = handObject.getHand().fingers_xyz.size();
				for(int i = 0; i < 5; i++){
					//Check if the fingertip is visible
					if(fingertips[i][2] <= xyz.at<cv::Vec3f>((int)fingertips[i][1],(int)fingertips[i][0])[2]*1000+VISIBLE_THRESHOLD){
						//circle(xyz, Point((int)fingertips[i][0],(int)fingertips[i][1]), 3, Scalar(255, 0, 0), 2);
						double mindist = ACCEPTED_THRESHOLD;
						cv::Point their_point((int)fingertips[i][0],(int)fingertips[i][1]);
						for (auto j = 0; j <num_fingers ; j++)
						{
							cv::Point our_point( 
								FX * (handObject.getHand().fingers_xyz[j][0] / handObject.getHand().fingers_xyz[j][2]) + CX,
							 	FY * (handObject.getHand().fingers_xyz[j][1] / handObject.getHand().fingers_xyz[j][2]) + CY);
							
							double dist = Util::euclideanDistance2D(our_point,their_point);
							if(dist<mindist)
								mindist=dist;
						}
						if(mindist<ACCEPTED_THRESHOLD){
							detected_fingertips++;
						}
						total_fingertips++;
					}
				}
			}else{
				for(int i = 0; i < 5; i++){
					//Check if the fingertip is visible
					if(fingertips[i][2] <= xyz.at<cv::Vec3f>((int)fingertips[i][1],(int)fingertips[i][0])[2]*1000+VISIBLE_THRESHOLD){
						//circle(xyz, Point((int)fingertips[i][0],(int)fingertips[i][1]), 3, Scalar(255, 0, 0), 2);
						total_fingertips++;
					}
				}
			}
			//imshow("test",xyz);
	
			/**** Start: Loop Break Condition ****/
			auto c = waitKey(1);
			if (c == 'q' || c == 'Q' || c == 27) {
				camera->destroyInstance();
				destroyAllWindows();
				return 0;
			}
			/**** End: Loop Break Condition ****/
			frame++;
			fingertips_file.getline(buffer,1024);
		}

		printf("Percent Correct: %f\n", detected_fingertips/(float)total_fingertips);
	} //for (String path:paths)

	camera->destroyInstance();
	destroyAllWindows();
	return 0;
}