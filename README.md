# OpenARK_test

OpenARK_test is an open-source way of testing the OpenARK platform fingertip detection. Currently, OpenARK_test can calculate the accuracy for each folder in the [CVAR dataset](https://www.tugraz.at/institute/icg/teams/teamlepetit/research/hand-detection-and-3d-pose-estimation/). Note that this dataset is released for non-commercial reseach purposes only. If using this dataset for published research please cite their [paper](https://www.tugraz.at/fileadmin/user_upload/Institute/ICG/Images/team_lepetit/publications/oberweger_cvpr16.pdf): M. Oberweger, G. Riegler, P. Wohlhart, and V. Lepetit.  Efficiently Creating 3D Training Data for Fine Hand Pose Estimation. In Conference on Computer Vision and Pattern Recognition (CVPR), 2016. <br />

Please refer to the main OpenARK github for instructions on using this code 

Note: CVAR dataset has folders named P1, P3, P4, P5, P6, and P7. P3 folder includes two hands however only the 21 joints information for the right hand is given. In the modified dataset, we have provided the user with only the depth image for the right hand by masking the left hand. CVAR dataset images are of the resolution 320*240 pixels. The camera intrinsics for the CVAR dataset collected by Creative Senz3D camera is as follows: *Fx: 224.501999*, *Fy: 230.494003*, *Cx: 160.000000*, and *Cy: 120.000000*. <br />


----
## Credits and references

[Mona Jalal](http://monajalal.com/), [Joseph Menke](https://people.eecs.berkeley.edu/~joemenke/), [Allen Y. Yang](https://people.eecs.berkeley.edu/~yang/), [S. Shankar Sastry](http://robotics.eecs.berkeley.edu/~sastry/).
