//
//  main.cpp
//  Pre_Process
//
//  Created by Rydge on 5/4/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#include "header.h"

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

void getFiles(string path, vector<string> &files)
{
    DIR* dir;
    dirent* pdir;
    
    dir = opendir(path.c_str());
    int count=0;
    while ((pdir = readdir(dir))) {
        if (count>=2)
        {
            files.push_back(pdir->d_name);
        }
        count++;
    }
    
    closedir(dir);
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
        for (int i = 0; i< MaxPoints; i++)
        {
            Point3D p;
            fscanf(fp, "%d%d%d", &p.x, &p.y, &p.z);
            P.push_back(p);
        }
    }
    fclose(fp);
}

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

void readPointFiles(const char *filename, vector<Point3D> &P)// too slow!!!
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

void detectation(vector <Point3D> &source)
{
    
    vector<Point3D>::iterator it;
    int depth_high=0, depth_1200=0, depth_1100 = 0, depth_1000 = 0, depth_900 = 0, depth_800 = 0, depth_700 = 0, depth_600 = 0;
    int depth_500 = 0, depth_low = 0;
    for (it = source.begin(); it != source.end(); it++)
    {
        if (it->z >= 1300)
            depth_high++;
        else
        {
            if (it->z >= 1200)
                depth_1200++;
            else
            {
                if (it->z >= 1100)
                    depth_1100++;
                else
                {
                    if (it->z >= 1000)
                        depth_1000++;
                    else
                    {
                        if (it->z >= 900)
                            depth_900++;
                        else
                        {
                            if (it->z >= 800)
                                depth_800++;
                            else
                            {
                                if (it->z >= 700)
                                    depth_700++;
                                else
                                {
                                    if (it->z >= 600)
                                        depth_600++;
                                    else
                                    {
                                        if (it->z >= 500)
                                            depth_500++;
                                        else
                                        {
                                            depth_low++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
        }
    }
    
    cout << "大于1200的有" << depth_high<<endl;
    cout << "1200的有" << depth_1200 << endl;
    cout << "1100的有" << depth_1100<<endl;
    cout << "1000的有" << depth_1000 << endl;
    cout << "900的有" << depth_900 << endl;
    cout << "800的有" << depth_800 << endl;
    cout << "700的有" << depth_700 << endl;
    cout << "600的有" << depth_600 << endl;
    cout << "500的有" << depth_500 << endl;
    cout << "小于500的有" << depth_low << endl;
}

void prepare_detection()
{
    vector<Point3D> source;
    readPoint("D:\\BS\\eurecom\\source\\s1\\depth_0001_s1_LightOn.txt", source);
    detectation(source);
}

void process(const char *filename, vector<Point3D> &source, vector<Point3D> &mark)
{
    FILE *fp;
    fp = fopen(filename, "w");
    vector<Point3D>::iterator it1;
    vector<Point3D>::size_type i = 2;
//    int j = 0;
    int corner_x = mark[i].x - 25;
    int corner_y = mark[i].y - 30;
    //cout << "鼻子的深度值为" << mark[i].z << endl;
    for (it1 = source.begin(); it1 != source.end(); it1++)
    {
        //vector<Point3D>::iterator it2 = mark.begin();
        if (abs(it1->x - mark[i].x) < 25 && (it1->y > mark[i].y-30) && (it1->y < mark[i].y+50))
        {
            fprintf(fp, "%d %d %d\n", it1->x - corner_x, it1->y - corner_y, it1->z);
            //j = 0;
        }
    }
    
    fclose(fp);
    
    //FILE *fp;
    //fopen_s(&fp, filename, "w");
    //int i = 1, j;
    //vector<Point3D>::iterator it1,it2;
    //double temp_z;
    //for (it1 = mark.begin(); it1 != mark.end(); it1++,i++)
    //{
    //	j = 0;
    //	for (it2 = source.begin(); it2 != source.end(); it2++)
    //	{
    //		if (abs(it1->x - it2->x) <= 3 && abs(it1->y - it2->y) <= 3)
    //		{
    //			j++;
    //			temp_z = abs(it2->z);
    //			//if (it1->x == it2->x && it1->y == it2->y)
    //			//	temp_z = -temp_z;
    //			fprintf(fp, "%d %d %lf\n", it2->x, it2->y, temp_z);
    //		}
    //	}
    //	//if (it1->x >= 270 && it1->x <= 369 && it1->y >= 160 && it1->y <= 299 && (it1->x-270) % 5==0 && (it1->y-160) % 5==0)
    //	//{
    //	//		fprintf(fp, "%d %d %lf\n", it1->x, it1->y, it1->z*0.001);
    //	//}
    //}
    //
    //fclose(fp);
    
}

void prepare_process()
{
    string sourcePath = "/Users/rydge/desktop/Current/BS/eurecom3/source/";
    string markPath = "/Users/rydge/desktop/Current/BS/eurecom3/mark/";
    vector<string> sourceFiles, markFiles;

    string base_path = "/Users/rydge/desktop/Current/BS/eurecom3/face_data/";
    string result_path;
    
    //获取路径下的所有文件
    getFiles(sourcePath, sourceFiles);
    getFiles(markPath, markFiles);
    
    size_t size = sourceFiles.size();
    
    for (int i = 0; i < size; i++)
    {
        
//        ostringstream oss;
//        oss << (i+1);
//        c = oss.str();
        
        cout << (sourcePath+sourceFiles[i]).c_str() << endl;
        cout<< (markPath+markFiles[i]).c_str()<<endl;
        
        vector<Point3D> source;
        vector<Point3D> mark;
        readPoint((sourcePath+sourceFiles[i]).c_str(), source);
        readMarkPoint((markPath+markFiles[i]).c_str(), mark);
        
        result_path = base_path + sourceFiles[i];
        
        process(result_path.c_str(), source, mark);
        cout << result_path.c_str() << endl;
    }
}

void segmentation(const char *filename, vector<Point3D> &source, vector<Point3D> &mark)
{
    FILE *fp;
    fp = fopen(filename, "w");
    vector<Point3D>::iterator it1;
    vector<Point3D>::size_type i = 2;
    int last_z = mark[i].z;
    cout << "鼻子的深度值为" << mark[i].z << endl;
    for (it1 = source.begin(); it1 != source.end(); it1++)
    {
        if (abs(it1->z - mark[i].z) <= 50)
        {
            fprintf(fp, "%d %d %d\n", it1->x, it1->y, it1->z);
            last_z = it1->z;
        }
        else
        {
            fprintf(fp, "%d %d %d\n", it1->x, it1->y, last_z);
        }
    }
    
    fclose(fp);
}

void prepare_segmentation()
{
    string sourcePath = "/Users/rydge/desktop/Current/BS/eurecom3/face_data/";
    string markPath = "/Users/rydge/desktop/Current/BS/eurecom3/mark/";
    vector<string> sourceFiles, markFiles;
    string base_path = "/Users/rydge/desktop/Current/BS/eurecom3/seg/";
    string result_path;
    
    //获取路径下的所有文件
    getFiles(sourcePath, sourceFiles);
    getFiles(markPath, markFiles);
    
    size_t size = sourceFiles.size();
    
    for (int i = 0; i < size; i++)
    {
//        ostringstream oss;
//        oss << (i+1);
//        c = oss.str();
        
        vector<Point3D> source;
        vector<Point3D> mark;
        readSegPoint((sourcePath+sourceFiles[i]).c_str(), source);
        readMarkPoint((markPath+markFiles[i]).c_str(), mark);
        
        result_path = base_path +sourceFiles[i];
        
        segmentation(result_path.c_str(), source, mark);
    }
}

int main()
{
    
    //prepare_detection();
//    prepare_process();
    prepare_segmentation();
    
    //read Test
    //vector<Point3D> source;
    //readFiles("D:\\BS\\eurecom\\mark\\s2\\depth_0001_s2_LightOn_Points_TXT.txt", source);
    //vector<Point3D>::iterator it1;
    //for (it1 = source.begin(); it1 != source.end(); it1++)
    //	cout << it1->x << " " << it1->y << " " << it1->z<<endl;
}