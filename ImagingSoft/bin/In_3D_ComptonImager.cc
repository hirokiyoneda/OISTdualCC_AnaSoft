#include "In_3D_ComptonImager.hh"

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

In_3D_ComptonImager::~In_3D_ComptonImager(){
    cout << "##################" << flush;
    cout << "  PROGRAM DONE !  " << flush;
    cout << "##################" << endl;
}

In_3D_ComptonImager::In_3D_ComptonImager(){
    print_div_num = 100;
}

In_3D_ComptonImager::In_3D_ComptonImager(TString infilename_c1, TString infilename_c2) : TFileMaker("In_3D_ComptonImage.root"){
    ct_c1 = new CompTreeReader(infilename_c1);
    ct_c2 = new CompTreeReader(infilename_c2);
    SetDefault();
}

In_3D_ComptonImager::In_3D_ComptonImager(TString infilename_c1, TString infilename_c2, TString outfilename) : TFileMaker(outfilename){
    ct_c1 = new CompTreeReader(infilename_c1);
    ct_c2 = new CompTreeReader(infilename_c2);
    SetDefault();
}

void In_3D_ComptonImager::SaveImage(){
    TH2F add2dhist_c1c2_multiply_non_coinimage = (*compimage_c1)*(*compimage_c2);
    compimage_c1c2_multiply_non_coinimage->Add(&add2dhist_c1c2_multiply_non_coinimage);

    TH2F add2dhist_c1c2_ZX_multiply_non_coinimage = (*compimage_c1_ZX)*(*compimage_c2_ZX);
    compimage_c1c2_ZX_multiply_non_coinimage->Add(&add2dhist_c1c2_ZX_multiply_non_coinimage);

    TH2F add2dhist_c1c2_ZY_multiply_non_coinimage = (*compimage_c1_ZY)*(*compimage_c2_ZY);
    compimage_c1c2_ZY_multiply_non_coinimage->Add(&add2dhist_c1c2_ZY_multiply_non_coinimage);

//    outfile->Add(image_3d);
//    outfile->Add(projectionXY_image_3d);
//    outfile->Add(projectionZX_image_3d);
//    outfile->Add(projectionZY_image_3d);
//    outfile->Add(compimage_c1);
//    outfile->Add(arm_compimage_c1);
//    outfile->Add(compimage_c2);
//    outfile->Add(arm_compimage_c2);
//    outfile->Add(compimage_c1c2);
//
//    outfile->Add(compimage_c1_ZX);
//    outfile->Add(compimage_c2_ZX);
//    outfile->Add(compimage_c1c2_ZX);
//
//    outfile->Add(compimage_c1_ZY);
//    outfile->Add(compimage_c2_ZY);
//    outfile->Add(compimage_c1c2_ZY);
//
//    outfile->Add(image_pet);
//    outfile->Add(ehist_c1);
//    outfile->Add(ehist_c2);
//    outfile->Add(ehist_sum);
//    outfile->Add(ehist_c1c2);

    outfile->Write();

}

