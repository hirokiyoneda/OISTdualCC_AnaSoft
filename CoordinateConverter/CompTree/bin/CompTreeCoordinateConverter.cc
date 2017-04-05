#include "CompTreeCoordinateConverter.hh"

/*
v1.1.1 local から global へ最初に変換 2017-03-25 by Hiroki Yoneda
*/

CompTreeCoordinateConverter::CompTreeCoordinateConverter(){
    header = "global_";
    print_div_num = 10000;
}

CompTreeCoordinateConverter::~CompTreeCoordinateConverter(){

}

void CompTreeCoordinateConverter::ReadInputfile(TString infilename){
    in_cetree = new CompTreeReader(infilename);
    this->infilename = infilename;
    totaleventnum = in_cetree->GetEntries();
    delete_fluo = 0;
}

void CompTreeCoordinateConverter::SetOutfileHeader(TString header){
    this->header = header;
}

void CompTreeCoordinateConverter::SetParallelTranslationVector(TVector3 pt_vector){
    this->pt_vector = pt_vector;
}

void CompTreeCoordinateConverter::SetRotateAngle(double rotate_angle){
    this->rotate_angle = rotate_angle;
}

/*
void CompTreeCoordinateConverter::SetDeleteFluo(bool delete_fluo){
    this->delete_fluo = delete_fluo;
}
*/

void CompTreeCoordinateConverter::Initialize(){
    out_cetree = new CompTreeMaker();   

    outfilename = header + infilename;

    out_cetree->Initialize(outfilename);
}

void CompTreeCoordinateConverter::Run(){
    for(Long64_t i_eventnum = 0; i_eventnum < totaleventnum; ++i_eventnum){ 
        currenteventnum = i_eventnum;
        if(i_eventnum%print_div_num == 0){
            cout << i_eventnum << "/" << totaleventnum << endl;
        }
        in_cetree->SetEntry(i_eventnum);

        CopyValueFile();

        ConvertCoordinate();

        /*
        if(delete_fluo){
            if(!DeleteFluo()){
                WriteEvent();
            }
        }else{
            WriteEvent();
        }
        */
        WriteEvent();
    }

    out_cetree->SaveFile();
}

void CompTreeCoordinateConverter::WriteEvent(){
    out_cetree->FillNewEvent();
}

void CompTreeCoordinateConverter::ConvertCoordinate(){
    LoadPosition();
    ParallelTranslation(pt_vector);
    RotateY(rotate_angle);
    SetPosition();
}

void CompTreeCoordinateConverter::LoadPosition(){
	  pt_hit1_posx = out_cetree->hit1_posx;
	  pt_hit1_posy = out_cetree->hit1_posy*-1.0;
	  pt_hit1_posz = out_cetree->hit1_posz;

	  pt_hit2_posx = out_cetree->hit2_posx;
	  pt_hit2_posy = out_cetree->hit2_posy*-1.0;
	  pt_hit2_posz = out_cetree->hit2_posz;
}

void CompTreeCoordinateConverter::ParallelTranslation(TVector3 vector){
    pt_hit1_posx += vector.X();
    pt_hit1_posy += vector.Y();
    pt_hit1_posz += vector.Z();

    pt_hit2_posx += vector.X();
    pt_hit2_posy += vector.Y();
    pt_hit2_posz += vector.Z();
}

void CompTreeCoordinateConverter::RotateY(double angle){
    Float_t new_pt_hit1_posx, new_pt_hit1_posz;
    new_pt_hit1_posx = TMath::Cos(angle)*pt_hit1_posx - TMath::Sin(angle)*pt_hit1_posz;
    new_pt_hit1_posz = TMath::Sin(angle)*pt_hit1_posx + TMath::Cos(angle)*pt_hit1_posz;
    pt_hit1_posx = new_pt_hit1_posx;
    pt_hit1_posz = new_pt_hit1_posz;

    Float_t new_pt_hit2_posx, new_pt_hit2_posz;
    new_pt_hit2_posx = TMath::Cos(angle)*pt_hit2_posx - TMath::Sin(angle)*pt_hit2_posz;
    new_pt_hit2_posz = TMath::Sin(angle)*pt_hit2_posx + TMath::Cos(angle)*pt_hit2_posz;
    pt_hit2_posx = new_pt_hit2_posx;
    pt_hit2_posz = new_pt_hit2_posz;
}

