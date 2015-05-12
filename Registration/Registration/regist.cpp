//
//  regist.cpp
//  Registration
//
//  Created by Rydge on 5/4/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#include "regist.h"

void ReadPoint(const char *filename, vector<Point3D> &P)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Open Error!!!");
        exit(1);
    }
    else
    {
        for (int i = 0; i<maxPoints; i++)
        {
            Point3D p;
            fscanf(fp, "%lf%lf%lf", &p.x, &p.y, &p.z);
            P.push_back(p);
        }
    }
    fclose(fp);
}

void ReadPoint3D_100(const char *filename, vector<Point3D> &P)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Open Error!!!");
        exit(1);
    }
    else
    {
        for (int i = 0; i<100; i++)
        {
            Point3D p;
            fscanf(fp, "%lf%lf%lf", &p.x, &p.y, &p.z);
            P.push_back(p);
        }
    }
    
    fclose(fp);
}

void readPointFiles(const char* filename, vector<Point3D> &P)
{
    fstream file;
    file.open(filename, ios::in);
    while (file.peek() != EOF)
    {
        Point3D p;
        file >> p.x;
        file >> p.y;
        file >> p.z;
        if (file.fail())
            break;
        P.push_back(p);
    }
    file.close();
}

void SelectPoint3D(const char *filename, vector<Point3D> &P, int ratio)
{
    FILE *fp = fopen(filename, "r");
    char str[10];
    int i = 0;
    
    while (fscanf(fp, "%s", str) != EOF)
    {
        if (strcmp(str, "v") == 0)
        {
            Point3D p;
            fscanf(fp, "%lf%lf%lf", &p.x, &p.y, &p.z);
            
            if (i%ratio == 0)
                P.push_back(p);
            i++;
        }
    }
    
    fclose(fp);
}

void getFiles(string path, vector<string> &files)
{
    DIR* dir;
    dirent* pdir;
    
    dir = opendir(path.c_str());
    int count=0;
    while ((pdir = readdir(dir))) {
        if (count>=2)
        {
            if(strcmp(pdir->d_name,".DS_Store"))
                files.push_back(pdir->d_name);
        }
        count++;
    }
    
    closedir(dir);
}


