#ifndef In_3D_SingleCam_ComptonImager_HH 
#define In_3D_SingleCam_ComptonImager_HH 

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <map>
#include <string.h>
#include <vector>
#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TRandom.h"
#include "TGraph.h"
#include "TVector3.h"
#include "TF1.h"
#include "TSpline.h"
#include "TMath.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "CompTreeReader.hh"
#include "TFileMaker.hh"
#include "AnalysisParameter.hh"
#include "In111Constant.hh"

using namespace std;

class In_3D_SingleCam_ComptonImager : public TFileMaker{
    private:
        Long64_t total_event_num;

        Long64_t current_event_num; 

        Long64_t print_div_num; 

        double world_x, world_y, world_z;
        double world_lx, world_ly, world_lz;
        int n_x, n_y, n_z;

        double c_x, c_y, c_z;

        double slice_x, slice_y, slice_z;
        int comp_n_x, comp_n_y, comp_n_z;

        CompTreeReader *ct;

        TH3F *image_3d;
        TH2F *projectionXY_image_3d;
        TH2F *projectionZX_image_3d;
        TH2F *projectionZY_image_3d;

        TH2F *compimage;
        TH1D *arm_all;
        TH1D *arm_171keV;
        TH1D *arm_245keV;

        TH2F *compimage_ZX;
        TH2F *compimage_ZY;

        TH1D *ehist;

        double e;
        TVector3 pos;
        double costheta;
        double dtheta;
        TVector3 vecG;

        void PrintNow();
        void SetDefault();
        bool ImageStartUp();
        bool GetNextEvent();

        bool ThreeDImageFilter();
        bool CompImageFilter();
        int check_line(double e);

        void SetCompImageCalPar(int n_cam, double &costheta, double &dtheta, TVector3 &axis, TVector3 &inipos, double &e);
        double CalCompWeight(TVector3 comp_point, TVector3 start_point, TVector3 axis, double costheta, double dtheta, double e);

        void GetExpValue();
        void FillEnergy();
        void CalARM();
        void MakeImage();
        void Make3DImage();
        void MakeCompImage();
        void MakeCompImageXY();
        void MakeCompImageZX();
        void MakeCompImageZY();

        void MakeCompImageEachCameraXY(int n_cam, TH2F *compimage);
        void MakeCompImageEachCameraZX(int n_cam, TH2F *compimage);
        void MakeCompImageEachCameraZY(int n_cam, TH2F *compimage);

        double CalTotalWeight();

        bool boolMake3DImage, boolMakeCompImage;

        double gauss_FWHM_171keV;
        double gauss_FWHM_245keV;

        double totalweight;
        
        bool boolCalARM;
        double source_x, source_y, source_z;

        bool boolNormalization;
        int norm_n_x, norm_n_y, norm_n_z;
        double rescale_factor;
        double totalweight_cut_min;
        double totalweight_cut_max;

        TH1D *hist_total_weight;
        TH1D *hist_total_weight_aftercut;

    protected:
        void WarnTESTRUNmode();

    public:
        ~In_3D_SingleCam_ComptonImager();
        In_3D_SingleCam_ComptonImager();
        In_3D_SingleCam_ComptonImager(TString infilename);
        In_3D_SingleCam_ComptonImager(TString infilename, TString outfilename);

        void LoadParameterFile(string parfilename);

        void Initialize();
        void Run();
        void SaveImage();
        void PrintPar();

        void SetWorldPosition(double world_x, double world_y, double world_z);
        void SetWorldSize(double world_lx, double world_ly, double world_lz);

        void SetCamPosition(double c_x, double c_y, double c_z);
        void SetSliceX(double slice_x);
        void SetSliceY(double slice_y);
        void SetSliceZ(double slice_z);

        void Set3DImageDivision(int n_x, int n_y, int n_z);
        void SetCompImageDivision(int comp_n_x, int comp_n_y, int comp_n_z);
        void SetGaussFWHM(double gauss_FWHM_171keV, double gauss_FWHM_245keV);
        void SetCalARM(bool boolCalARM, double source_x, double source_y, double source_z);
        void SetNormPar( bool boolNormalization, int norm_n_x, int norm_n_y, int norm_n_z, double rescale_factor, double totalweight_cut_min, double totalweight_cut_max);

        void SetMakeImageList(bool boolMake3DImage, bool boolMakeCompImage);

        void SetPrintDivNum(Long64_t print_div_num);
};

#endif