void CompTreeCoordinateConverter::SetPosition(){
	  out_cetree->hit1_posx = pt_hit1_posx;
	  out_cetree->hit1_posy = pt_hit1_posy;
	  out_cetree->hit1_posz = pt_hit1_posz;

	  out_cetree->hit2_posx = pt_hit2_posx;
	  out_cetree->hit2_posy = pt_hit2_posy;
	  out_cetree->hit2_posz = pt_hit2_posz;
}

/*
bool CompTreeCoordinateConverter::DeleteFluo(){
    if(in_cetree->num_hits > 2){
        if(in_cetree->ihit == 0){
            num_hits = in_cetree->num_hits; 
            n_deleled_hit = 0;
            for(Long64_t delta_eventnum = 0; delta_eventnum < in_cetree->num_hits; ++delta_eventnum){
                in_cetree->SetEntry(currenteventnum + delta_eventnum);
                if(TMath::Abs(in_cetree->energy - In111Constant::EneFluo) < AnalysisParameter::DeltaE){
                    --num_hits;
                }
            }
        }

        if(TMath::Abs(out_cetree->energy - In111Constant::EneFluo) < AnalysisParameter::DeltaE){
            ++n_deleled_hit;
            return 1;
        }

        out_cetree->ihit -= n_deleled_hit;
        out_cetree->num_hits = num_hits;
    }
    return 0;
}
*/

void CompTreeCoordinateConverter::CopyValueFile(){
	out_cetree->exptime = in_cetree->exptime;
	out_cetree->eventid = in_cetree->eventid;
	out_cetree->num_hits = in_cetree->num_hits;
	out_cetree->hit1_id = in_cetree->hit1_id;
	out_cetree->hit1_process = in_cetree->hit1_process;
	out_cetree->hit1_detector = in_cetree->hit1_detector;
	out_cetree->hit1_readout_module = in_cetree->hit1_readout_module;
	out_cetree->hit1_section = in_cetree->hit1_section;
	out_cetree->hit1_channel = in_cetree->hit1_channel;
	out_cetree->hit1_pixelx = in_cetree->hit1_pixelx;
	out_cetree->hit1_pixely = in_cetree->hit1_pixely;
	out_cetree->hit1_time = in_cetree->hit1_time;
	out_cetree->hit1_posx = in_cetree->hit1_posx;
	out_cetree->hit1_posy = in_cetree->hit1_posy;
	out_cetree->hit1_posz = in_cetree->hit1_posz;
	out_cetree->hit1_energy = in_cetree->hit1_energy;
	out_cetree->hit2_id = in_cetree->hit2_id;
	out_cetree->hit2_process = in_cetree->hit2_process;
	out_cetree->hit2_detector = in_cetree->hit2_detector;
	out_cetree->hit2_readout_module = in_cetree->hit2_readout_module;
	out_cetree->hit2_section = in_cetree->hit2_section;
	out_cetree->hit2_channel = in_cetree->hit2_channel;
	out_cetree->hit2_pixelx = in_cetree->hit2_pixelx;
	out_cetree->hit2_pixely = in_cetree->hit2_pixely;
	out_cetree->hit2_time = in_cetree->hit2_time;
	out_cetree->hit2_posx = in_cetree->hit2_posx;
	out_cetree->hit2_posy = in_cetree->hit2_posy;
	out_cetree->hit2_posz = in_cetree->hit2_posz;
	out_cetree->hit2_energy = in_cetree->hit2_energy;
	out_cetree->flags = in_cetree->flags;
	out_cetree->costheta = in_cetree->costheta;
	out_cetree->dtheta = in_cetree->dtheta;
	out_cetree->phi = in_cetree->phi;
	out_cetree->hitpattern = in_cetree->hitpattern;
	out_cetree->grade = in_cetree->grade;
	out_cetree->likelihood = in_cetree->likelihood;
	out_cetree->ini_energy = in_cetree->ini_energy;
	out_cetree->ini_dirx = in_cetree->ini_dirx;
	out_cetree->ini_diry = in_cetree->ini_diry;
	out_cetree->ini_dirz = in_cetree->ini_dirz;
	out_cetree->ini_time = in_cetree->ini_time;
	out_cetree->ini_posx = in_cetree->ini_posx;
	out_cetree->ini_posy = in_cetree->ini_posy;
	out_cetree->ini_posz = in_cetree->ini_posz;
	out_cetree->ini_polarx = in_cetree->ini_polarx;
	out_cetree->ini_polary = in_cetree->ini_polary;
	out_cetree->ini_polarz = in_cetree->ini_polarz;
	out_cetree->weight = in_cetree->weight;
}

void CompTreeCoordinateConverter::SetPrintDivNum(Long64_t print_div_num){
    this->print_div_num = print_div_num;
}
