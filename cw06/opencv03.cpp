#include <opencv2/opencv.hpp>
#include <string> 
#include <iostream> 
#include <cstdio>
using namespace cv;
using namespace std;

int main(int argc, char * argv[])
{
    double new_size = 128.0;
    Mat resized_down;
    double a, new_width, new_height;

    // const string file_name = "test_img/lena_color.png";
    const string file_name = "random.png";
    Mat img = imread(file_name);
    double width = img.cols;
    double height = img.rows;

    a = height / width;

    if (height < width){
        cout << "szerszy" << endl;
        new_width = new_size;
        new_height = new_width * a;
    } else if (height > width){
        cout << "wyzszy" << endl;
        new_height = new_size;
        new_width = new_height * a;
    } else {
        cout << "taki sam" << endl;
        new_height = new_size;
        new_width = new_size;
    }

    cout << new_height << endl;
    cout << new_width << endl;

    resize(img, img, Size(new_width, new_height), INTER_LINEAR);

    if (! cv::imwrite("wynik_zad01.png", img)) {
            std::cerr << "Błąd przy zapisie do pliku.\n";
    }


}