//
//  header.cpp
//  Pre_Process
//
//  Created by Rydge on 5/11/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#include "header.h"

//** Get files from given directory **
void getFiles(string path, vector<string> &files)
{
    DIR* dir;
    dirent* pdir;
    
    dir = opendir(path.c_str());
    int count=0;
    while ((pdir = readdir(dir))) {
        if (count>=2)
        {
            if(strcmp(pdir->d_name, ".DS_Store"))
                files.push_back(pdir->d_name);
        }
        count++;
    }
    
    closedir(dir);
}

// Read face points
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
        for (int i = 0; i< MaxPoints; i++)
        {
            Point3D p;
            fscanf(fp, "%d%d%d", &p.x, &p.y, &p.z);
            P.push_back(p);
        }
    }
    fclose(fp);
}

// read marked face points (6)
void readMarkPoint(const char*filename, vector<Point3D> &P)
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
        for (int i = 0; i< 6; i++)
        {
            Point3D p;
            fscanf(fp, "%d%d%d", &p.x, &p.y, &p.z);
            P.push_back(p);
        }
    }
    fclose(fp);
}

//read face points after segmentation
void readSegPoint(const char*filename, vector<Point3D> &P)
{
    FILE *fp;
    fp=fopen(filename, "r");
    
    if (fp == NULL)
    {
        printf("Open Error!!!");
        exit(1);
    }
    else
    {
        for (int i = 0; i< SegMaxPoints; i++)
        {
            Point3D p;
            fscanf(fp, "%d%d%d", &p.x, &p.y, &p.z);
            P.push_back(p);
        }
    }
    fclose(fp);
}

//read points from given file (too slow!!! Abandoned)
void readPointFiles(const char *filename, vector<Point3D> &P)
{
    fstream file;
    file.open(filename, ios::in);
    while (file.peek()!=EOF)
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


//***** For Windows Only *****

//void getFiles(string path, vector<string>& files)
//{
//    //文件句柄
//    long   hFile = 0;
//    //文件信息
//    struct _finddata_t fileinfo;
//    string p;
//    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
//    {
//        do
//        {
//            //如果是目录,迭代之
//            //如果不是,加入列表
//            if ((fileinfo.attrib &  _A_SUBDIR))
//            {
//                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
//                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
//            }
//            else
//            {
//                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
//            }
//        } while (_findnext(hFile, &fileinfo) == 0);
//        _findclose(hFile);
//    }
//}
