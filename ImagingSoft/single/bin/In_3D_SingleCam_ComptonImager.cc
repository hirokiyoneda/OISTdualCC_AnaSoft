#include "In_3D_SingleCam_ComptonImager.hh"

#define CutPoint

/*
-- 2017-02-19 by Hiroki Yoneda 
-- MakeCompImage() を実装 2017-02-21 by Hiroki Yoneda 
-- 3Dイメージを修正 2017-02-22 by Hiroki Yoneda 
-- In用にfilterを変更 2017-02-22 by Hiroki Yoneda 
-- ComptonImageの作成方法を変更 2017-02-24 by Hiroki Yoneda 
-- ZX, ZYでのスライス面でのComptonImageを追加 2017-02-24 by Hiroki Yoneda 
-- ZX, ZYでのスライス面を変更する関数を追加 2017-03-23 by Hiroki Yoneda 
-- 入力ファイルがglobal座標であることを想定して変更 2017-03-25 by Hiroki Yoneda 
-- ノーマライゼーションを導入。イメージングのガウシアンなましは、エネルギー依存性を導入。 2017-04-06 by Hiroki Yoneda
*/

In_3D_SingleCam_ComptonImager::~In_3D_SingleCam_ComptonImager(){
    cout << "##################" << flush;
    cout << "  PROGRAM DONE !  " << flush;
    cout << "##################" << endl;
}

In_3D_SingleCam_ComptonImager::In_3D_SingleCam_ComptonImager(){
    print_div_num = 100;
}

In_3D_SingleCam_ComptonImager::In_3D_SingleCam_ComptonImager(TString infilename) : TFileMaker("In_3D_ComptonImage.root"){
    ct = new CompTreeReader(infilename);
    SetDefault();
}

In_3D_SingleCam_ComptonImager::In_3D_SingleCam_ComptonImager(TString infilename, TString outfilename) : TFileMaker(outfilename){
    ct = new CompTreeReader(infilename);
    SetDefault();
}

void In_3D_SingleCam_ComptonImager::SaveImage(){
    outfile->Write();
}

void In_3D_SingleCam_ComptonImager::SetDefault(){
    world_x = 0, world_y = 0, world_z = 0;
    world_lx = 20.0 , world_ly = 20.0 , world_lz = 20.0;
    n_x = 10, n_y = 10, n_z = 10;

    c_x = 0 , c_y = 0, c_z = -10.0;

    slice_x = 0.0;
    slice_y = 0.0; 
    slice_z = 0.0;

    comp_n_x = 200;
    comp_n_y = 200;
    comp_n_z = 200;

    boolMake3DImage = true;
    boolMakeCompImage = true;

    gauss_FWHM_171keV = AnalysisParameter::GaussFWHM;
    gauss_FWHM_245keV = AnalysisParameter::GaussFWHM;

    boolNormalization = false;

    norm_n_x = 200;
    norm_n_y = 200;
    norm_n_z = 200;
    rescale_factor = AnalysisParameter::rescale_factor;
    totalweight_cut_min = AnalysisParameter::totalweight_cut_min;
    totalweight_cut_max = AnalysisParameter::totalweight_cut_max;
}

