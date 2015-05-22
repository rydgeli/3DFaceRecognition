//
//  main.cpp
//  ICP_in_VTK
//
//  Created by Rydge on 1/21/15.
//  Copyright (c) 2015 SHU. All rights reserved.
//

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkLandmarkTransform.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkProperty.h>
#include <vtkPLYReader.h>
#include <sstream>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    vtkSmartPointer<vtkPolyData> sourceTmp = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> targetTmp = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> alignedTmp = vtkSmartPointer<vtkPolyData>::New();
    
    vtkSmartPointer<vtkPolyData> source = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> aligned = vtkSmartPointer<vtkPolyData>::New();
    
    //自建Kinect三维人脸库
//    string strSource = "/Users/rydge/desktop/face/source/001_s1.txt";
//    string strTarget = "/Users/rydge/desktop/face/source/002_s1.txt";
    
    
    string probe = "0002_s1";
    string canonical = "0001_s1";
    // Eurecom 欧盟电信Kinect三维人脸库
    string strSource="/Users/rydge/desktop/Current/BS/eurecom1/seg/depth_" + probe + "_LightOn.txt";
    string strTarget="/Users/rydge/desktop/Current/BS/eurecom1/seg/depth_" + canonical+ "_LightOn.txt";
    
    string strAligned = "/Users/rydge/desktop/Current/BS/eurecom1/aligned/" + canonical +"/" + probe + " - " + canonical + ".txt";
    
    // SHU_face自建人脸库
//    string strSource = "/Users/rydge/desktop/Current/BS/shu_face/depth_source/" + probe + ".txt";
//    string strTarget = "/Users/rydge/desktop/Current/BS/shu_face/depth_source/" +canonical +".txt";
//    string strAligned = "/Users/rydge/desktop/Current/BS/shu_face/aligned/" + canonical +"/" + probe + " - " + canonical + ".txt";
    
    ifstream fSource(strSource.c_str());
    ifstream fTarget(strTarget.c_str());
    ifstream fAligned(strAligned.c_str());
        
    string line;
    vtkSmartPointer<vtkPoints> sourcePoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> targetPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> alignedPoints = vtkSmartPointer<vtkPoints>::New();

        
        while(getline(fSource, line))
        {
            double x,y,z;
            stringstream linestream;
            linestream << line;
            linestream >> x >> y >> z;
            sourcePoints->InsertNextPoint(x, y, z);
        }
        sourceTmp->SetPoints(sourcePoints);
        vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter1 = vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
        vertexFilter1->SetInputConnection(sourceTmp->GetProducerPort());
#else
        vertexFilter1->SetInputData(sourceTmp);
#endif
        vertexFilter1->Update();
        source->ShallowCopy(vertexFilter1->GetOutput());
  ////////////////////////////
        while(getline(fTarget, line))
        {
            double x,y,z;
            stringstream linestream;
            linestream << line;
            linestream >> x >> y >> z;
            targetPoints->InsertNextPoint(x, y, z);
        }
        targetTmp->SetPoints(targetPoints);
        vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter2 = vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
        vertexFilter2->SetInputConnection(targetTmp->GetProducerPort());
#else
        vertexFilter2->SetInputData(targetTmp);
#endif
        vertexFilter2->Update();
        target->ShallowCopy(vertexFilter2->GetOutput());
    ///////////////////////
    while(getline(fAligned, line))
    {
        double x,y,z;
        stringstream linestream;
        linestream << line;
        linestream >> x >> y >> z;
        alignedPoints->InsertNextPoint(x, y, z);
    }
    alignedTmp->SetPoints(alignedPoints);
    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter3 = vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
    vertexFilter3->SetInputConnection(alignedTmp->GetProducerPort());
#else
    vertexFilter3->SetInputData(alignedTmp);
