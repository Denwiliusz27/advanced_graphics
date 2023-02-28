#include <opencv2/opencv.hpp>
#include <string> 
#include <iostream> 
#include <cstdio>
using namespace cv;
using namespace std;

int main(int argc, char * argv[]){
    Mat images[ argc ];

    for(int i=0; i<argc; i++){
        images[i] = imread(argv[i]);
    }

    Mat img = imread("test_img/cameraman.png");
    Mat blur, ;

    cv::blur(img, blur, { 5, 5 });
    imshow("org", img);
    imshow("blur", blur);
    waitKey();
    destroyAllWindows();

    image



}