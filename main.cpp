#include "mainwindow.h"
#include <QApplication>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <Windows.h>
#include <QSlider>
#include <QMessageBox>
#include <QtMultimedia/QSound>
//#include <QSound>
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eye_cascade;


#include "HelpingFun.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    //QSound tiltSound();


    face_cascade.load("haarcascade_frontalface_alt2.xml");
    eye_cascade.load("haarcascade_eye_tree_eyeglasses.xml");

    // Open webcam

    cv::VideoCapture cap(0);

    double threshold = 0.3;

    // Check if everything is ok
    if (face_cascade.empty() || eye_cascade.empty() || !cap.isOpened())
        return QMessageBox::critical(&w, "Critical Error", "HaarCascade File (.xml) not found!");

    // Set video to 320x240
    int VideoW=320;
    int VideoH=240;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, VideoW);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, VideoH);

    std::cout.precision(3);

    int faceDetected=0;
    int ReyeDetected=0;
    int LeyeDetected=0;

    cv::Mat frame;
    cv::Mat face_tpl;
    cv::Mat rEye_tpl;
    cv::Mat lEye_tpl;

    cv::Rect faceROI;
    cv::Rect rEyeROI;
    cv::Rect lEyeROI;

    cv::Rect face_rec;
    cv::Rect eyeR_rec;
    cv::Rect eyeL_rec;


    double counter[2];
    counter[0] = 0.5;
    counter[1] = 0.5;


    int degree = 0;

    int thresholdSum = 400;
    int signalSum = 0;
    int savedHistory = 20;
    int actionTriggered = 0;



    int runCascadeEach = 20; //this will run cascade each X steps, IF THE EYES ARE ALIGNED.
    std::stringstream out;
    std::string s;
    cv::String genericText;
    std::vector<int> degreeV(savedHistory);
    std::vector<int> signalV(savedHistory);
    for (int i = 0; i < degreeV.size(); i++)
        degreeV[i] = 0;

    cap >> frame;
    cv::Size sizeFrame = frame.size();
    cv::Rect allImg;
    allImg.height = sizeFrame.height;
    allImg.width = sizeFrame.width;


    int updateTemplateFace;
    updateTemplateFace = 1;
    cv::flip(frame, frame, 1);
    int forceDetection = 0;
    int timeStepsCascadeFace = 0;
    int timeStepsCascadeRE = 0;
    int timeStepsCascadeLE = 0;
    int timeTriggeredUntilForceDetection = 25;
    int counterForceDetectoinTrigger = 0;



    while (cv::waitKey(10) != 'q' && w.isVisible())
    {


        if (w.getForceDetection())
        {
            forceDetection=1;
            w.setForceDetection(0);
        }

        if (actionTriggered == 1 || actionTriggered==-1)
            counterForceDetectoinTrigger++;
        else
            counterForceDetectoinTrigger = 0;

        if (counterForceDetectoinTrigger > timeTriggeredUntilForceDetection)
        {
            forceDetection = 1;
        }
        thresholdSum=w.getSliderValue();


        timeStepsCascadeFace++; //ignore this for now, set it to timeStepsCascade++ if you want to use it (change alos the other stuff)
        timeStepsCascadeRE++;
        timeStepsCascadeLE++;
        /*trackedR = 0;
            trackedL = 0;*/
        cap >> frame;
        if (frame.empty())
            break;

        // Flip the frame horizontally, Windows users might need this
        cv::flip(frame, frame, 1);

        cv::Mat gray;
        cv::cvtColor(frame, gray, CV_BGR2GRAY);


        //THE RETURN VALUE INDICATES IF THE DETECTION WAS SUCCESSFUL, but often we will use the rect.x,y,height and width to check if any template is available
        //(we may want to use the last useful template)

        if (faceDetected == 0 || (actionTriggered == 0 && timeStepsCascadeFace%runCascadeEach == 0) || forceDetection==1)
        {

            //w.setText("Trying to detect face...",1);
            timeStepsCascadeFace = 1; //set this to 1 if you want to use it
            faceDetected = applyCascade(gray, face_rec, 1, allImg);
            if (faceDetected)
            {
              //  w.setText("Face Detected",1);
                face_tpl = gray(face_rec);
            }
            else
                timeStepsCascadeFace = runCascadeEach - 1; //will be run again next time
        }
        if ((ReyeDetected == 0 || (actionTriggered == 0 && timeStepsCascadeRE%runCascadeEach == 0)  || forceDetection == 1) && faceDetected==1)
        {
            //w.setText("Trying to detect right eye...",1);
            timeStepsCascadeRE = 1;
            rEyeROI.x = face_rec.x + face_rec.width / 2; rEyeROI.y = face_rec.y + face_rec.height / 8; rEyeROI.width = face_rec.width / 2; rEyeROI.height = face_rec.height / 2;
            ReyeDetected = applyCascade(gray, eyeR_rec, 0, rEyeROI);
            if (ReyeDetected)
            {
              //  w.setText("Right eye detected",1);
                cv::rectangle(frame, eyeR_rec, CV_RGB(0, 255, 0));
                rEye_tpl = gray(eyeR_rec);
            }
            else
                timeStepsCascadeRE = runCascadeEach - 1;
        }
        if ((LeyeDetected == 0 || (actionTriggered == 0 && timeStepsCascadeLE%runCascadeEach == 0) || forceDetection == 1) && faceDetected == 1)
        {
            //w.setText("Trying to detect left eye...",1);
            timeStepsCascadeLE = 1;
            lEyeROI.x = face_rec.x; lEyeROI.y = face_rec.y + face_rec.height / 8; lEyeROI.width = face_rec.width / 2; lEyeROI.height = face_rec.height / 2;
            LeyeDetected = applyCascade(gray, eyeL_rec, 0, lEyeROI);
            if (LeyeDetected)
            {
              //  w.setText("Left eye detected",1);
                cv::rectangle(frame, eyeL_rec, CV_RGB(0, 0, 0));
                lEye_tpl = gray(eyeL_rec);
            }
            else
                timeStepsCascadeLE = runCascadeEach - 1;

        }

        forceDetection = 0;
        //TRACK FACE
        if (face_rec.height != 0 && face_rec.width!=0)
        {

            //cv::Size size(rect.width *0.5, rect.height *0.5);

            faceROI.x = face_rec.x - face_rec.width*0.25;
            faceROI.y = face_rec.y - face_rec.height*0.25;
            faceROI.height = face_rec.height * 1.5;
            faceROI.width = face_rec.width* 1.5;
            faceROI &= cv::Rect(0, 0, gray.cols, gray.rows);

            faceDetected = templateMatch(gray, face_tpl, face_rec, faceROI); //ROI NOT IMPLEMENTED YET
            cv::rectangle(frame, face_rec, CV_RGB(0, 255, 255));
        }
        //TRACK RIGHT EYES, IF POSSIBLE INSIDE THE ROI

        if (eyeR_rec.height!=0 && eyeR_rec.width!=0)
        {
            if (face_rec.height != 0 && face_rec.width != 0)//use the face ROI as a rectangle
            {
                rEyeROI.x = face_rec.x + face_rec.width / 2; rEyeROI.y = face_rec.y + face_rec.height / 8; rEyeROI.width = face_rec.width / 2; rEyeROI.height = face_rec.height / 2;
            }
            else
            {
                rEyeROI.x = eyeR_rec.x - eyeR_rec.width*0.25;
                rEyeROI.y = eyeR_rec.y - eyeR_rec.height*0.25;
                rEyeROI.height = eyeR_rec.height * 1.5;
                rEyeROI.width = eyeR_rec.width* 1.5;
                rEyeROI &= cv::Rect(0, 0, gray.cols, gray.rows);
            }
            /*cv::rectangle(frame, eyeR_rec, CV_RGB(0, 255, 255));
                    cv::rectangle(frame, rEyeROI, CV_RGB(255, 255, 255));*/


            ReyeDetected = templateMatch(gray, rEye_tpl, eyeR_rec,rEyeROI,threshold,1);
        }


        if (eyeL_rec.height != 0 && eyeL_rec.width != 0)
        {
            if (face_rec.height != 0 && face_rec.width != 0) //use the face ROI as a rectangle
            {
                lEyeROI.x = face_rec.x; lEyeROI.y = face_rec.y + face_rec.height / 8; lEyeROI.width = face_rec.width / 2; lEyeROI.height = face_rec.height / 2;
            }
            else
            {
                lEyeROI.x = eyeL_rec.x - eyeL_rec.width*0.25;
                lEyeROI.y = eyeL_rec.y - eyeL_rec.height*0.25;
                lEyeROI.height = eyeL_rec.height * 1.5;
                lEyeROI.width = eyeL_rec.width* 1.5;
                lEyeROI &= cv::Rect(0, 0, gray.cols, gray.rows);
            }
            /*cv::rectangle(frame, eyeL_rec, CV_RGB(0, 255, 255));
                    cv::rectangle(frame, lEyeROI, CV_RGB(255, 255, 255));*/


            LeyeDetected = templateMatch(gray, lEye_tpl, eyeL_rec, lEyeROI, threshold, 1);
        }


        //PLOT
        if (faceDetected==1)
            cv::rectangle(frame, face_rec, CV_RGB(0, 255, 0));

        if (ReyeDetected == 1)
            cv::rectangle(frame, eyeR_rec, CV_RGB(255, 0, 0));

        if (LeyeDetected == 1)
            cv::rectangle(frame, eyeL_rec, CV_RGB(255, 0, 0));

        if (LeyeDetected == 1 && ReyeDetected == 1)
        {
            degree = calculateDegree(eyeR_rec, eyeL_rec);
            std::stringstream out;			out << degree;			s = out.str();			genericText = "Degree: " + s;
            cv::putText(frame, genericText, cv::Point(face_rec.x,face_rec.y+face_rec.height), cv::FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv::LINE_AA);
        }

        //update vector degree by discarding last result, shift, put degree of this timestep in the first place
        std::rotate(degreeV.begin(), degreeV.end() - 1, degreeV.end());
        degreeV[0] = degree;
        //signalV = degreeV;
        signalSum = 0;
        for (int i = 0; i < degreeV.size(); i++)
            signalSum += degreeV[i] ^ 3 / 7000; // apply function here, directly


        if (signalSum>thresholdSum && actionTriggered == 0)
        {
            //TRIGGER ACTION: RIGHT ARROW KEY
            GenerateKey(VK_NEXT, FALSE);
            actionTriggered = 1;
            if (w.getCheckBoxValue())
            {
                //SOUND SIGNAL!
                //w.setText("Right Tilting detected",1);
                QSound::play("moveRight.wav");
            }

        }
        if (signalSum<-thresholdSum && actionTriggered == 0)
        {
            //TRIGGER ACTION: LEFT ARROW KEY
            GenerateKey(VK_PRIOR, FALSE);
            //w.setText("Left Tilting detected",1);
            actionTriggered = -1;

            if (w.getCheckBoxValue())
            {
                //SOUND SIGNAL!
                QSound::play("moveLeft.wav");
            }
        }
        if (signalSum<thresholdSum && signalSum>-thresholdSum)
            actionTriggered = 0; //release action




        out.str("");
        out << signalSum;			s = out.str();			genericText = "Rotation Value: " + s + "/"; out.str(""); out<<thresholdSum; genericText=genericText+ out.str();
        cv::putText(frame, genericText, cv::Point(face_rec.x,face_rec.y), cv::FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2, cv::LINE_AA);

        int key = cv::waitKey(1);
        if (key == 32)//SPACEBAR]
        {
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