#endif
    vertexFilter3->Update();
    aligned->ShallowCopy(vertexFilter3->GetOutput());
    //////////////////////////////////////////////////////////
    
    // Setup ICP transform
    vtkSmartPointer<vtkIterativeClosestPointTransform> icp =
    vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
    icp->SetSource(source);
    icp->SetTarget(target);
    
    icp->GetLandmarkTransform()->SetModeToRigidBody();
    
    icp->SetMaximumNumberOfIterations(30);
    //icp->StartByMatchingCentroidsOn();
    icp->Modified();
    icp->Update();
    // Get the resulting transformation matrix (this matrix takes the source points to the target points)
    vtkSmartPointer<vtkMatrix4x4> M = icp->GetMatrix();
    cout << "The resulting matrix is: " << endl;
    for(int i = 0;i<= 3;i++)
    {
        printf("\n");
        for(int j = 0;j <= 3;j++)
        {
            printf("%e\t",M->Element[i][j]);
        }
    }
    
    // Transform the source points by the ICP solution
    vtkSmartPointer<vtkTransformPolyDataFilter> icpTransformFilter =
    vtkSmartPointer<vtkTransformPolyDataFilter>::New();
#if VTK_MAJOR_VERSION <= 5
    icpTransformFilter->SetInput(source);
#else
    icpTransformFilter->SetInputData(source);
#endif
    icpTransformFilter->SetTransform(icp);
    icpTransformFilter->Update();
    
    //////////////////////////////////////////////////////////
    
    /*
     // If you need to take the target points to the source points, the matrix is:
     icp->Inverse();
     vtkSmartPointer<vtkMatrix4x4> minv = icp->GetMatrix();
     cout << "The resulting inverse matrix is: " << *minv << std::cout;
     */
    
    // Visualize
    
    vtkSmartPointer<vtkPolyDataMapper> sourceMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
    sourceMapper->SetInputConnection(source->GetProducerPort());
#else
    sourceMapper->SetInputData(source);
#endif
    
    vtkSmartPointer<vtkActor> sourceActor =
    vtkSmartPointer<vtkActor>::New();
    sourceActor->SetMapper(sourceMapper);
    sourceActor->GetProperty()->SetColor(1,0,0);
    sourceActor->GetProperty()->SetPointSize(5);
    
///////////////////
    vtkSmartPointer<vtkPolyDataMapper> targetMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
    targetMapper->SetInputConnection(target->GetProducerPort());
#else
    targetMapper->SetInputData(target);
#endif
    
    vtkSmartPointer<vtkActor> targetActor =
    vtkSmartPointer<vtkActor>::New();
    targetActor->SetMapper(targetMapper);
    targetActor->GetProperty()->SetColor(0,1,0);
    targetActor->GetProperty()->SetPointSize(4);
    
    ///////////////////
    vtkSmartPointer<vtkPolyDataMapper> alignedMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
    alignedMapper->SetInputConnection(aligned->GetProducerPort());
#else
    alignedMapper->SetInputData(aligned);
#endif
    
    vtkSmartPointer<vtkActor> alignedActor =
    vtkSmartPointer<vtkActor>::New();
    alignedActor->SetMapper(alignedMapper);
    alignedActor->GetProperty()->SetColor(0,0,1);
    alignedActor->GetProperty()->SetPointSize(3);
    
    ////////////////////
    vtkSmartPointer<vtkPolyDataMapper> solutionMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    solutionMapper->SetInputConnection(icpTransformFilter->GetOutputPort());
    
    vtkSmartPointer<vtkActor> solutionActor =
    vtkSmartPointer<vtkActor>::New();
    solutionActor->SetMapper(solutionMapper);
    solutionActor->GetProperty()->SetColor(0,0,1);
    solutionActor->GetProperty()->SetPointSize(3);
    
    // Create a renderer, render window, and interactor
    vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    
    // Add the actor to the scene
    renderer->AddActor(sourceActor);
    renderer->AddActor(targetActor);
    renderer->AddActor(alignedActor);
//    renderer->AddActor(solutionActor);
    renderer->SetBackground(.3, .6, .3); // Background color green
    
    // Render and interact
    renderWindow->Render();
    renderWindowInteractor->Start();
    
    return EXIT_SUCCESS;
}
