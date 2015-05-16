//
//  calculate.h
//  Identify
//
//  Created by Rydge on 5/4/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#ifndef Identify_calculate_h
#define Identify_calculate_h

#define maxPoints 3871
#define markMaxPoints 6

#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "dirent.h"

using namespace std;
using namespace cv;

typedef struct _Point3D
{
    double x, y, z;
    double w = 1;
}Point3D;

void showRGB(string imageName, string title);
void showRGB_temp(string imageName, string title);

void getFiles(string path, vector<string> &files);
void readFiles(const char *filename, vector<Point3D> &P); //too slow !!!
void readPoint(const char *filename, vector<Point3D> &P);
void readMarkPoint(const char *filename, vector<Point3D> &P);

double calculate (vector<Point3D>, vector<Point3D>, vector<Point3D>);
double calculate (vector<Point3D>, vector<Point3D>);
double calculate_depth(vector<Point3D>, vector<Point3D>);

double distance(double, double, double, double, double, double);
double distance_2D(double x1, double y1, double x2, double y2);

void Weight(vector<Point3D> &p, vector<Point3D> &mark);
void Weight(vector<Point3D> &p);


#endif
