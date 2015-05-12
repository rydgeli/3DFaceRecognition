//
//  main.cpp
//  Registration
//
//  Created by Rydge on 5/4/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#include "regist.h"

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


int main()
{
    string canonical="0002_s1";
    perform(canonical);
    return 0;
}

