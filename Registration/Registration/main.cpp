//
//  main.cpp
//  Registration
//
//  Created by Rydge on 5/4/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#include "regist.h"

// For Windows Only
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


void registration(const char *source_path, const char *target_path, const char *out_path)
{
    vector<Point3D> source, target;
    double e = 10; //0.001;
    double R[9], T[3];
    ReadPoint(source_path, source);
    ReadPoint(target_path, target);
    ICP(source, target, out_path, R, T, e);
}


void perform(string canonical)
{
    string canonical_path = "/Users/rydge/desktop/Current/BS/eurecom3/seg/" + canonical + ".txt";
    string filePath = "/Users/rydge/desktop/Current/BS/eurecom3/seg/";
    string alignedPath = "/Users/rydge/desktop/Current/BS/eurecom3/aligned/" + canonical + "/";
    string path = "";
    vector<string> files;
    
    getFiles(filePath, files);
    
    size_t size = files.size();
    
    for (int i = 0; i < size; i++)
    {
        path = alignedPath + files[i].substr(0,6) + "_" + canonical + ".txt";
        cout << path.c_str() << endl;
        registration((filePath + files[i]).c_str(), canonical_path.c_str(), path.c_str());
    }
}


int main()
{
    string canonical="001_s1";
    perform(canonical);
    return 0;
}

//void testOnce()
//{
//    registration("D:\\BS\\eurecom\\100\\s1\\001_s1.txt", "D:\\BS\\eurecom\\100\\s1\\001_s1.txt", "C:\\Users\\Rydge\\Desktop\\result.txt");
//}


//void testOld()
//{
//    vector<Point3D> source, target;
//    char *path = nullptr;
//    double e = 0.001; //0.001;
//    for (int i = 2; i < 5; i++)
//    {
//        double  R[9], T[3];
//        ReadPoint3D("C:\\Users\\Rydge\\Desktop\\face\\source\\001\\1.txt",target);
//        switch (i)
//        {
//            case 2:
//                ReadPoint3D("C:\\Users\\Rydge\\Desktop\\face\\source\\002\\1.txt", source);
//                path = "C:\\Users\\Rydge\\Desktop\\2.1_1.1.txt";
//                break;
//            case 3:
//                ReadPoint3D("C:\\Users\\Rydge\\Desktop\\face\\source\\003\\1.txt", source);
//                path = "C:\\Users\\Rydge\\Desktop\\3.1_1.1.txt";
//                break;
//            case 4:
//                ReadPoint3D("C:\\Users\\Rydge\\Desktop\\face\\source\\004\\1.txt", source);
//                path = "C:\\Users\\Rydge\\Desktop\\4.1_1.1.txt";
//                break;
//            default:
//                break;
//        }
//        
//        ICP(source, target, path, R, T, e);
//    }
//}

