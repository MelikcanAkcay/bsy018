#include <opencv2/opencv.hpp>



bool picam_photo (const char * filename)
{
    cv::VideoCapture cap ;
    if (!cap.open (0, cv::CAP_ANY))
        return false ; // could not open device
    cv::Mat frame ;
    if (!cap.read(frame))
    {
        cap.release();
        return false ; // could not read a frame
    }
    cap.release() ;
    if (!cv::imwrite(filename, frame))
        return false; // could not write the file
    return true;
}
int main(){
    picam_photo("test.jpg");
}