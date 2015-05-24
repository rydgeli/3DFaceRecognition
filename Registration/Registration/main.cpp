//
//  main.cpp
//  Registration
//
//  Created by Rydge on 5/4/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#include "regist.h"

double total_time=0;

void registration(const char *source_path, const char *target_path, const char *out_path)
{
    vector<Point3D> source, target;
    double e = 10; //0.001;
    double R[9], T[3];
    ReadPoint(source_path, source);
    ReadPoint(target_path, target);
    clock_t start,finish;
    double duration;
    start=clock();
    ICP(source, target, out_path, R, T, e);
    finish=clock();
    duration=(double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"||-----完成一次配准时间为："<<duration<<"-----||"<<endl;
}

void registration_shu(const char *source_path, const char *target_path, const char *out_path)
{
    vector<Point3D> source,target;
    double e = 0.01;
    double R[9],T[3];
    ReadPoint_shu(source_path, source);
    ReadPoint_shu(target_path, target);
    clock_t start,finish;
    double duration;
    start=clock();
    ICP(source, target, out_path, R, T, e);
    finish=clock();
    duration=(double)(finish-start)/CLOCKS_PER_SEC;
    total_time+=duration;
    cout<<"||-----完成一次配准时间为："<<duration<<"-----||"<<endl;

}

void perform(string canonical)
{
    string canonical_path = "/Users/rydge/desktop/Current/BS/eurecom1/seg/depth_" + canonical + "_LightOn.txt";
    string segPath = "/Users/rydge/desktop/Current/BS/eurecom1/seg/";
    string alignedPath = "/Users/rydge/desktop/Current/BS/eurecom1/aligned/" + canonical + "/";
    string path = "";
    vector<string> segFiles;
    
    getFiles(segPath, segFiles);
    
    size_t size = segFiles.size();
    
    for (int i = 0; i < size; i++)
    {
        cout<<segFiles[i]<<endl;
        path = alignedPath + segFiles[i].substr(6,7) + " - " + canonical + ".txt";
        cout << path.c_str() << endl;
        registration((segPath + segFiles[i]).c_str(), canonical_path.c_str(), path.c_str());
    }
}

void perform_shu(string canonical)
{
    string cannonical_path = "/Users/rydge/desktop/Current/BS/shu_face/depth_source/" + canonical + ".txt";
    string depthPath="/Users/rydge/desktop/Current/BS/shu_face/depth_source/";
    string alignedPath="/Users/rydge/desktop/Current/BS/shu_face/aligned/"+ canonical + "/";
    string path = "";
    vector<string> depthFiles;
    getFiles(depthPath, depthFiles);
    
    size_t size = depthFiles.size();
    
    for (int i=0;i<size;i++)
    {
        path = alignedPath + depthFiles[i].substr(0,6)+ " - " + canonical + ".txt";
        cout<<path.c_str()<<endl;
        registration_shu((depthPath + depthFiles[i]).c_str(), cannonical_path.c_str(), path.c_str());
    }
    
}

int main()
{
    string canonical="005_s1";
    perform_shu(canonical);
//    perform(canonical);
    cout<<"||-----总时间为"<<total_time<<"-----||"<<endl;
    return 0;
}

