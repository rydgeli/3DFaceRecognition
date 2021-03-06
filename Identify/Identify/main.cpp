//
//  main.cpp
//  Identify
//
//  Created by Rydge on 15/3/30.
//  Copyright (c) 2015年 SHU. All rights reserved.
//

#include "calculate.h"
#include <time.h>
double total_time=0;
string ppl_no[21]={"001","002","003","004","005","006","007","008","009","010",
    "011","012","013","014","015","016","017","018","019","020","021"};
string ppl_set[5]={"s1","s2","s3","s4","s5"};

string ppl_no_e[20]={"0001","0002","0003","0004","0005","0006","0007","0008","0009","0010",
    "0011","0012","0013","0014","0015","0016","0017","0018","0019","0020"};

int random_1;
int random_2;

void getFiles_test()
{
    DIR* dir;
    dirent* pdir;
    
    dir = opendir("/Users/rydge/desktop/Current/BS/eurecom3/seg");
    int count=0;
    while ((pdir = readdir(dir))) {
        if(count>=2)
        {
            if(strcmp(pdir->d_name, ".DS_Store"))
               cout << pdir->d_name << endl;
        }
        count++;
    }
    
    closedir(dir);
}


void identify_x2()
{
    string probe="009_s1";
    string canonical_1 ="002_s1";
    string canonical_2="003_s1";
    
    string rgbPath = "/Users/rydge/desktop/Current/BS/eurecom/rgb/";
    string rgbProbe = rgbPath + "/" + probe + ".bmp";
    string path_1 = "/Users/rydge/desktop/Current/BS/eurecom/aligned/" + canonical_1 + "/";
    string path_2 = "/Users/rydge/desktop/Current/BS/eurecom/aligned/" + canonical_2 + "/";
    string markPath = "/Users/rydge/desktop/Current/BS/eurecom/mark/";
    string depthResult;
    string rgbResult;
    
    vector<string> depthFiles_1,depthFiles_2, markFiles,rgbFiles;
    double distance_tmp, distance_min = 1000000;
    
    getFiles(path_1, depthFiles_1);
    getFiles(path_2, depthFiles_2);
    getFiles(markPath, markFiles);
    getFiles(rgbPath, rgbFiles);
    
    size_t size = depthFiles_1.size();
    
    vector<Point3D> target_1;
    string targetPath_1 = path_1 + "/" + probe + "_" + canonical_1 + ".txt";
    readPoint(targetPath_1.c_str(), target_1);
    
    vector<Point3D> target_2;
    string targetPath_2 = path_2 + "/" + probe + "_" + canonical_2 + ".txt";
    readPoint(targetPath_2.c_str(), target_2);
    
    showRGB(rgbProbe,"Probe Face");
    
    for (int i = 0; i < size; i++)
    {
        vector<Point3D> source_1, source_2, mark;
        
        readPoint((path_1+depthFiles_1[i]).c_str(), source_1);
        readPoint((path_2+depthFiles_2[i]).c_str(), source_2);
        
        readMarkPoint((markPath+markFiles[i]).c_str(), mark);
        
        cout << "源文件1:      "<< depthFiles_1[i].c_str() << endl;
        cout << "源文件2:      "<< depthFiles_2[i].c_str() << endl;
        cout << "特征点:       " << markFiles[i].c_str() << endl;
        
        distance_tmp = calculate(source_1, target_1, mark) + calculate(source_2, target_2, mark);
        cout << "欧氏距离:      " << distance_tmp << endl;
        
        if (distance_tmp < distance_min && distance_tmp > 0)
        {
            distance_min = distance_tmp;
            depthResult = depthFiles_1[i]+" & "+depthFiles_2[i];
            rgbResult = rgbFiles[i];
        }
        showRGB_temp(rgbPath+rgbFiles[i],"Searching...");
        
        //cout << files[i].c_str() << endl;
    }
    cout << "识别文件为:     " << depthResult.c_str() << endl;
    cout << "最近距离为:     " << distance_min << endl;
    cout << "识别脸RGB为:    " << rgbResult.c_str() << endl;
    showRGB(rgbPath+rgbResult, "Result Face");

}

