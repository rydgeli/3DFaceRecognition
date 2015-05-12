//
//  regist.h
//  Registration
//
//  Created by Rydge on 5/4/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#ifndef Registration_regist_h
#define Registration_regist_h

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <fstream>
#include "dirent.h"

using namespace std;

#define maxPoints 3871//121//726

//typedef unsigned char uchar;

typedef struct _Point3D
{
    double x, y, z;
}Point3D;


void ReadPoint(const char *filename, vector<Point3D> &P);
void ReadPoint3D_100(const char *filename, vector<Point3D> &P);
void readPointFiles(const char* filename, vector<Point3D> &P);// too slow!!!
void SelectPoint3D(const char *filename, vector<Point3D> &P, int ratio = 100);
void Writefile(const char *filename, vector<Point3D> &cloud);
void getFiles(string path, vector<string> &files);

void ICP(vector<Point3D> &source, vector<Point3D> &target, const char *path, double *R, double *T, double e);

void FindClosestPointSet(vector<Point3D> &model, vector<Point3D> &data, vector<Point3D> &Y);

void CalculateMeanPoint3D(vector<Point3D> &P, Point3D &mean);

void MatrixMul(double *p, double *y, double *mul, int m1, int n1, int m2, int n2);
void MatrixAdd(double *a, double *b, int m, int n);
void MatrixDiv(double *a, double *b, int m, int n);
void MatrixTran(double *src, double *src_T, int m, int n);
double MatrixTR(double *m, int n);
void MatrixEigen(double *m, double *eigen, double *q, int n);
void PrintMatrix(double *M, int m, int n);

void CalculateRotation(double *q, double *R);

void TransPoint(vector<Point3D> &src, vector<Point3D> &dst, double *R, double *T,const char *path);

#endif