void ICP(vector<Point3D> &source, vector<Point3D> &target, const char *path, double *R, double *T, double e)
{
    // P->source    Y->target
    printf("|||||||||||||||||||\nRegistration Begins!\n|||||||||||||||||||||\n");
    vector<Point3D> P, Y;
    vector<Point3D>::iterator it1, it2;
    double pre_d = 0.0, d = 0.0;
    int round = 0;
    P = source;
    //
    Y = target;
    //
    do
    {
        pre_d = d;
        double R1[9], T1[3];
        //FindClosestPointSet(target, P, Y);
        //P - original source; Y - new cloest points set of target
        Point3D _mean_P, _mean_Y;
        CalculateMeanPoint3D(P, _mean_P);//Source
        CalculateMeanPoint3D(Y, _mean_Y);//new set of Target
        double m[9], A[9], delta[3];
        for (int i = 0; i<9; i++)
        {
            m[i] = 0;
        }
        it1 = P.begin();// Source
        it2 = Y.begin();// new set of Target
        for (; it1 != P.end(); it1++, it2++)
        {
            double p[3] = { it1->x, it1->y, it1->z }, y[3] = {it2->x, it2->y, it2->z}, mul[9];
            
            //p[0] = it1->x;
            //p[1] = it1->y;
            //p[2] = it1->z;
            
            //y[0] = it2->x;
            //y[1] = it2->y;
            //y[2] = it2->z;
            
            MatrixMul(p, y, mul, 3, 1, 1, 3);
            MatrixAdd(m, mul, 3, 3);
        }
        
        for (int i = 0; i<9; i++)
        {
            m[i] = m[i] / (double) P.size();
        }
        
        double mean_P[3], mean_Y[3], mul[9];
        
        mean_P[0] = _mean_P.x;
        mean_P[1] = _mean_P.y;
        mean_P[2] = _mean_P.z;
        
        mean_Y[0] = _mean_Y.x;
        mean_Y[1] = _mean_Y.y;
        mean_Y[2] = _mean_Y.z;
        
        MatrixMul(mean_P, mean_Y, mul, 3, 1, 1, 3);
        MatrixDiv(m, mul, 3, 3);
        
        double m_T[9];
        MatrixTran(m, m_T, 3, 3);
        
        for (int i = 0; i<9; i++)
        {
            A[i] = m[i];
        }
        
        MatrixDiv(A, m_T, 3, 3);
        
        delta[0] = A[5];
        delta[1] = A[6];
        delta[2] = A[1];
        
        double tr = MatrixTR(m, 3);
        MatrixAdd(m, m_T, 3, 3);
        
        double I3[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
        I3[0] = tr;
        I3[4] = tr;
        I3[8] = tr;
        
        MatrixDiv(m, I3, 3, 3);
        
        double Q[16];
        Q[0] = tr;
        
        for (int i = 1; i <= 3; i++)
            Q[i] = delta[i - 1];
        
        for (int i = 1; i <= 3; i++)
            Q[i * 4] = delta[i - 1];
        
        for (int i = 1; i <= 3; i++)
        {
            Q[i * 4 + 1] = m[4 * (i - 1) + 0];
            Q[i * 4 + 2] = m[4 * (i - 1) + 1];
            Q[i * 4 + 3] = m[4 * (i - 1) + 2];
        }
        
        double eigen, qr[4];
        MatrixEigen(Q, &eigen, qr, 4);
        
        CalculateRotation(qr, R1);
        
        double qt[3];
        for (int i = 0; i<3; i++)
            qt[i] = mean_Y[i];
        
        double mul1[3];
        MatrixMul(R1, mean_P, mul1, 3, 3, 3, 1);
        MatrixDiv(qt, mul1, 3, 1);
        
        for (int i = 0; i<3; i++)
            T1[i] = qt[i];
        
        d = 0.0;
        it1 = P.begin();
        it2 = Y.begin();
        int pcount = 0;
        for (; it1 != P.end(); it1++, it2++)
        {
            double sum = (it1->x - it2->x)*(it1->x - it2->x) + (it1->y - it2->y)*(it1->y - it2->y) + (it1->z - it2->z)*(it1->z - it2->z);
            //sum = sqrt(sum);
            //double sum = (it1->z - it2->z);
            d += sum;
            pcount++;
        }
        d = d / (double)(pcount);
        round++;
        
        if (round > 1)
        {
            printf("\nRound: ====== %d\n", round);
            
            //打印R和T
            
            //printf("R:\n");
            //for (int i = 0; i<3; i++)
            //	printf("%lf %lf %lf\n", R[3 * i], R[3 * i + 1], R[3 * i + 2]);
            //printf("T:\n%lf %lf %lf\n", T[0], T[1], T[2]);
            
        }
        printf("D - Pre_D: %lf\n", abs(d - pre_d));
        
        if (abs(d - pre_d) >= e)
        {
            if (round == 1)
            {
                for (int i = 0; i<9; i++)
                    R[i] = R1[i];
                
                for (int i = 0; i<3; i++)
                    T[i] = T1[i];
            }
            else
            {
                double tempR[9], tempT[3];
                MatrixMul(R1, R, tempR, 3, 3, 3, 3);
                MatrixMul(R1, T, tempT, 3, 3, 3, 1);
                
                for (int i = 0; i<9; i++)
                    R[i] = tempR[i];
                
                for (int i = 0; i<3; i++)
                    T[i] = tempT[i] + T1[i];
            }
            TransPoint(source, P, R, T, path);
        }
        else
        {
            Writefile(path, P);
        }
        
    } while (abs(d - pre_d) >= e && round < 20 );
    
    //it1 = P.begin();
    //source.clear();
    //for (; it1 != P.end(); it1++)
    //	source.push_back(*it1);
}

void Writefile(const char *filename, vector<Point3D> &cloud)
{
    FILE *fp = fopen(filename, "w");
    
    //	fprintf(fp, "Number of Points: %lu\n", cloud.size());
    
    vector<Point3D>::iterator it1;
    
    for (it1 = cloud.begin(); it1 != cloud.end(); it1++)
    {
        fprintf(fp, "%lf %lf %lf\n", it1->x, it1->y, it1->z);
    }
    
    fclose(fp);
}

void FindClosestPointSet(vector<Point3D> &target, vector<Point3D> &source, vector<Point3D> &Y)
{
    vector<Point3D>::iterator it1, it2;
    //it1 - source it2 - target
    Y.clear();
    
    for (it1 = source.begin(); it1 != source.end(); it1++)
    {
        double min;
        vector<Point3D>::iterator it;
        it2 = target.begin();
        it = it2;
        min = (it1->x - it2->x)*(it1->x - it2->x) + (it1->y - it2->y)*(it1->y - it2->y) + (it1->z - it2->z)*(it1->z - it2->z);
        it2++;
        
        for (; it2 != target.end(); it2++)
        {
            double d;
            d = (it1->x - it2->x)*(it1->x - it2->x) + (it1->y - it2->y)*(it1->y - it2->y) + (it1->z - it2->z)*(it1->z - it2->z);
            
            if (d < min)
            {
                min = d;
                it = it2;
            }
        }
        
        Y.push_back(*it);
    }
}

//计算点云P的中心点Mean
void CalculateMeanPoint3D(vector<Point3D> &P, Point3D &mean)
{
    vector<Point3D>::iterator it;
    
    mean.x = 0;
    mean.y = 0;
    mean.z = 0;
    
    for (it = P.begin(); it != P.end(); it++)
    {
        mean.x += it->x;
        mean.y += it->y;
        mean.z += it->z;
    }
    
    mean.x = mean.x / P.size();
    mean.y = mean.y / P.size();
    mean.z = mean.z / P.size();
}

void MatrixMul(double *p, double *y, double *mul, int m1, int n1, int m2, int n2)
{
    if (n1 != m2)
        return;
    
    CvMat _p = cvMat(m1, n1, CV_64F, p);
    CvMat _y = cvMat(m2, n2, CV_64F, y);
    CvMat _mul = cvMat(m1, n2, CV_64F, mul);
    
    cvMatMul(&_p, &_y, &_mul);
}

void MatrixAdd(double *a, double *b, int m, int n)
{
    for (int i = 0; i<m; i++)
    {
        for (int j = 0; j<n; j++)
        {
            a[i*n + j] = a[i*n + j] + b[i*n + j];
        }
    }
    
}

void MatrixDiv(double *a, double *b, int m, int n)
{
    for (int i = 0; i<m; i++)
    {
        for (int j = 0; j<n; j++)
        {
            a[i*n + j] = a[i*n + j] - b[i*n + j];
        }
    }
}

void MatrixTran(double *src, double *src_T, int m, int n)
{
    for (int i = 0; i<n; i++)
    {
        for (int j = 0; j<m; j++)
        {
            src_T[i*m + j] = src[j*n + i];
        }
    }
}

double MatrixTR(double *m, int n)
{
    double sum = 0.0;
    
    for (int i = 0; i<n; i++)
        sum += m[i*n + i];
    
    return sum;
}

void PrintMatrix(double *M, int m, int n)
{
    for (int i = 0; i<m; i++)
    {
        for (int j = 0; j<n; j++)
        {
            printf("%10.4lf ", M[i*n + j]);
        }
        printf("\n");
    }
}

void MatrixEigen(double *m, double *eigen, double *q, int n)
{
    double *vec, *eig;
    
    vec = new double[n*n];
    eig = new double[n];
    
    CvMat _m = cvMat(n, n, CV_64F, m);
    CvMat _vec = cvMat(n, n, CV_64F, vec);
    CvMat _eig = cvMat(n, 1, CV_64F, eig);
    
    cvEigenVV(&_m, &_vec, &_eig);
    
    *eigen = eig[0];
    for (int i = 0; i<n; i++)
        q[i] = vec[i];
    
    delete[] vec;
    delete[] eig;
}

void CalculateRotation(double *q, double *R)
{
    R[0] = q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
    R[1] = 2.0 * (q[1] * q[2] - q[0] * q[3]);
    R[2] = 2.0 * (q[1] * q[3] + q[0] * q[2]);
    R[3] = 2.0 * (q[1] * q[2] + q[0] * q[3]);
    R[4] = q[0] * q[0] - q[1] * q[1] + q[2] * q[2] - q[3] * q[3];
    R[5] = 2.0 * (q[2] * q[3] - q[0] * q[1]);
    R[6] = 2.0 * (q[1] * q[3] - q[0] * q[2]);
    R[7] = 2.0 * (q[2] * q[3] + q[0] * q[1]);
    R[8] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
}

void TransPoint(vector<Point3D> &src, vector<Point3D> &dst, double *R, double *T, const char *path)
{
    vector<Point3D>::iterator it;
    dst.clear();
    
    for (it = src.begin(); it != src.end(); it++)
    {
        double p[3], r[3];
        p[0] = it->x;
        p[1] = it->y;
        p[2] = it->z;
        
        MatrixMul(R, p, r, 3, 3, 3, 1);
        MatrixAdd(r, T, 3, 1);
        
        Point3D point = *it;
        point.x = r[0];
        point.y = r[1];
        point.z = r[2];
        
        dst.push_back(point);
        Writefile(path, dst);
    }
}
