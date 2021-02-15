#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc.hpp" // #include "opencv2/videoio.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>

using namespace cv;
using namespace std;

typedef struct{
    Mat* img;
    set<int>* colors;
} ClusteringParams;

int main(int argc, char **argv)
{

    void onMouse(int event, int x, int y, int, void* params);
    int readFile(string file, set<int>& colors);
    void saveToFile(string filePath, set<int>& colors);
    void clusterizarImagem(Mat& img, set<int> colors);
    int vec3bToInt(cv::Vec3b vec);
    set<int> clusteredColors;
    readFile("colors.txt", clusteredColors);
    bool playing = true;
    VideoCapture* videoCap = new VideoCapture(argv[1]);
    // videoCap->set(CV_CAP_PROP_FOURCC, CV_FOURCC('F', 'F', 'V', '1'));
    cout << videoCap->set(CV_CAP_PROP_FOURCC, (int)CV_FOURCC('L', 'A', 'G', 'S'))<< endl;
    Mat frame, ffillImg;
    ClusteringParams clusParams = {&frame, &clusteredColors};
    namedWindow("frame");
    setMouseCallback("frame", onMouse, &clusParams);
    unsigned char bytes[4];
    // videoCap->set(CV_CAP_PROP_FOURCC, CV_FOURCC('H', 'F', 'Y', 'U'));
    unsigned long n = videoCap->get(CV_CAP_PROP_FOURCC);

    bytes[3] = (n >> 24) & 0xFF;
    bytes[2] = (n >> 16) & 0xFF;
    bytes[1] = (n >> 8) & 0xFF;
    bytes[0] = n & 0xFF;
    cout << "f;dsklfls " << bytes << endl;
    cout <<
    while(playing){
        *videoCap >> frame;
        if(frame.empty()){
            cout << "empty";
            playing = false;
            break;
        }
        frame.copyTo(ffillImg);
        clusterizarImagem(ffillImg, clusteredColors);
        imshow("frame", ffillImg);
        waitKey(30);
    }
    saveToFile("colors.txt", clusteredColors);
    waitKey();
    return 0;
}
int vec3bToInt(cv::Vec3b vec){
    return ( ((int)vec.val[0]) << 16 )
        | ( ((int)vec.val[1]) << 8 )
        | ( (int)vec.val[2] );
}
void clusterizarImagem(Mat& img, set<int> colors){
    for (size_t i = 0; i < img.rows; i++) {
        for (size_t j = 0; j < img.cols; j++) {
            if(colors.find(vec3bToInt(img.at<Vec3b>(i, j))) != colors.end()){
                img.at<Vec3b>(i, j) = {255, 0, 200};
            }
        }
    }
}
void onMouse(int event, int x, int y, int, void* params){
    ClusteringParams* clusParams = (ClusteringParams*) params;
    Mat img = *clusParams->img;
    if(!img.data)
       cout << "NULL IMAGE ";
    if (event != CV_EVENT_LBUTTONDOWN)
        return;
    else if(x> img.size().width || x < 0 || y > img.size().height || y < 0)
        return;
    set<int>* colors = clusParams->colors;
    cv::Point seed = cv::Point(x,y);
    Mat floodFilledImg = img.clone();
    floodFill(floodFilledImg, seed, Scalar(255, 0, 200), 0, Scalar(10, 10, 10), Scalar(10, 10, 10));
    for (size_t i = 0; i < img.rows; i++) {
        for (size_t j = 0; j < img.cols; j++) {
            if(floodFilledImg.at<Vec3b>(i, j) == Vec3b{255, 0, 200}){
                colors->insert(vec3bToInt(img.at<Vec3b>(i,j)));
            }
        }
    }
}
int readFile(string file, set<int>& colors){
    int c;
    //cout<< "Loading file..." << endl;
    string line;
    ifstream inFile(file.c_str(), ios::in);
    if (inFile)
    {
        while (getline(inFile, line))
        {
            istringstream actualColor(line);
            actualColor >> c;
            //cout << c << endl;
            colors.insert(c);
            actualColor.str("");
        }
        inFile.close();
        return 0;
    }
    else
        //cout << "could not open file, creating a new one..." << endl;
    return -1;
}
void saveToFile(string filePath, set<int>& colors){
    ofstream outFile(filePath.c_str());
    for(auto& color : colors){
        outFile << color << endl;
    }
    outFile.close();
}
