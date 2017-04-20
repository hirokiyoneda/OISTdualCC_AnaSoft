#ifndef In_3D_ComptonImager_HH 
#define In_3D_ComptonImager_HH 

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
#include "CdTeConstant.hh"
#include "PhysicalConstant.hh"

#include "ComptonEventReconstruction.hh"

using namespace std;

class In_3D_ComptonImager : public TFileMaker{
    private:
        Long64_t c1_total_event_num;
        Long64_t c2_total_event_num;

        Long64_t c1_current_event_num; 
        Long64_t c2_current_event_num; 

        Long64_t print_div_num; 

        double world_x, world_y, world_z;
        double world_lx, world_ly, world_lz;
        int n_x, n_y, n_z;

        double c1_x, c1_y, c1_z;
        double c2_x, c2_y, c2_z;

        double slice_x, slice_y, slice_z;
        int comp_n_x, comp_n_y, comp_n_z;
        int pet_n_x, pet_n_y;

        CompTreeReader *ct_c1, *ct_c2;

        TH3F *image_3d;
        TH2F *projectionXY_image_3d;
        TH2F *projectionZX_image_3d;
        TH2F *projectionZY_image_3d;

        TH2F *compimage_c1, *compimage_c2;

        TH2F *compimage_c1_ZX, *compimage_c2_ZX;
        TH2F *compimage_c1_ZY, *compimage_c2_ZY;

        TH2F *compimage_c1c2;
        TH2F *compimage_c1c2_ZX;
        TH2F *compimage_c1c2_ZY;

        TH2F *compimage_c1c2_multiply_non_coinimage;
        TH2F *compimage_c1c2_ZX_multiply_non_coinimage;
        TH2F *compimage_c1c2_ZY_multiply_non_coinimage;

        TH2F *divide_c1c2;

        TH2D *image_pet;

        TH1D *ehist_c1, *ehist_c2;
        TH1D *ehist_sum;
        TH2D *ehist_c1c2;

        double e1, e2;

        TVector3 c1_pos, c2_pos;

        double c1_costheta, c2_costheta;
        double c1_dtheta, c2_dtheta;
        TVector3 c1_vecG, c2_vecG;

        void PrintNow();
        void SetDefault();
        bool ImageStartUp();
        bool GetNextEvent();

        bool ThreeDImageFilter();
        bool CompImageFilter();
        bool PETFilter();
        int CheckLine(double e);
        bool CheckFluo(double e);

        //double CalWeight(TVector3 comp_point, TVector3 start_point, TVector3 vecG, double costheta, double dtheta);

        void SetCompImageCalPar(int n_cam, double &costheta, double &dtheta, TVector3 &axis, TVector3 &inipos, double &e);
//        double CalCompWeight(TVector3 light_direction, TVector3 axis, double thetaE, double sigma);
//        double CalCompWeight(TVector3 comp_point, TVector3 start_point, TVector3 axis, double costheta, double dtheta);
        double CalCompWeight(TVector3 comp_point, TVector3 start_point, TVector3 axis, double costheta, double dtheta, double e);

        void GetExpValue();
        void FillEnergy();
        void EnergyCorrection();
        void CheckFluo(double c1_e_first, double c1_e_second, double c2_e_first, double c2_e_second);
        void MakeImage();
        void Make3DImage();
        void MakeCompImage();
        void MakeCompImageXY();
        void MakeCompImageZX();
        void MakeCompImageZY();

//        void MakeCompImageEachCamera(int n_cam, TH2D *compimage, TH1D *armhist);
        void MakeCompImageEachCameraXY(int n_cam, TH2F *compimage);
        void MakeCompImageEachCameraZX(int n_cam, TH2F *compimage);
        void MakeCompImageEachCameraZY(int n_cam, TH2F *compimage);
//        void MakeSyncCompImage();
        void MakePETImage();

        double CalTotalWeight();

        bool boolMake3DImage, boolMakePETImage, boolMakeCompImage;

        double gauss_FWHM_171keV;
        double gauss_FWHM_245keV;

        bool boolEnergyCorrection;
        bool boolTwoLines;
        bool boolOneLine;
        bool boolNoLine;
        bool boolRemoveFluoEvent;

        double totalweight;

        bool boolNormalization;
        int norm_n_x, norm_n_y, norm_n_z;
        double rescale_factor;
        double totalweight_cut_min;
        double totalweight_cut_max;

        TH1D *hist_total_weight;
        TH1D *hist_total_weight_aftercut;

    protected:

    public:
        ~In_3D_ComptonImager();
        In_3D_ComptonImager();
        In_3D_ComptonImager(TString infilename_c1, TString infilename_c2);
        In_3D_ComptonImager(TString infilename_c1, TString infilename_c2, TString outfilename);

        void LoadParameterFile(string parfilename);

        void Initialize();
        void Run();
        void SaveImage();
        void PrintPar();

        void SetWorldPosition(double world_x, double world_y, double world_z);
        void SetWorldSize(double world_lx, double world_ly, double world_lz);

        void SetCam1Position(double c1_x, double c1_y, double c1_z);
        void SetCam2Position(double c2_x, double c2_y, double c2_z);
        void SetSliceX(double slice_x);
        void SetSliceY(double slice_y);
        void SetSliceZ(double slice_z);

        void Set3DImageDivision(int n_x, int n_y, int n_z);
        void SetCompImageDivision(int comp_n_x, int comp_n_y, int comp_n_z);
        void SetPETImageDivision(int pet_n_x, int pet_n_y);
        void SetGaussFWHM(double gauss_FWHM_171keV, double gauss_FWHM_245keV);
        void SetEnergyCorrection(bool boolEnergyCorrection, bool boolTwoLines, bool boolOneLine, bool boolNoLine, bool boolRemoveFluoEvent);
        void SetNormPar( bool boolNormalization, int norm_n_x, int norm_n_y, int norm_n_z, double rescale_factor, double totalweight_cut_min, double totalweight_cut_max);

        void SetMakeImageList(bool boolMake3DImage, bool boolMakeCompImage, bool boolMakePETImage);

        void SetPrintDivNum(Long64_t print_div_num);
};

#endif