void In_3D_SingleCam_ComptonImager::PrintPar(){
    cout << "##################" << flush;
    cout << "  Parameter List  " << flush;
    cout << "##################" << endl;

    cout << "world centre point (cm) : " 
    << world_x << " " 
    << world_y << " " 
    << world_z << " "
    << endl;

    cout << "world size (cm) : " 
    << world_lx << " " 
    << world_ly << " " 
    << world_lz << " "
    << endl;

    cout << "Camera position (cm) : "
    << c_x << " "
    << c_y << " "
    << c_z << " "
    << endl;

    cout << "slice plane x (cm) : " << slice_x << endl;
    cout << "slice plane y (cm) : " << slice_y << endl;
    cout << "slice plane z (cm) : " << slice_z << endl;

    if(boolMake3DImage == true){
    //cout << "Make3DImage : ON" << endl;
    cout << "bins of 3D image : " 
    << n_x << " " 
    << n_y << " " 
    << n_z << " "
    << endl;
    }

    if(boolMakeCompImage == true){
    //cout << "MakeCompImage : ON" << endl;
    cout << "bins of Compton Images : " 
    << comp_n_x << " "
    << comp_n_y << " "
    << comp_n_z << " "
    << endl;
    }

    cout << "gauss FWHM 171keV : " << gauss_FWHM_171keV << endl;
    cout << "gauss FWHM 245keV : " << gauss_FWHM_245keV << endl;

    if(boolNormalization == true){
    cout << "bins of normalization : " 
    << norm_n_x << " "
    << norm_n_y << " "
    << norm_n_z << " "
    << endl;
    cout << "rescale factor : " << rescale_factor << endl;
    cout << "cut min : " << totalweight_cut_min << endl;
    cout << "cut max : " << totalweight_cut_max << endl;
    }
}

void In_3D_SingleCam_ComptonImager::LoadParameterFile(string parfilename){
    boost::property_tree::ptree par_tree;
    boost::property_tree::read_json(parfilename, par_tree);

    SetWorldPosition(
        par_tree.get<double>("world.centre.x"),
        par_tree.get<double>("world.centre.y"),
        par_tree.get<double>("world.centre.z")
    );

    SetWorldSize(
        par_tree.get<double>("world.size.x"),
        par_tree.get<double>("world.size.y"),
        par_tree.get<double>("world.size.z")
    );

    SetCamPosition(
        par_tree.get<double>("position.camera.x"),
        par_tree.get<double>("position.camera.y"),
        par_tree.get<double>("position.camera.z")
    );

    SetSliceX(par_tree.get<double>("sliceplane.x"));
    SetSliceY(par_tree.get<double>("sliceplane.y"));
    SetSliceZ(par_tree.get<double>("sliceplane.z"));

    SetMakeImageList(
        par_tree.get<bool>("3DComptonImage.run"),
        par_tree.get<bool>("SliceComptonImage.run")
    );

    Set3DImageDivision(
        par_tree.get<int>("3DComptonImage.bin.x"),
        par_tree.get<int>("3DComptonImage.bin.y"),
        par_tree.get<int>("3DComptonImage.bin.z")
    );

    SetCompImageDivision(
        par_tree.get<int>("SliceComptonImage.bin.x"),
        par_tree.get<int>("SliceComptonImage.bin.y"),
        par_tree.get<int>("SliceComptonImage.bin.z")
    );

    SetGaussFWHM(
        par_tree.get<double>("gaussFWHM.171keV", -1.0),
        par_tree.get<double>("gaussFWHM.245keV", -1.0)
    );

    SetNormPar(
        par_tree.get<bool>("Normalization.run", 0),
        par_tree.get<int>("Normalization.bin.x", 100),
        par_tree.get<int>("Normalization.bin.y", 100),
        par_tree.get<int>("Normalization.bin.z", 100),
        par_tree.get<double>("Normalization.rescale", 1.0),
        par_tree.get<double>("Normalization.cut.min", 1e-10),
        par_tree.get<double>("Normalization.cut.max", 1e10)
    );

}

void In_3D_SingleCam_ComptonImager::SetWorldPosition(double world_x, double world_y, double world_z){
    this->world_x = world_x;
    this->world_y = world_y;
    this->world_z = world_z;
}

void In_3D_SingleCam_ComptonImager::SetWorldSize(double world_lx, double world_ly, double world_lz){
    this->world_lx = world_lx;
    this->world_ly = world_ly;
    this->world_lz = world_lz;
}

void In_3D_SingleCam_ComptonImager::Set3DImageDivision(int n_x, int n_y, int n_z){
    this->n_x = n_x;
    this->n_y = n_y;
    this->n_z = n_z;
}

