#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include "TROOT.h"
#include "TString.h"
#include "TVector3.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "CompTreeCoordinateConverter.hh"

using namespace std;

void DescribeInputParameters();
int GetCameraNum(string cameraname);

int main(int argc, char** argv){
    if(argc < 4){
        DescribeInputParameters();
        return -1;
    }

    TString infilename = argv[1];
    string parfilename = argv[2];
    string cameraname = argv[3];

    CompTreeCoordinateConverter *converter = new CompTreeCoordinateConverter();

    converter->SetPrintDivNum(10000);
    
    converter->ReadInputfile(infilename);

    TVector3 parallel_translation_vector;
    double rotate_angle;
    boost::property_tree::ptree par_tree;
    boost::property_tree::read_json(parfilename, par_tree);
    
    switch(GetCameraNum(cameraname)){
        case 1:
            parallel_translation_vector.SetXYZ(
                par_tree.get<double>("CC1.parallel_translation_x"),
                par_tree.get<double>("CC1.parallel_translation_y"),
                par_tree.get<double>("CC1.parallel_translation_z")
            );
            rotate_angle = TMath::Pi()/180*par_tree.get<double>("CC1.rotate_angle_in_deg");
            break;
        case 2:
            parallel_translation_vector.SetXYZ(
                par_tree.get<double>("CC2.parallel_translation_x"),
                par_tree.get<double>("CC2.parallel_translation_y"),
                par_tree.get<double>("CC2.parallel_translation_z")
            );
            rotate_angle = TMath::Pi()/180*par_tree.get<double>("CC2.rotate_angle_in_deg");
            break;
        default:
            DescribeInputParameters();
            delete converter;
            return -1;
            break;
    }

    converter->SetParallelTranslationVector(parallel_translation_vector);
    converter->SetRotateAngle(rotate_angle);

    converter->Initialize();
    converter->Run();

    delete converter;
}

void DescribeInputParameters(){
    cout << "1 : rootfilename (including cetree)" << endl;
    cout << "2 : parameter file (.json)" << endl;
    cout << "3 : cameraname(ex. 1 or cam1 or cc1 or camera1)" << endl;
}

int GetCameraNum(string cameraname){
    if(!strcmp( cameraname.c_str() , "1") || !strcmp( cameraname.c_str() , "cam1") || !strcmp( cameraname.c_str() , "cc1") || !strcmp( cameraname.c_str() , "camera1")){
        return 1;
    }else if(!strcmp( cameraname.c_str() , "2") || !strcmp( cameraname.c_str() , "cam2") || !strcmp( cameraname.c_str() , "cc2") || !strcmp( cameraname.c_str() , "camera2")){
        return 2;
    }
    return 0;
}
