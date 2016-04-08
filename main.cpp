#include "mainwindow.h"
#include <QApplication>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif




#include <QSlider>

#include <QtMultimedia/QSound>
#include <QCamera>
#include <QCameraImageCapture>
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eye_cascade;


#include "HelpingFun.h"


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    w.callInstructions();

    //QSound tiltSound();

    cv::namedWindow("Cam Viewer");
    face_cascade.load("haarcascade_frontalface_alt2.xml");
    eye_cascade.load("haarcascade_eye_tree_eyeglasses.xml");

    // Open webcam

    cv::VideoCapture cap;
    cap.open(0);

    // Check if everything is ok
    if (face_cascade.empty() || eye_cascade.empty())
        return QMessageBox::critical(&w, "Critical Error", "HaarCascade File (.xml) not found!");

    if ( !cap.isOpened())
        return QMessageBox::critical(&w, "Critical Error", "Webcam cannot be open!");

    // Set video to 320x240
    int origVideoW=320;
    int origVideoH=240;

    int VideoW=origVideoW; int VideoH=origVideoH;

    //cap.set(CV_CAP_PROP_FRAME_WIDTH, VideoW);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, VideoH);

    std::cout.precision(3);

    int faceDetected=0;

    cv::Mat origFrame;
    cv::Mat frame;


    cv::Rect faceROI;
    cv::Rect face_rec;



    double counter[2];
    counter[0] = 0.5;
    counter[1] = 0.5;





    cap >> origFrame;
    //  cv::Size sizeFrame = frame.size();
    cv::Rect origAllImg; cv::Rect allImg;

    //allImg.height = sizeFrame.height;
    //allImg.width = sizeFrame.width;]#
    origAllImg.height=origVideoH; origAllImg.width=origVideoW;
    allImg=origAllImg;

    int updateTemplateFace;
    updateTemplateFace = 1;
    cv::flip(frame, frame, 1);
    int forceDetection = 0;
    int actionTriggered=0;


    int colorRect=100;

    int angle=w.getSliderValue();

    cap.read(origFrame);

    int flagRot=0;


    int counterLeft=0;
    int counterRight=0;
    int thresholdCounter=5;

    cv::Mat gray;
    cv::Mat grayRot;
    cv::Point center = cv::Point(origVideoW/2, origVideoH/2 );
    cv::Mat rot_matR = cv::getRotationMatrix2D( center, angle, 1);
    cv::Mat rot_matL = cv::getRotationMatrix2D( center, -angle, 1);

    int tryFrame=0;
    while (cv::waitKey(10) != 'q' && w.isVisible())
    {


        cap >> frame;
        cv::flip(frame,frame,1);
        cv::resize(frame,frame,cv::Size(origVideoW, origVideoH));

        // frame=origFrame;

        if ((w.rotate==1 || w.rotate==3) && flagRot==0)
        {
            face_cascade.load("haarcascade_frontalface_alt2.xml");
            eye_cascade.load("haarcascade_eye_tree_eyeglasses.xml");
            face_rec.height=0;face_rec.width=0;
            faceDetected=0;
            flagRot=1;
            VideoH=origVideoW;
            VideoW=origVideoH;
            allImg.height=VideoH; allImg.width=VideoW; //there is an error here on in the next one
            forceDetection=1;

        }
        if ((w.rotate==0 || w.rotate==2 ) &&  flagRot==1)
        {
            face_cascade.load("haarcascade_frontalface_alt2.xml");
            eye_cascade.load("haarcascade_eye_tree_eyeglasses.xml");
            face_rec.height=0;face_rec.width=0;
            faceDetected=0;
            flagRot=0;
            VideoH=origVideoH;
            VideoW=origVideoW;
            allImg.height=VideoH; allImg.width=VideoW;

            forceDetection=1;
        }

        if (w.rotate==1)
        {
            cv::flip(frame,frame,0);
            cv::transpose(frame, frame);
        }
        if (w.rotate==2)
        {
            cv::transpose(frame,frame);
            cv::flip(frame,frame,1);
            cv::transpose(frame,frame);
        }
        if (w.rotate==3)
        {
            cv::transpose(frame,frame);
        }


        if (w.flipHorzValue==1)
        {
            cv::flip(frame,frame,1);
        }
        if (w.flipVertValue==1)
        {
            cv::flip(frame,frame,0);
        }

        if (frame.empty())
        {
            if ( !cap.isOpened())
                return QMessageBox::critical(&w, "Critical Error", "Webcam cannot be open!");
            if (++tryFrame>2) QMessageBox::critical(&w, "Critical Error", "Webcam cannot be open!");
            else QMessageBox::about(&w, "Error", "Webcam cannot be open! Retry...");

        }
        if (w.isPaused()==1)
        {
            cv::imshow("Cam Viewer",frame);
            continue;
        }




        cv::cvtColor(frame, gray, CV_BGR2GRAY);

        if (w.getSliderValue()!=angle)
        {
            angle=w.getSliderValue();
            rot_matR = cv::getRotationMatrix2D( center, angle, 1);
            rot_matL = cv::getRotationMatrix2D( center, -angle, 1);
        }
        //THE RETURN VALUE INDICATES IF THE DETECTION WAS SUCCESSFUL, but often we will use the rect.x,y,height and width to check if any template is available
        //(we may want to use the last useful template)

        faceDetected=0;
        int rightDetected=0;
        int leftDetected=0;


        if (faceDetected==0)
            faceROI=allImg;

        faceDetected = applyCascade(gray, face_rec, 1, faceROI);

        if (faceDetected)
        {
            actionTriggered=0;
            counterRight=counterLeft=0;
        }
        //DON'T KNOW HOW TO SOLVE THIS!
        if (faceDetected)
            faceROI=scaleSizeRect(face_rec, 20);


        //try rotated image (right
        if (~faceDetected)
        {

        cv::warpAffine(gray, grayRot, rot_matR, gray.size() );
        //cv::imshow("rotateR", grayRot);
        rightDetected= applyCascade(grayRot, face_rec, 1, allImg);
        if (rightDetected)
            counterRight++;
        }

        if (~faceDetected && ~rightDetected)
        {

        cv::warpAffine(gray, grayRot, rot_matL, gray.size() );
       // cv::imshow("rotateL", grayRot);
        leftDetected= applyCascade(grayRot, face_rec, 1, allImg);
        if (leftDetected)
            counterLeft++;
        }


        //PLOT
        if (faceDetected || rightDetected || leftDetected)
        {
            cv::rectangle(frame, face_rec, CV_RGB(255*leftDetected, 255, 255*rightDetected));
            cv::rectangle(frame, faceROI, CV_RGB(255, 255, 0));
        }


        //if (rightDetected && actionTriggered==0)
        if (counterRight>thresholdCounter && actionTriggered==0)
        {
            //TRIGGER ACTION: RIGHTARROW KEY
            #ifdef _WIN32
            GenerateKey(VK_NEXT, FALSE);
            #endif
            #ifdef linux
            system("xdotool key Page_Down");
            #endif
            actionTriggered = 1;

            if (w.getCheckBoxValue())
            {
                //SOUND SIGNAL!
                QSound::play(":/myFile/moveLeft.wav");
            }
        }

        if (counterLeft>thresholdCounter && actionTriggered==0)
        {
            //TRIGGER ACTION: LEFT ARROW KEY
            #ifdef _WIN32
            GenerateKey(VK_PRIOR, FALSE);
            #endif
            #ifdef linux
            system("xdotool key Page_Up");
            #endif
            actionTriggered = -1;

            if (w.getCheckBoxValue())
            {
                //SOUND SIGNAL!
                QSound::play(":/myFile/moveLeft.wav");
            }
        }







        int key = cv::waitKey(1);
        if (key == 32)//SPACEBAR]
        {
            std::cout <<  "\n\n\n\n M = " << " "  << gray << endl << endl ;
            forceDetection = 1; //force detection + reset degreev
            /*for (int i = 0; i < degreev.size(); i++)
                                degreev[i] = 0;*/
        }

        if (actionTriggered==1)
            cv::circle(frame, cv::Point(3*VideoW/4,VideoH/2), VideoH/20, CV_RGB(0, 255, 0),-1 );

        if (actionTriggered==-1)
            cv::circle(frame, cv::Point(1*VideoW/4,VideoH/2), VideoH/20, CV_RGB(0, 255, 0),-1 );

        cv::imshow("Cam Viewer",frame);

    }


    cv::destroyAllWindows();
    //return a.exec();
    QCoreApplication::exit(1);
}