void identify()
{
//    string probe = "0005_s1";
    string canonical = "0001_s1";
    random_1=rand() % 20;
    random_2=rand() % 2;
    string probe = ppl_no_e[random_1] + "_" + ppl_set[random_2];
    string rgbPath = "/Users/rydge/desktop/Current/BS/eurecom1/rgb/";
    string rgbProbe = rgbPath + "/" + probe + ".bmp";
    string path = "/Users/rydge/desktop/Current/BS/eurecom1/aligned/" + canonical + "/";
    string markPath = "/Users/rydge/desktop/Current/BS/eurecom1/mark/";
    string depthResult;
    string rgbResult;
    
    vector<string> depthFiles,markFiles,rgbFiles;
    double distance_tmp, distance_min = 1000000;
    
    getFiles(path, depthFiles);
    getFiles(markPath, markFiles);
    getFiles(rgbPath, rgbFiles);
    
    size_t size = depthFiles.size();
    
    vector<Point3D> target;
    string targetPath = path + probe + " - " + canonical + ".txt";
    readPoint(targetPath.c_str(), target);
    showRGB(rgbProbe,"Probe Face");
    clock_t start,finish;
    start=clock();
    
    for (int i = 0; i < size; i++)
    {
        vector<Point3D> source, mark;
        
        readPoint((path+depthFiles[i]).c_str(), source);
        readMarkPoint((markPath+markFiles[i]).c_str(), mark);
        cout << "源文件:     "<< depthFiles[i].c_str() << endl;
        cout << "特征点文件: " << markFiles[i].c_str() << endl;
        
        distance_tmp = calculate(source, target, mark);
        cout << "欧氏距离:      " << distance_tmp << endl;
        
        if (distance_tmp < distance_min && distance_tmp > 0)
        {
            distance_min = distance_tmp;
            depthResult = depthFiles[i];
            rgbResult = rgbFiles[i];
        }
        showRGB_temp(rgbPath+rgbFiles[i],"Searching...");

        //cout << files[i].c_str() << endl;
    }
    
    finish=clock();
    total_time+=(double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"||-----识别所花时间为:"<<total_time<<"s-----||"<<endl;
    cout << "待检测脸深度文件："<< probe.c_str() << endl << distance_min << endl;
    cout << "识别脸深度文件为："<< depthResult.c_str() << endl << distance_min << endl;
    cout << "识别脸RGB文件为：" << rgbResult.c_str() << endl;
    showRGB(rgbPath+rgbResult, "Result Face");
};

void identify_shu()
{
    
//    string probe = "010_s1";
    random_1=rand() % 21;
    random_2=rand() % 5;
    string canonical = "001_s1";
    string probe = ppl_no[random_1]+"_"+ppl_set[random_2];
    string rgbPath = "/Users/rydge/desktop/Current/BS/shu_face/rgb/";
    string rgbProbe = rgbPath + "/" + probe + ".bmp";
    string path = "/Users/rydge/desktop/Current/BS/shu_face/aligned/" + canonical + "/";
    string depthResult;
    string rgbResult;
    
    vector<string> depthFiles,rgbFiles;
    double distance_tmp, distance_min = 1000000;
    
    getFiles(path, depthFiles);
    getFiles(rgbPath, rgbFiles);
    
    size_t size = depthFiles.size();
    
    vector<Point3D> target;
    string targetPath = path + probe + " - " + canonical + ".txt";
    shuReadPoints(targetPath.c_str(), target);
    showRGB(rgbProbe,"Probe Face");
    
    clock_t start,finish;
    start=clock();
    for (int i = 0; i < size; i++)
    {
        vector<Point3D> source, mark;
        
        shuReadPoints((path+depthFiles[i]).c_str(), source);
        cout << "源文件:     "<< depthFiles[i].c_str() << endl;
        
        distance_tmp = calculate(source, target);
        cout << "欧氏距离:      " << distance_tmp << endl;
        
        if (distance_tmp < distance_min && distance_tmp > 0)
        {
            distance_min = distance_tmp;
            depthResult = depthFiles[i];
            rgbResult = rgbFiles[i];
        }
        showRGB_temp(rgbPath+rgbFiles[i],"Searching...");
        
    }
    finish=clock();
    total_time+=(double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"||-----识别所花时间为:"<<total_time<<"s-----||"<<endl;
    cout << "识别脸深度文件为："<< depthResult.c_str() << endl << distance_min << endl;
    cout << "识别脸RGB文件为：" << rgbResult.c_str() << endl;
    showRGB(rgbPath+rgbResult, "Result Face");
};

int main(int argc, const char * argv[]) {
    
    srand((int)time(NULL));
    
//    identify_shu();
    identify();
    
//    identify_x2();
    
    return 0;
}
