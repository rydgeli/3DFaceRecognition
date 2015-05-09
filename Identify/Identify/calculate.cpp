//
//  calculate.cpp
//  Identify
//
//  Created by Rydge on 5/4/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#include "calculate.h"

void readFiles(const char *filename, vector<Point3D> &P)
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

void readPoint(const char*filename, vector<Point3D> &P)
{
    FILE *fp;
    fp = fopen(filename, "r");
    
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

void readMarkPoint(const char *filename, vector<Point3D> &P)
{
    FILE *fp;
    fp = fopen(filename, "r");
    
    if (fp == NULL)
    {
        printf("Open Error!!!");
        exit(1);
    }
    else
    {
        for (int i = 0; i<markMaxPoints; i++)
        {
            Point3D p;
            fscanf(fp, "%lf%lf%lf", &p.x, &p.y, &p.z);
            P.push_back(p);
        }
    }
    fclose(fp);
}

double calculate(vector<Point3D> source, vector<Point3D> target, vector<Point3D> mark)
{
    Weight(source,mark);
    vector<Point3D>::iterator it1, it2;
    double dist = 0, temp = 0;
//    int count = 0;
    for (it1 = source.begin(), it2 = target.begin(); it1 != source.end() && it2 != target.end(); it1++, it2++) {
        temp = distance(it1->x, it1->y, it1->z, it2->x, it2->y, it2->z);
        //if (temp < 10)
        {
            temp = temp*it1->w;
            dist += temp;
        }
        //temp = abs(it1->z - it2->z)*it1->w;
        //count++;
        //cout << "# " << count << " temp distance: " << temp << endl;
        //cout << "weight: " << it1->w << endl;
    }
//    cout << "Total Distance: " << dist << endl;
    return dist;
}

double calculate_depth(vector<Point3D> source, vector<Point3D> target)
{
    vector<Point3D>::iterator it1, it2;
    double dist = 0, temp = 0;
    for (it1 = source.begin(), it2 = target.begin(); it1 != source.end() && it2 != target.end(); it1++, it2++) {
        temp = abs(it1->z - it2->z);
        //cout << "# " << count << " temp distance: " << temp << endl;
        //cout << "weight: " << it1->w << endl;
        dist += temp;
    }
    cout << "Total Distance: " << dist << endl;
    return dist;
}

double distance(double x1, double y1, double z1, double x2, double y2, double z2)
{
    double dist;
    dist = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2);
    dist = sqrt(dist);
    return dist;
}

double distance_2D(double x1, double y1, double x2, double y2)
{
    double dist;
    dist = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
    dist = sqrt(dist);
    return dist;
}