void In_3D_ComptonImager::SetDefault(){
    world_x = 0, world_y = 0, world_z = 0;
    world_lx = 20.0 , world_ly = 20.0 , world_lz = 20.0;
    n_x = 10, n_y = 10, n_z = 10;

    c1_x = 0 , c1_y = 0, c1_z = -10.0;
    c2_x = 0 , c2_y = 0, c2_z = 10.0;

    slice_x = 0.0;
    slice_y = 0.0; 
    slice_z = 0.0;

    comp_n_x = 200;
    comp_n_y = 200;
    comp_n_z = 200;

    pet_n_x = 1000;
    pet_n_y = 1000;

    boolMake3DImage = true;
    boolMakeCompImage = true;
    boolMakePETImage = true;

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

void In_3D_ComptonImager::PrintPar(){
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

    cout << "Camera1 position (cm) : "
    << c1_x << " "
    << c1_y << " "
    << c1_z << " "
    << endl;

    cout << "Camera2 position (cm) : "
    << c2_x << " "
    << c2_y << " "
    << c2_z << " "
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

    if(boolMakePETImage == true){
    //cout << "MakePETImage : ON" << endl;
    cout << "bins of PET Images : " 
    << pet_n_x << " "
    << pet_n_y << " "
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

void In_3D_ComptonImager::LoadParameterFile(string parfilename){
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

    SetCam1Position(
        par_tree.get<double>("position.camera1.x"),
        par_tree.get<double>("position.camera1.y"),
        par_tree.get<double>("position.camera1.z")
    );

    SetCam2Position(
        par_tree.get<double>("position.camera2.x"),
        par_tree.get<double>("position.camera2.y"),
        par_tree.get<double>("position.camera2.z")
    );

    SetSliceX(par_tree.get<double>("sliceplane.x"));
    SetSliceY(par_tree.get<double>("sliceplane.y"));
    SetSliceZ(par_tree.get<double>("sliceplane.z"));

    SetMakeImageList(
        par_tree.get<bool>("3DComptonImage.run"),
        par_tree.get<bool>("SliceComptonImage.run"),
        par_tree.get<bool>("PETImage.run")
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

    SetPETImageDivision(
        par_tree.get<int>("PETImage.bin.x"),
        par_tree.get<int>("PETImage.bin.y")
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

void In_3D_ComptonImager::SetWorldPosition(double world_x, double world_y, double world_z){
    this->world_x = world_x;
    this->world_y = world_y;
    this->world_z = world_z;
}

void In_3D_ComptonImager::SetWorldSize(double world_lx, double world_ly, double world_lz){
    this->world_lx = world_lx;
    this->world_ly = world_ly;
    this->world_lz = world_lz;
}

void In_3D_ComptonImager::Set3DImageDivision(int n_x, int n_y, int n_z){
    this->n_x = n_x;
    this->n_y = n_y;
    this->n_z = n_z;
}

void In_3D_ComptonImager::SetCam1Position(double c1_x, double c1_y, double c1_z){
    this->c1_x = c1_x;
    this->c1_y = c1_y;
    this->c1_z = c1_z;
}

void In_3D_ComptonImager::SetCam2Position(double c2_x, double c2_y, double c2_z){
    this->c2_x = c2_x;
    this->c2_y = c2_y;
    this->c2_z = c2_z;
}

void In_3D_ComptonImager::SetSliceX(double slice_x){
    this->slice_x = slice_x;
}

void In_3D_ComptonImager::SetSliceY(double slice_y){
    this->slice_y = slice_y;
}

void In_3D_ComptonImager::SetSliceZ(double slice_z){
    this->slice_z = slice_z;
}

void In_3D_ComptonImager::SetCompImageDivision(int comp_n_x, int comp_n_y, int comp_n_z){
    this->comp_n_x = comp_n_x;
    this->comp_n_y = comp_n_y;
    this->comp_n_z = comp_n_z;
}

void In_3D_ComptonImager::SetPETImageDivision(int pet_n_x, int pet_n_y){
    this->pet_n_x = pet_n_x;
    this->pet_n_y = pet_n_y;
}

void In_3D_ComptonImager::SetGaussFWHM(double gauss_FWHM_171keV, double gauss_FWHM_245keV){
    if(gauss_FWHM_171keV > 0){
        this->gauss_FWHM_171keV = gauss_FWHM_171keV;
    }

    if(gauss_FWHM_245keV > 0){
        this->gauss_FWHM_245keV = gauss_FWHM_245keV;
    }
}

void In_3D_ComptonImager::SetNormPar( bool boolNormalization, int norm_n_x, int norm_n_y, int norm_n_z, double rescale_factor, double totalweight_cut_min, double totalweight_cut_max){
    this->boolNormalization = boolNormalization;
    this->norm_n_x = norm_n_x;
    this->norm_n_y = norm_n_y;
    this->norm_n_z = norm_n_z;
    this->rescale_factor = rescale_factor;
    this->totalweight_cut_min = totalweight_cut_min;
    this->totalweight_cut_max = totalweight_cut_max;
}

void In_3D_ComptonImager::SetMakeImageList(bool boolMake3DImage, bool boolMakeCompImage, bool boolMakePETImage){
    this->boolMake3DImage = boolMake3DImage;
    this->boolMakeCompImage = boolMakeCompImage;
    this->boolMakePETImage = boolMakePETImage;

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

    if(boolMakePETImage == true){
        cout << "MakePETImage : ON" << endl;
    }else{
        cout << "MakePETImage : OFF" << endl;
    }
}

void In_3D_ComptonImager::Initialize(){
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
    //compimage_c1 = new TH2D("compimage_c1", "compimage_c1", 
    compimage_c1 = new TH2F("compimage_c1", "compimage_c1;X (cm);Y (cm)", 
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    arm_compimage_c1 = new TH1D("arm_compimage_c1", "arm_compimage_c1;ARM (degree);Counts", 360, -180, 180);

    //compimage_c2 = new TH2D("compimage_c2", "compimage_c2", 
    compimage_c2 = new TH2F("compimage_c2", "compimage_c2;X (cm);Y (cm)", 
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    arm_compimage_c2 = new TH1D("arm_compimage_c2", "arm_compimage_c2;ARM (degree);Counts", 360, -180, 180);

    compimage_c1c2 = new TH2F("compimage_c1c2", "compimage_c1c2;X (cm);Y (cm)", 
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    compimage_c1c2_multiply_non_coinimage = new TH2F("compimage_c1c2_multiply_non_coinimage", "compimage_c1c2_multiply_non_coinimage;X (cm);Y (cm)", 
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    outfile->cd("../../");

    outfile->mkdir("compimage/ZX");
    outfile->cd("compimage/ZX");
    compimage_c1_ZX = new TH2F("compimage_c1_ZX", "compimage_c1_ZX;Z (cm);X (cm)", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0);
    compimage_c2_ZX = new TH2F("compimage_c2_ZX", "compimage_c2_ZX;Z (cm);X (cm)", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0);
    compimage_c1c2_ZX = new TH2F("compimage_c1c2_ZX", "compimage_c1c2_ZX;Z (cm);X (cm)", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0);
    compimage_c1c2_ZX_multiply_non_coinimage = new TH2F("compimage_c1c2_ZX_multiply_non_coinimage", "compimage_c1c2_ZX_multiply_non_coinimage;Z (cm);X (cm)", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0);
    outfile->cd("../../");

    outfile->mkdir("compimage/ZY");
    outfile->cd("compimage/ZY");
    compimage_c1_ZY = new TH2F("compimage_c1_ZY", "compimage_c1_ZY;Z (cm);Y (cm)", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    compimage_c2_ZY = new TH2F("compimage_c2_ZY", "compimage_c2_ZY;Z (cm);Y (cm)", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    compimage_c1c2_ZY = new TH2F("compimage_c1c2_ZY", "compimage_c1c2_Z;Z (cm);Y (cm)Y", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    compimage_c1c2_ZY_multiply_non_coinimage = new TH2F("compimage_c1c2_ZY_multiply_non_coinimage", "compimage_c1c2_ZY_multiply_non_coinimage;Z (cm);Y (cm)", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    outfile->cd("../../");

    outfile->mkdir("PETimage");
    outfile->cd("PETimage");
    image_pet = new TH2D("PET_image", "PET_image;X (cm);Y (cm)", 
            pet_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            pet_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    outfile->cd("../");

    outfile->mkdir("energyhist");
    outfile->cd("energyhist");
    ehist_c1 = new TH1D("ehist_c1", "ehist_c1;Energy (keV);Counts", 2000, 0, 2000);
    ehist_c2 = new TH1D("ehist_c2", "ehist_c2;Energy (keV);Counts", 2000, 0, 2000);
    ehist_sum = new TH1D("ehist_sum", "ehist_sum;Energy (keV);Counts", 2000, 0, 2000);
    ehist_c1c2 = new TH2D("ehist_c1c2", "ehist_c1c2;Energy (keV);Energy (keV)", 2000, 0, 2000, 2000, 0, 2000);
    outfile->cd("../");

    outfile->mkdir("normalization");
    outfile->cd("normalization");
    hist_total_weight = new TH1D("totalweight", "totalweight;Log totalweight;Counts", 4000, -20 ,20);
    hist_total_weight_aftercut = new TH1D("totalweight_aftercut", "totalweight_aftercut;Log totalweight;Counts", 4000, -20 ,20);
    outfile->cd("../");
}

void In_3D_ComptonImager::Run(){
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
    } while (GetNextEvent());


    cout << "##################" << flush;
    cout << "  END ANALYSIS    " << flush;
    cout << "##################" << endl;

    SaveImage();

    cout << "##################" << flush;
    cout << "  SAVE IMAGES     " << flush;
    cout << "##################" << endl;
}

bool In_3D_ComptonImager::ImageStartUp(){
    c1_total_event_num = ct_c1->GetEntries();
    c2_total_event_num = ct_c2->GetEntries();

    ct_c1->SetEntry(0);
    ct_c2->SetEntry(0);
	
	c1_current_event_num = 0;
	c2_current_event_num = 0;

	while(ct_c1->Get_eventid() < ct_c2->Get_eventid() && c1_current_event_num < c1_total_event_num - 1){
        ++c1_current_event_num;
        ct_c1->SetEntry(c1_current_event_num);
    }

	if(ct_c1->Get_eventid() == ct_c2->Get_eventid()){
        return true;
    }
    
    return GetNextEvent();
}

bool In_3D_ComptonImager::GetNextEvent(){
    if(c1_current_event_num >= c1_total_event_num - 1){
        return false;
    }
    if(c2_current_event_num >= c2_total_event_num - 1){
        return false;
    }
	if(ct_c1->Get_eventid() == ct_c2->Get_eventid()){
        ++c1_current_event_num;
        ct_c1->SetEntry(c1_current_event_num);
    }

    for( ; c1_current_event_num < c1_total_event_num; ++c1_current_event_num, ct_c1->SetEntry(c1_current_event_num)){
        for( ; c2_current_event_num < c2_total_event_num; ++c2_current_event_num, ct_c2->SetEntry(c2_current_event_num)){
            if(ct_c1->Get_eventid() > ct_c2->Get_eventid()){

            }else if(ct_c1->Get_eventid() == ct_c2->Get_eventid()){
                return true;
            }else{
                --c2_current_event_num;
                ct_c2->SetEntry(c2_current_event_num);
                break;
            }
        }
    }
    return false;
}

void In_3D_ComptonImager::PrintNow(){
    if(c1_current_event_num%print_div_num == 0){
        cout << c1_current_event_num << " / " << c1_total_event_num << endl;
    }
}

void In_3D_ComptonImager::GetExpValue(){
    e1 = ct_c1->GetTotalEnergy();
	c1_pos = ct_c1->Get_hit1_pos();
	c1_costheta = ct_c1->Get_costheta();
	c1_dtheta = ct_c1->Get_dtheta();
	c1_vecG = ct_c1->Get_vecG();

    e2 = ct_c2->GetTotalEnergy();
	c2_pos = ct_c2->Get_hit1_pos();
//    c2_pos.SetXYZ(-c2_pos.X() + c2_x, c2_pos.Y() + c2_y, c2_z - c2_pos.Z());
	c2_costheta = ct_c2->Get_costheta();
	c2_dtheta = ct_c2->Get_dtheta();
	c2_vecG = ct_c2->Get_vecG();
//   c2_vecG.SetXYZ(-c2_vecG.X(), c2_vecG.Y(), - c2_vecG.Z());
}

void In_3D_ComptonImager::FillEnergy(){
    ehist_c1->Fill(e1);
    ehist_c2->Fill(e2);
    ehist_c1c2->Fill(e1,e2);
    ehist_sum->Fill(e1+e2);
}

void In_3D_ComptonImager::MakeImage(){
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

    if(boolMakePETImage && PETFilter()) MakePETImage();
}

void In_3D_ComptonImager::Make3DImage(){
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

/*
double In_3D_ComptonImager::CalWeight(TVector3 comp_point, TVector3 start_point, TVector3 vecG, double costheta, double dtheta){
    TVector3 new_vecG = comp_point - start_point;

    double thetaE =  TMath::ACos(costheta);
    double diff_angle = new_vecG.Angle(vecG) - thetaE;
    diff_angle *= 180.0/TMath::Pi();

	double sigma = TMath::Pi()/180.0*(AnalysisParameter::GaussFWHM/2.355);

    double weight = TMath::Gaus(diff_angle, 0, sigma);

    return weight;
}
*/

void In_3D_ComptonImager::MakeCompImage(){
    MakeCompImageXY();
    MakeCompImageZX();
    MakeCompImageZY();
}

double In_3D_ComptonImager::CalTotalWeight(){
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
                weight *= CalCompWeight(fillpoint, c1_pos, c1_vecG, c1_costheta, c1_dtheta, e1);
                weight *= CalCompWeight(fillpoint, c2_pos, c2_vecG, c2_costheta, c2_dtheta, e2);
                
                totalweight += weight;
            }
        }
    }
    
    return totalweight*rescale_factor;
}

void In_3D_ComptonImager::MakeCompImageXY(){
    //TH2D *this_compimage_c1 = new TH2D("this_compimage_c1", "this_compimage_c1", 
    TH2F *this_compimage_c1 = new TH2F("this_compimage_c1", "this_compimage_c1", 
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    //TH1D *this_arm_compimage_c1 = new TH1D("this_arm_compimage_c1", "this_arm_compimage_c1", 360, -180, 180);
    double diff_angle_c1;

    //TH2D *this_compimage_c2 = new TH2D("this_compimage_c2", "this_compimage_c2", 
    TH2F *this_compimage_c2 = new TH2F("this_compimage_c2", "this_compimage_c2", 
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);
    //TH1D *this_arm_compimage_c2 = new TH1D("this_arm_compimage_c2", "this_arm_compimage_c2", 360, -180, 180);
    double diff_angle_c2;

    TH2F *this_compimage_c1c2 = new TH2F("this_compimage_c1c2", "this_compimage_c1c2", 
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0, 
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);

    MakeCompImageEachCameraXY(1, this_compimage_c1, diff_angle_c1);
    MakeCompImageEachCameraXY(2, this_compimage_c2, diff_angle_c2);

    compimage_c1->Add(this_compimage_c1);
    compimage_c2->Add(this_compimage_c2);

    arm_compimage_c1->Fill(diff_angle_c1);
    arm_compimage_c2->Fill(diff_angle_c2);

    TH2F add2dhist_c1c2 = (*this_compimage_c1)*(*this_compimage_c2);

    add2dhist_c1c2.Scale(1.0/totalweight);
    
    compimage_c1c2->Add(&add2dhist_c1c2);

    delete this_compimage_c1;
//    delete this_arm_compimage_c1;
    delete this_compimage_c2;
//    delete this_arm_compimage_c2;
    delete this_compimage_c1c2;
}

void In_3D_ComptonImager::MakeCompImageZX(){
    TH2F *this_compimage_c1_ZX = new TH2F("this_compimage_c1_ZX", "this_compimage_c1_ZX", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0);

    TH2F *this_compimage_c2_ZX = new TH2F("this_compimage_c2_ZX", "this_compimage_c2_ZX", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0);

    TH2F *this_compimage_c1c2_ZX = new TH2F("this_compimage_c1c2_ZX", "this_compimage_c1c2_ZX", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_x, world_x - world_lx/2.0, world_x + world_lx/2.0);

    MakeCompImageEachCameraZX(1, this_compimage_c1_ZX);
    MakeCompImageEachCameraZX(2, this_compimage_c2_ZX);

    compimage_c1_ZX->Add(this_compimage_c1_ZX);
    compimage_c2_ZX->Add(this_compimage_c2_ZX);

    TH2F add2dhist_c1c2_ZX = (*this_compimage_c1_ZX)*(*this_compimage_c2_ZX);

    add2dhist_c1c2_ZX.Scale(1.0/totalweight);
    
    compimage_c1c2_ZX->Add(&add2dhist_c1c2_ZX);

    delete this_compimage_c1_ZX;
    delete this_compimage_c2_ZX;
    delete this_compimage_c1c2_ZX;
}

void In_3D_ComptonImager::MakeCompImageZY(){
    TH2F *this_compimage_c1_ZY = new TH2F("this_compimage_c1_ZY", "this_compimage_c1_ZY", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);

    TH2F *this_compimage_c2_ZY = new TH2F("this_compimage_c2_ZY", "this_compimage_c2_ZY", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);

    TH2F *this_compimage_c1c2_ZY = new TH2F("this_compimage_c1c2_ZY", "this_compimage_c1c2_ZY", 
            comp_n_z, world_z - world_lz/2.0, world_z + world_lz/2.0,
            comp_n_y, world_y - world_ly/2.0, world_y + world_ly/2.0);

    MakeCompImageEachCameraZY(1, this_compimage_c1_ZY);
    MakeCompImageEachCameraZY(2, this_compimage_c2_ZY);

    compimage_c1_ZY->Add(this_compimage_c1_ZY);
    compimage_c2_ZY->Add(this_compimage_c2_ZY);

    TH2F add2dhist_c1c2_ZY = (*this_compimage_c1_ZY)*(*this_compimage_c2_ZY);

    add2dhist_c1c2_ZY.Scale(1.0/totalweight);
    
    compimage_c1c2_ZY->Add(&add2dhist_c1c2_ZY);

    delete this_compimage_c1_ZY;
    delete this_compimage_c2_ZY;
    delete this_compimage_c1c2_ZY;
}

void In_3D_ComptonImager::MakePETImage(){
    double x = (c2_pos.X()*(c2_pos.Z() - slice_z) + c1_pos.X()*(slice_z - c1_pos.Z()))/(c2_pos.Z() - c1_pos.Z());
    double y = (c2_pos.Y()*(c2_pos.Z() - slice_z) + c1_pos.Y()*(slice_z - c1_pos.Z()))/(c2_pos.Z() - c1_pos.Z());

    image_pet->Fill(x,y);
}

void In_3D_ComptonImager::MakeCompImageEachCameraXY(int n_cam, TH2F *compimage, double &diff_angle){
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

void In_3D_ComptonImager::MakeCompImageEachCameraZX(int n_cam, TH2F *compimage){
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

void In_3D_ComptonImager::MakeCompImageEachCameraZY(int n_cam, TH2F *compimage){
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

void In_3D_ComptonImager::SetCompImageCalPar(int n_cam, double &costheta, double &dtheta, TVector3 &axis, TVector3 &inipos, double &e){
    if(n_cam == 1){
        costheta = c1_costheta;
        dtheta = c1_dtheta;
        axis = c1_vecG;
        inipos = c1_pos;
        e = e1;
    }else if(n_cam == 2){
        costheta = c2_costheta;
        dtheta = c2_dtheta;
        axis = c2_vecG;
        inipos = c2_pos;
        e = e2;
    }else{
        cerr << "input wrong parameter" << endl;
        return;
    }
}

double In_3D_ComptonImager::CalCompWeight(TVector3 comp_point, TVector3 start_point, TVector3 axis, double costheta, double dtheta, double e){
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

/*
void In_3D_ComptonImager::MakeSyncCompImage(){
#ifdef CutPoint
	if(c1_costheta < -1 || 1 < c1_costheta) return;
#endif

	double sigma = TMath::Pi()/180.0*(AnalysisParameter::GaussFWHM/2.355);
    double  thetaE =  TMath::ACos(c1_costheta);
    TVector3 inipos = c1_pos;
    TVector3 axis = c1_vecG;
    TVector3 artho = axis.Orthogonal();
    TVector3 diri = axis;
    diri.Rotate(thetaE,artho);

    for(int i=0;i<AnalysisParameter::CompImageSampleNum;i++){
        double phi = 2.*TMath::Pi()*gRandom->Uniform(0,1);
        double dtheta = gRandom->Gaus(0.,sigma);
        TVector3 sampl = diri;
        sampl.Rotate(dtheta,artho);
        sampl.Rotate(phi,axis);

        double factor = (slice_z - inipos.Z())/sampl.Z();

        if(factor > 0){
            double x = sampl.X()*factor + inipos.X();
            double y = sampl.Y()*factor + inipos.Y();

            TVector3 comp_point(x, y, slice_z);

            TVector3 new_c2_vecG = comp_point - c2_pos;

            double c2_thetaE =  TMath::ACos(c2_costheta);
            double diff_angle = new_c2_vecG.Angle(c2_vecG) - c2_thetaE;
            diff_angle *= 180.0/TMath::Pi();

            double weight = TMath::Gaus(diff_angle, 0, sigma);

            compimage_c1c2->Fill(x, y, weight);
        }

    }
//Arm
///    TVector3 source(0,0,distance_source);
//    TVector3 light_direction = source - v1;
//    double thetaGeo = light_direction.Angle(axis);
//    double thetaGeo = source.Angle(axis);
//    double diff_angle = 180.0/TMath::Pi()*(thetaGeo-thetaE);
//    
//    ArmHist->Fill(diff_angle);
}
*/


bool In_3D_ComptonImager::ThreeDImageFilter(){
    return CompImageFilter();
}

bool In_3D_ComptonImager::CompImageFilter(){
    if(check_line(e1)*check_line(e2) == 2){
           return true;
    }
    return false;
}

bool In_3D_ComptonImager::PETFilter(){

    return true;
}

void In_3D_ComptonImager::SetPrintDivNum(Long64_t print_div_num){
    this->print_div_num = print_div_num;
}

int In_3D_ComptonImager::check_line(double e){ //0 : no line, 1 : 171 keV, 2 : 245 keV
    if( TMath::Abs(e - In111Constant::EneLine1) < AnalysisParameter::DeltaE ){
        return 1; 
    }else if(TMath::Abs(e - In111Constant::EneLine2) < AnalysisParameter::DeltaE){
        return 2;
    }
    return 0;
}