void In_3D_SingleCam_ComptonImager::SetCamPosition(double c_x, double c_y, double c_z){
    this->c_x = c_x;
    this->c_y = c_y;
    this->c_z = c_z;
}

void In_3D_SingleCam_ComptonImager::SetSliceX(double slice_x){
    this->slice_x = slice_x;
}

void In_3D_SingleCam_ComptonImager::SetSliceY(double slice_y){
    this->slice_y = slice_y;
}

void In_3D_SingleCam_ComptonImager::SetSliceZ(double slice_z){
    this->slice_z = slice_z;
}

void In_3D_SingleCam_ComptonImager::SetCompImageDivision(int comp_n_x, int comp_n_y, int comp_n_z){
    this->comp_n_x = comp_n_x;
    this->comp_n_y = comp_n_y;
    this->comp_n_z = comp_n_z;
}

void In_3D_SingleCam_ComptonImager::SetGaussFWHM(double gauss_FWHM_171keV, double gauss_FWHM_245keV){
    if(gauss_FWHM_171keV > 0){
        this->gauss_FWHM_171keV = gauss_FWHM_171keV;
    }

    if(gauss_FWHM_245keV > 0){
        this->gauss_FWHM_245keV = gauss_FWHM_245keV;
    }
}

void In_3D_SingleCam_ComptonImager::SetNormPar( bool boolNormalization, int norm_n_x, int norm_n_y, int norm_n_z, double rescale_factor, double totalweight_cut_min, double totalweight_cut_max){
    this->boolNormalization = boolNormalization;
    this->norm_n_x = norm_n_x;
    this->norm_n_y = norm_n_y;
    this->norm_n_z = norm_n_z;
    this->rescale_factor = rescale_factor;
    this->totalweight_cut_min = totalweight_cut_min;
    this->totalweight_cut_max = totalweight_cut_max;
}

void In_3D_SingleCam_ComptonImager::SetMakeImageList(bool boolMake3DImage, bool boolMakeCompImage){
    this->boolMake3DImage = boolMake3DImage;
    this->boolMakeCompImage = boolMakeCompImage;

    cout << "##################" << flush;
    cout << "  SET ImageList   " << flush;
    cout << "##################" << endl;

    if(boolMake3DImage == true){
        cout << "Make3DImage : ON" << endl;
    }else{
        cout << "Make3DImage : OFF" << endl;
    }

    if(boolMakeCompImage == true){
        cout << "MakeCompImage : ON" << endl;
    }else{
        cout << "MakeCompImage : OFF" << endl;
    }
}