void Weight(vector<Point3D> &p, vector<Point3D> &mark)
{
    vector<Point3D>::size_type i = 2;
    double left_eye[3], right_eye[3], nose[3], left_cheek[3], right_cheek[3], jaw[3];
    double eye_radius = 5, nose_radius = 5, check_radius = 4, jaw_radius = 2;
    vector<Point3D>::iterator w1, w2;
    int count = 0;
    for (w1 = mark.begin(); w1 != mark.end(); w1++)
    {
        count++;
        switch (count)
        {
            case 1:
                left_eye[0] = w1->x;
                left_eye[1] = w1->y;
                left_eye[2] = w1->z;
                break;
            case 2:
                right_eye[0] = w1->x;
                right_eye[1] = w1->y;
                right_eye[2] = w1->z;
                break;
            case 3:
                nose[0] = w1->x;
                nose[1] = w1->y;
                nose[2] = w1->z;
                break;
            case 4:
                left_cheek[0] = w1->x;
                left_cheek[1] = w1->y;
                left_cheek[2] = w1->z;
                break;
            case 5:
                right_cheek[0] = w1->x;
                right_cheek[1] = w1->y;
                right_cheek[2] = w1->z;
                break;
            case 6:
                jaw[0] = w1->x;
                jaw[1] = w1->y;
                jaw[2] = w1->z;
                break;
            default:
                break;
        }
    }
    
    double origin_x;
    double origin_y;
    
    for (w2 = p.begin(); w2 != p.end(); w2++)
    {
        origin_x = w2->x + mark[i].x - 25;
        origin_y = w2->y + mark[i].y - 30;
        
        if (distance_2D(origin_x, origin_y, left_eye[0], left_eye[1]) < eye_radius)
            w2->w += 3;
        if (distance_2D(origin_x, origin_y, right_eye[0], right_eye[1]) < eye_radius)
            w2->w += 3;
        if (distance_2D(origin_x, origin_y, nose[0], nose[1]-10) < nose_radius)
            w2->w += 2;
        if (distance_2D(origin_x, origin_y, left_cheek[0], left_cheek[1]) < check_radius)
            w2->w += 1;
        if (distance_2D(origin_x, origin_y, right_cheek[0], right_cheek[1]) < check_radius)
            w2->w += 1;
        if (distance_2D(origin_x, origin_y, jaw[0], jaw[1]) < jaw_radius)
            w2->w += 0.5;
        
        //if ((origin_x >= left_eye[0]) && (origin_x <= right_eye[0]) && (origin_y >= left_eye[1]-5) && (origin_y <= left_eye[1]+5))
        //	w2->w += 3;
        //if ((origin_x >= nose[0] - 5) && (origin_x >= nose[0] + 5) && (origin_y <= nose[1]) && (origin_y >= right_eye[1]))
        //	w2->w += 2;
        //if ((origin_x >= left_cheek[0]) && (origin_x <= right_cheek[0]) && (origin_y <= right_cheek[1] + 5) && (origin_y >= right_cheek[1] - 5))
        //	w2->w += 1;
        //if (distance_2D(origin_x, origin_y, jaw[0], jaw[1]) <= jaw_radius)
        //	w2->w += 0.5;
    }
    
    //for (w1 = p.begin(); count<6; w1++) {
    //	if (count == 1)
    //	{
    //		right_eye[0] = w1->x;
    //		right_eye[1] = w1->y;
    //		right_eye[2] = w1->z;
    //	}
    //	else if (count == 2)
    //	{
    //		left_eye_center[0] = w1->x;
    //		left_eye_center[1] = w1->y;
    //		left_eye_center[2] = w1->z;
    //	}
    //	else if (count == 3)
    //	{
    //		nose_center[0] = w1->x;
    //		nose_center[1] = w1->y;
    //		nose_center[2] = w1->z;
    //	}
    //	else if (count == 4)
    //	{
    //		right_cheek_center[0] = w1->x;
    //		right_cheek_center[1] = w1->y;
    //		right_cheek_center[2] = w1->z;
    //	}
    //	else if (count == 5)
    //	{
    //		left_cheek_center[0] = w1->x;
    //		left_cheek_center[1] = w1->y;
    //		left_cheek_center[2] = w1->z;
    //	}
    //	count++;
    //}
    
    //vector<Point3D>::iterator it1;
    //for (it1 = p.begin(); it1 != p.end(); it1++) 
    //{
    //	if (distance(it1->x, it1->y, it1->z, right_eye_center[0], right_eye_center[1], right_eye_center[2])<eye_radius)
    //		it1->w += 3;
    //	else if (distance(it1->x, it1->y, it1->z, left_eye_center[0], left_eye_center[1], left_eye_center[2])<eye_radius)
    //		it1->w += 3;
    //	else if (distance(it1->x, it1->y, it1->z, nose_center[0], nose_center[1], nose_center[2])<nose_radius)
    //		it1->w += 2;
    //	else if (distance(it1->x, it1->y, it1->z, left_cheek_center[0], left_cheek_center[1], left_cheek_center[2])<check_radius)
    //		it1->w += 1;
    //	else if (distance(it1->x, it1->y, it1->z, right_cheek_center[0], right_cheek_center[1], right_cheek_center[2])<check_radius)
    //		it1->w += 1;
    //}
}
