/**
 * Program by Fariz A. written as 02/02/16 15:28:12 GMT+7
 * Solving double-sided orthogonal maze, via eroding walls
 * 
 */
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main(int argc, char** argv)
{

    /* Command Line argument is your friend, as usual.....*/

    if( argc != 2)
     {
      std::cout <<" Usage: display_image ImageToLoadAndDisplay" << std::endl;
      return -1;
     }
   

   /* Load image and check validity of file*/  

     cv::Mat src;
     src = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

     if(! src.data )                              
     {
         std::cout <<  "Could not open or find the image" << std::endl ;
         return -1;
     }


   /*Convert to binary image*/
    cv::Mat bw;
    cv::cvtColor(src, bw, CV_BGR2GRAY);
    cv::threshold(bw, bw, 10, 255, CV_THRESH_BINARY_INV);

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


   /*Unfortunately, I havent found a way to solve branched-solution maze*/

    if (contours.size() != 2)
    {
        std::cout << "This is not a 'perfect maze' with just 2 walls!" << std::endl;
        return -1;
    }
    
    /*Since we have 2 walls, get the first wall first....*/
    cv::Mat path = cv::Mat::zeros(src.size(), CV_8UC1);
    cv::drawContours(path, contours, 0, CV_RGB(255,255,255), CV_FILLED);

   /*Dilate wall pixels, don't hestitate to experiment by varying the value for best result*/
    cv::Mat kernel = cv::Mat::ones(19, 19, CV_8UC1); 
    cv::dilate(path, path, kernel);

   /*Erode wall pixels*/
    cv::Mat path_erode;
    cv::erode(path, path_erode, kernel);

    /*Dilated - Eroded = Solution!*/
    cv::absdiff(path, path_erode, path);

    /*We'll use red as the solution, experiment with channels for different colour. I found this the easiest*/
    std::vector<cv::Mat> channels;
    cv::split(src, channels);
    channels[0] &= ~path;
    channels[1] &= ~path;
    channels[2] |= path;

   /*Show all progress.*/
    cv::Mat dst;
    cv::merge(channels, dst);
    cv::imshow("src", src);
    cv::imshow("bw", bw);
    //cv::waitKey(0);
    cv::imshow("path", path);
    //cv::waitKey(0);
    cv::imshow("path_erode", path_erode);
    //cv::waitKey(0);
    cv::imshow("solution", dst);
    cv::waitKey(0);

    return 0;
}