void In_3D_SingleCam_ComptonImager::Initialize(){
    outfile->mkdir("image3d");
    outfile->cd("image3d");
    image_3d = new TH3F("image_3d", "image_3d;X (cm);Y (cm);Z (cm)", 
                        n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
                        n_y, world_y - world_ly/2.0, world_y + world_ly/2.0, 
                        n_z, world_z - world_lz/2.0, world_z + world_lz/2.0
                        );
    projectionXY_image_3d = new TH2F("projectionXY_image_3d", "projectionXY_image_3d;X (cm);Y (cm)", 
                        n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
                        n_y, world_y - world_ly/2.0, world_y + world_ly/2.0
                        );
    projectionZX_image_3d = new TH2F("projectionZX_image_3d", "projectionZX_image_3d;Z (cm);X (cm)", 
                        n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
                        n_x, world_x - world_lx/2.0, world_x + world_lx/2.0
                        );
    projectionZY_image_3d = new TH2F("projectionZY_image_3d", "projectionZY_image_3d;Z (cm);Y (cm)", 
                        n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
                        n_y, world_y - world_ly/2.0, world_y + world_ly/2.0
                        );
    outfile->cd("../");

    outfile->mkdir("compimage");

    outfile->mkdir("compimage/XY");
    outfile->cd("compimage/XY");
    compimage = new TH2F("compimage", "compimage;X (cm);Y (cm)", 
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    arm_compimage = new TH1D("arm_compimage", "arm_compimage;ARM (degree);Counts", 360, -180, 180);

    outfile->cd("../../");

    outfile->mkdir("compimage/ZX");
    outfile->cd("compimage/ZX");
    compimage_ZX = new TH2F("compimage_ZX", "compimage_ZX;Z (cm);X (cm)", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0);
    outfile->cd("../../");

    outfile->mkdir("compimage/ZY");
    outfile->cd("compimage/ZY");
    compimage_ZY = new TH2F("compimage_ZY", "compimage_ZY;Z (cm);Y (cm)", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    outfile->cd("../../");

    outfile->mkdir("energyhist");
    outfile->cd("energyhist");
    ehist = new TH1D("ehist", "ehist;Energy (keV);Counts", 2000, 0, 2000);
    outfile->cd("../");

    outfile->mkdir("normalization");
    outfile->cd("normalization");
    hist_total_weight = new TH1D("totalweight", "totalweight;Log totalweight;Counts", 4000, -20 ,20);
    hist_total_weight_aftercut = new TH1D("totalweight_aftercut", "totalweight_aftercut;Log totalweight;Counts", 4000, -20 ,20);
    outfile->cd("../");
}

void In_3D_SingleCam_ComptonImager::Run(){
    PrintPar();

    cout << "##################" << flush;
    cout << "  START ANALYSIS  " << flush;
    cout << "##################" << endl;

    if(!ImageStartUp()) return;

//    int count = 0;
    do {
        PrintNow();
    
        GetExpValue();
        FillEnergy();
        MakeImage();
         
//        ++count;
//        if(count == 1) break;
#ifdef TESTRUN
        if(current_event_num > 10000){
            break;
        }
#endif
    } while (GetNextEvent());


    cout << "##################" << flush;
    cout << "  END ANALYSIS    " << flush;
    cout << "##################" << endl;

    SaveImage();

    cout << "##################" << flush;
    cout << "  SAVE IMAGES     " << flush;
    cout << "##################" << endl;
}

bool In_3D_SingleCam_ComptonImager::ImageStartUp(){
    total_event_num = ct->GetEntries();

    ct->SetEntry(0);
	
	current_event_num = 0;
    
    //return GetNextEvent();
    if(total_event_num == 0){
        return false;
    }
    return true;
}

bool In_3D_SingleCam_ComptonImager::GetNextEvent(){
    if(current_event_num >= total_event_num - 1){
        return false;
    }
    
    current_event_num++;
    ct->SetEntry(current_event_num);

    return true;
}

void In_3D_SingleCam_ComptonImager::PrintNow(){
    if(current_event_num%print_div_num == 0){
        cout << current_event_num << " / " << total_event_num << endl;
    }
}

void In_3D_SingleCam_ComptonImager::GetExpValue(){
    e = ct->GetTotalEnergy();
	pos = ct->Get_hit1_pos();
	costheta = ct->Get_costheta();
	dtheta = ct->Get_dtheta();
	vecG = ct->Get_vecG();
}

void In_3D_SingleCam_ComptonImager::FillEnergy(){
    ehist->Fill(e);
}

void In_3D_SingleCam_ComptonImager::MakeImage(){
    if( 
        (boolMake3DImage && ThreeDImageFilter()) || (boolMakeCompImage && CompImageFilter())
    ){
        if(boolNormalization){
            totalweight = CalTotalWeight();
            hist_total_weight->Fill(TMath::Log10(totalweight));
            if(totalweight < totalweight_cut_min ||
               totalweight > totalweight_cut_max){
                return;
            }
            hist_total_weight_aftercut->Fill(TMath::Log10(totalweight));
        }else{
            totalweight = 1.0;
            hist_total_weight->Fill(TMath::Log10(totalweight));
            hist_total_weight_aftercut->Fill(TMath::Log10(totalweight));
        }
    }

    if(boolMake3DImage && ThreeDImageFilter())  Make3DImage();
    if(boolMakeCompImage && CompImageFilter()) MakeCompImage();
}

void In_3D_SingleCam_ComptonImager::Make3DImage(){
    TH3F *tmp_image_3d = new TH3F("tmp_image_3d", "tmp_image_3d", 
                        n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
                        n_y, world_y - world_ly/2.0, world_y + world_ly/2.0, 
                        n_z, world_z - world_lz/2.0, world_z + world_lz/2.0
                        );
    TH2F *tmp_projectionXY_image_3d = new TH2F("tmp_projectionXY_image_3d", "tmp_projectionXY_image_3d", 
                        n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
                        n_y, world_y - world_ly/2.0, world_y + world_ly/2.0
                        );
    TH2F *tmp_projectionZX_image_3d = new TH2F("tmp_projectionZX_image_3d", "tmp_projectionZX_image_3d", 
                        n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
                        n_x, world_x - world_lx/2.0, world_x + world_lx/2.0
                        );
    TH2F *tmp_projectionZY_image_3d = new TH2F("tmp_projectionZY_image_3d", "tmp_projectionZY_image_3d", 
                        n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
                        n_y, world_y - world_ly/2.0, world_y + world_ly/2.0
                        );

    for(int i_x = 0; i_x < n_x; ++i_x){
        for(int i_y = 0; i_y < n_y; ++i_y){
            for(int i_z = 0; i_z < n_z; ++i_z){
                TVector3 fillpoint(
                world_x - world_lx/2.0 + world_lx/n_x*(i_x + 0.5),
                world_y - world_ly/2.0 + world_ly/n_y*(i_y + 0.5),
                world_z - world_lz/2.0 + world_lz/n_z*(i_z + 0.5)
                );

                double weight = 1.0;
                weight *= CalCompWeight(fillpoint, pos, vecG, costheta, dtheta, e);

                tmp_image_3d->Fill(fillpoint.X(), fillpoint.Y(), fillpoint.Z(), weight);
                tmp_projectionXY_image_3d->Fill(fillpoint.X(), fillpoint.Y(), weight);
                tmp_projectionZX_image_3d->Fill(fillpoint.Z(), fillpoint.X(), weight);
                tmp_projectionZY_image_3d->Fill(fillpoint.Z(), fillpoint.Y(), weight);
            }
        }
    }
   
    tmp_image_3d->Scale(1.0/totalweight);
    tmp_projectionXY_image_3d->Scale(1.0/totalweight);
    tmp_projectionZX_image_3d->Scale(1.0/totalweight);
    tmp_projectionZY_image_3d->Scale(1.0/totalweight);
    
    image_3d->Add(tmp_image_3d);
    projectionXY_image_3d->Add(tmp_projectionXY_image_3d);
    projectionZX_image_3d->Add(tmp_projectionZX_image_3d);
    projectionZY_image_3d->Add(tmp_projectionZY_image_3d);

    delete tmp_image_3d;
    delete tmp_projectionXY_image_3d;
    delete tmp_projectionZX_image_3d;
    delete tmp_projectionZY_image_3d;
}

void In_3D_SingleCam_ComptonImager::MakeCompImage(){
    MakeCompImageXY();
    MakeCompImageZX();
    MakeCompImageZY();
}

double In_3D_SingleCam_ComptonImager::CalTotalWeight(){
    double totalweight = 0;
    for(int i_x = 0; i_x < norm_n_x; ++i_x){
        for(int i_y = 0; i_y < norm_n_y; ++i_y){
            for(int i_z = 0; i_z < norm_n_z; ++i_z){
                TVector3 fillpoint(
                world_x - world_lx/2.0 + world_lx/n_x*(i_x + 0.5),
                world_y - world_ly/2.0 + world_ly/n_y*(i_y + 0.5),
                world_z - world_lz/2.0 + world_lz/n_z*(i_z + 0.5)
                );

                double weight = 1.0;
                weight *= CalCompWeight(fillpoint, pos, vecG, costheta, dtheta, e);
                
                totalweight += weight;
            }
        }
    }
    
    return totalweight*rescale_factor;
}

void In_3D_SingleCam_ComptonImager::MakeCompImageXY(){
    TH2F *this_compimage = new TH2F("this_compimage", "this_compimage", 
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    double diff_angle;

    MakeCompImageEachCameraXY(1, this_compimage, diff_angle);

    compimage->Add(this_compimage);

    arm_compimage->Fill(diff_angle);

    delete this_compimage;
}

void In_3D_SingleCam_ComptonImager::MakeCompImageZX(){
    TH2F *this_compimage_ZX = new TH2F("this_compimage_ZX", "this_compimage_ZX", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0);

    MakeCompImageEachCameraZX(1, this_compimage_ZX);

    compimage_ZX->Add(this_compimage_ZX);

    delete this_compimage_ZX;
}

void In_3D_SingleCam_ComptonImager::MakeCompImageZY(){
    TH2F *this_compimage_ZY = new TH2F("this_compimage_ZY", "this_compimage_ZY", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);

    MakeCompImageEachCameraZY(1, this_compimage_ZY);

    compimage_ZY->Add(this_compimage_ZY);

    delete this_compimage_ZY;
}

void In_3D_SingleCam_ComptonImager::MakeCompImageEachCameraXY(int n_cam, TH2F *compimage, double &diff_angle){
	double costheta, dtheta, e;
	TVector3 axis, inipos;
    SetCompImageCalPar(n_cam, costheta, dtheta, axis, inipos, e);

#ifdef CutPoint
	if(costheta < -1 || 1 < costheta) return;
#endif

    for(int i_x = 0; i_x < comp_n_x; ++i_x){
        for(int i_y = 0; i_y < comp_n_y; ++i_y){
            TVector3 fillpoint(
            world_x - world_lx/2.0 + world_lx/comp_n_x*(i_x + 0.5),
            world_y - world_ly/2.0 + world_ly/comp_n_y*(i_y + 0.5),
            slice_z
            );
            
            double weight = CalCompWeight(fillpoint, inipos, axis, costheta, dtheta, e);
            if(weight > 0){
                compimage->Fill(fillpoint.X(), fillpoint.Y(), weight);
            }
        }
    }

//Arm
    TVector3 source(0,0, slice_z);
    TVector3 light_direction = source - inipos;
    double thetaGeo = light_direction.Angle(axis);
    double thetaE =  TMath::ACos(costheta);
    diff_angle = 180.0/TMath::Pi()*(thetaGeo-thetaE);
//    
//    armhist->Fill(diff_angle);
}

void In_3D_SingleCam_ComptonImager::MakeCompImageEachCameraZX(int n_cam, TH2F *compimage){
	double costheta, dtheta, e;
	TVector3 axis, inipos;
    SetCompImageCalPar(n_cam, costheta, dtheta, axis, inipos, e);

#ifdef CutPoint
	if(costheta < -1 || 1 < costheta) return;
#endif

    for(int i_z = 0; i_z < comp_n_z; ++i_z){
        for(int i_x = 0; i_x < comp_n_x; ++i_x){
            TVector3 fillpoint(
            world_x - world_lx/2.0 + world_lx/comp_n_x*(i_x + 0.5),
            slice_y,
            world_z - world_lz/2.0 + world_lz/comp_n_z*(i_z + 0.5)
            );
            
            double weight = CalCompWeight(fillpoint, inipos, axis, costheta, dtheta, e);
            if(weight > 0){
                compimage->Fill(fillpoint.Z(), fillpoint.X(), weight);
            }
        }
    }
}

void In_3D_SingleCam_ComptonImager::MakeCompImageEachCameraZY(int n_cam, TH2F *compimage){
	double costheta, dtheta, e;
	TVector3 axis, inipos;
    SetCompImageCalPar(n_cam, costheta, dtheta, axis, inipos, e);

#ifdef CutPoint
	if(costheta < -1 || 1 < costheta) return;
#endif

    for(int i_z = 0; i_z < comp_n_z; ++i_z){
        for(int i_y = 0; i_y < comp_n_y; ++i_y){
            TVector3 fillpoint(
            slice_x,
            world_y - world_ly/2.0 + world_ly/comp_n_y*(i_y + 0.5),
            world_z - world_lz/2.0 + world_lz/comp_n_z*(i_z + 0.5)
            );
            
            double weight = CalCompWeight(fillpoint, inipos, axis, costheta, dtheta, e);
            if(weight > 0){
                compimage->Fill(fillpoint.Z(), fillpoint.Y(), weight);
            }
        }
    }
}

void In_3D_SingleCam_ComptonImager::SetCompImageCalPar(int n_cam, double &costheta, double &dtheta, TVector3 &axis, TVector3 &inipos, double &e){
    costheta = this->costheta;
    dtheta = this->dtheta;
    axis = this->vecG;
    inipos = this->pos;
    e = this->e;
}

double In_3D_SingleCam_ComptonImager::CalCompWeight(TVector3 comp_point, TVector3 start_point, TVector3 axis, double costheta, double dtheta, double e){
	double sigma;
    if(check_line(e) == 1){
	    sigma = TMath::Pi()/180.0*(gauss_FWHM_171keV/2.355);
    }else if(check_line(e) == 2){
	    sigma = TMath::Pi()/180.0*(gauss_FWHM_245keV/2.355);
    }else{
        cerr << "error in calculation of filling weight." << endl;
    }

    double  thetaE =  TMath::ACos(costheta);

    TVector3 light_direction = comp_point - start_point;
    TVector3 artho = light_direction.Cross(axis);

    TVector3 diri1 = axis;
    diri1.Rotate(thetaE,artho);

    TVector3 diri2 = axis;
    diri2.Rotate(-thetaE,artho);

    TVector3 diri;

    if(diri1.Angle(light_direction) < diri2.Angle(light_direction)){
        diri = diri1;
    }else{
        diri = diri2;
    }
           
    if(diri.Dot(light_direction) < 0) return 0;

    TVector3 coneprojection_direction = diri.Dot(light_direction)*diri;
    double length_along_cone = coneprojection_direction.Mag();
    TVector3 diff_vec = coneprojection_direction - light_direction;

    double diff_length = diff_vec.Mag();

    double weight = 1.0;
    weight *= TMath::Power(length_along_cone, -1*AnalysisParameter::CompImageW);
    double comp_sigma = diff_length/(length_along_cone*TMath::Tan(sigma));
    if(comp_sigma < AnalysisParameter::CompImageSigmaCut) {
        weight *= TMath::Exp(-0.5*TMath::Power(comp_sigma,2));
        return weight;
    }
    return 0;
}

bool In_3D_SingleCam_ComptonImager::ThreeDImageFilter(){
    return CompImageFilter();
}

bool In_3D_SingleCam_ComptonImager::CompImageFilter(){
    if(check_line(e) != 0){
           return true;
    }
    return false;
}

void In_3D_SingleCam_ComptonImager::SetPrintDivNum(Long64_t print_div_num){
    this->print_div_num = print_div_num;
}

int In_3D_SingleCam_ComptonImager::check_line(double e){ //0 : no line, 1 : 171 keV, 2 : 245 keV
    if( TMath::Abs(e - In111Constant::EneLine1) < AnalysisParameter::DeltaE ){
        return 1; 
    }else if(TMath::Abs(e - In111Constant::EneLine2) < AnalysisParameter::DeltaE){
        return 2;
    }
    return 0;
}
