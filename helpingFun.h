#ifndef HELPINGFUN_H
#define HELPINGFUN_H

#endif // HELPINGFUN_H


s

int applyCascade(const cv::Mat im, cv::Rect& rect, int flagFaceEye, const cv::Rect ROI)
{


    std::vector<cv::Rect> resultsCascade;

    if (flagFaceEye==1)
        face_cascade.detectMultiScale(im(ROI), resultsCascade, 1.1, 10, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(10, 10));
    else
        eye_cascade.detectMultiScale(im(ROI), resultsCascade, 1.1, 10, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(10, 10));
    if (resultsCascade.size() > 0)
    {
        rect = resultsCascade[0];
        rect.x = rect.x + ROI.x;
        rect.y = rect.y + ROI.y;

        return 1;
    }
    return 0;
}



int templateMatch(const cv::Mat im, cv::Mat& tpl, cv::Rect& rect, cv::Rect ROI,double threshold = 0.2, int flagEye=0) //return 1 if it's been found, 0 otherwise
{

    cv::Mat dst(ROI.width - tpl.rows + 1, ROI.height - tpl.cols + 1, CV_32FC1);
    cv::matchTemplate(im(ROI), tpl, dst, CV_TM_SQDIFF_NORMED);

    double minval, maxval;
    cv::Point minloc, maxloc;
    cv::minMaxLoc(dst, &minval, &maxval, &minloc, &maxloc);




    //if the error is lower than this threshold, we update the image, otherwise we set the rect to zero (find the fac again)
    if (minval <= threshold)
    {
        rect.x = ROI.x + minloc.x;
        rect.y = ROI.y + minloc.y;
        tpl = im(rect).clone(); //UPDATE THE  TEMPLATE (EXPERIMENTAL)

        return 1;
    }

    return 0;
}

int calculateDegree(cv::Rect eyeR, cv::Rect eyeL)
{
    int distX = eyeR.x + (eyeR.width / 2) - (eyeL.x + (eyeL.width / 2));
    //int distX = eyesR[0].x  - eyesR[1].x ;
    int distY = eyeR.y + (eyeR.height / 2) - (eyeL.y + (eyeL.height / 2)); //this is positive when right eye is up
    double h = cv::sqrt(distX *distX + distY *distY);
    if (distY>0)
        return acos(distX / h) * 360 / 3.14;
    else
        return -acos(distX / h) * 360 / 3.14;
}

#ifdef _WIN32
void GenerateKey(int vk, BOOL bExtended)
{
    KEYBDINPUT kb = { 0 };
    INPUT    Input = { 0 };
    // generate down
    if (bExtended)
        kb.dwFlags = KEYEVENTF_EXTENDEDKEY;
    kb.wVk = vk;
    Input.type = INPUT_KEYBOARD;

    Input.ki = kb;
    ::SendInput(1, &Input, sizeof(Input));

    // generate up
    ::ZeroMemory(&kb, sizeof(KEYBDINPUT));
    ::ZeroMemory(&Input, sizeof(INPUT));
    kb.dwFlags = KEYEVENTF_KEYUP;
    if (bExtended)
        kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;

    kb.wVk = vk;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    ::SendInput(1, &Input, sizeof(Input));
}

#endif
