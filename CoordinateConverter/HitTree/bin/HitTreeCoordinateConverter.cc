#include "HitTreeCoordinateConverter.hh"

/*
v1.1.1 local から global へ最初に変換 2017-03-25 by Hiroki Yoneda
*/

//const double HitTreeCoordinateConverter::EneLine1 = 171.28;
//const double HitTreeCoordinateConverter::EneLine2 = 245.395;
//const double HitTreeCoordinateConverter::EneFluo = 23.174;
//const double HitTreeCoordinateConverter::DeltaE = 5.0;

HitTreeCoordinateConverter::HitTreeCoordinateConverter(){
    header = "global_";
    print_div_num = 10000;
}

HitTreeCoordinateConverter::~HitTreeCoordinateConverter(){

}

void HitTreeCoordinateConverter::ReadInputfile(TString infilename){
    in_hittree = new HitTreeReader(infilename);
    this->infilename = infilename;
    totaleventnum = in_hittree->GetEntries();
    delete_fluo = 0;
}

void HitTreeCoordinateConverter::SetOutfileHeader(TString header){
    this->header = header;
}

void HitTreeCoordinateConverter::SetParallelTranslationVector(TVector3 pt_vector){
    this->pt_vector = pt_vector;
}

void HitTreeCoordinateConverter::SetRotateAngle(double rotate_angle){
    this->rotate_angle = rotate_angle;
}

void HitTreeCoordinateConverter::SetDeleteFluo(bool delete_fluo){
    this->delete_fluo = delete_fluo;
}

void HitTreeCoordinateConverter::Initialize(){
    out_hittree = new HitTreeMaker();   

    outfilename = header + infilename;

    out_hittree->Initialize(outfilename);
}

void HitTreeCoordinateConverter::Run(){
    for(Long64_t i_eventnum = 0; i_eventnum < totaleventnum; ++i_eventnum){ 
        currenteventnum = i_eventnum;
        if(i_eventnum%print_div_num == 0){
            cout << i_eventnum << "/" << totaleventnum << endl;
        }
        in_hittree->SetEntry(i_eventnum);

        CopyValueFile();

        ConvertCoordinate();

        if(delete_fluo){
            if(!DeleteFluo()){
                WriteEvent();
            }
        }else{
            WriteEvent();
        }
    }

    out_hittree->SaveFile();
}

void HitTreeCoordinateConverter::WriteEvent(){
    out_hittree->FillNewEvent();
}

void HitTreeCoordinateConverter::ConvertCoordinate(){
    LoadPosition();
    ParallelTranslation(pt_vector);
    RotateY(rotate_angle);
    SetPosition();
}

void HitTreeCoordinateConverter::LoadPosition(){
	  pt_posx = out_hittree->posx;
	  pt_posy = out_hittree->posy*-1.0;
	  pt_posz = out_hittree->posz;
}

void HitTreeCoordinateConverter::ParallelTranslation(TVector3 vector){
    pt_posx += vector.X();
    pt_posy += vector.Y();
    pt_posz += vector.Z();
}

void HitTreeCoordinateConverter::RotateY(double angle){
    Float_t new_pt_posx, new_pt_posz;
    new_pt_posx = TMath::Cos(angle)*pt_posx - TMath::Sin(angle)*pt_posz;
    new_pt_posz = TMath::Sin(angle)*pt_posx + TMath::Cos(angle)*pt_posz;
    pt_posx = new_pt_posx;
    pt_posz = new_pt_posz;
}

void HitTreeCoordinateConverter::SetPosition(){
	  out_hittree->posx = pt_posx;
	  out_hittree->posy = pt_posy;
	  out_hittree->posz = pt_posz;
}

bool HitTreeCoordinateConverter::DeleteFluo(){
    if(in_hittree->num_hits > 2){
        if(in_hittree->ihit == 0){
            num_hits = in_hittree->num_hits; 
            n_deleled_hit = 0;
            for(Long64_t delta_eventnum = 0; delta_eventnum < in_hittree->num_hits; ++delta_eventnum){
                in_hittree->SetEntry(currenteventnum + delta_eventnum);
                if(TMath::Abs(in_hittree->energy - In111Constant::EneFluo) < AnalysisParameter::DeltaE){
                    --num_hits;
                }
            }
        }

        if(TMath::Abs(out_hittree->energy - In111Constant::EneFluo) < AnalysisParameter::DeltaE){
            ++n_deleled_hit;
            return 1;
        }

        out_hittree->ihit -= n_deleled_hit;
        out_hittree->num_hits = num_hits;
    }
    return 0;
}

void HitTreeCoordinateConverter::CopyValueFile(){
	  out_hittree->exptime = in_hittree->exptime;
	  out_hittree->eventid = in_hittree->eventid;
	  out_hittree->ihit = in_hittree->ihit;
	  out_hittree->num_hits = in_hittree->num_hits;
	  out_hittree->time = in_hittree->time;
	  out_hittree->instrument = in_hittree->instrument;
	  out_hittree->detector = in_hittree->detector;
	  out_hittree->det_section = in_hittree->det_section;
	  out_hittree->readout_module = in_hittree->readout_module;
	  out_hittree->section = in_hittree->section;
	  out_hittree->channel = in_hittree->channel;
	  out_hittree->pixelx = in_hittree->pixelx;
	  out_hittree->pixely = in_hittree->pixely;
	  out_hittree->rawpha = in_hittree->rawpha;
	  out_hittree->pha = in_hittree->pha;
	  out_hittree->epi = in_hittree->epi;
	  out_hittree->flag_data = in_hittree->flag_data;
	  out_hittree->flags = in_hittree->flags;
	  out_hittree->real_time = in_hittree->real_time;
	  out_hittree->time_group = in_hittree->time_group;
	  out_hittree->real_posx = in_hittree->real_posx;
	  out_hittree->real_posy = in_hittree->real_posy;
	  out_hittree->real_posz = in_hittree->real_posz;
	  out_hittree->edep = in_hittree->edep;
	  out_hittree->echarge = in_hittree->echarge;
	  out_hittree->process = in_hittree->process;
	  out_hittree->energy = in_hittree->energy;
	  out_hittree->posx = in_hittree->posx;
	  out_hittree->posy = in_hittree->posy;
	  out_hittree->posz = in_hittree->posz;
	  out_hittree->local_posx = in_hittree->local_posx;
	  out_hittree->local_posy = in_hittree->local_posy;
	  out_hittree->local_posz = in_hittree->local_posz;
	  out_hittree->grade = in_hittree->grade;
}

void HitTreeCoordinateConverter::SetPrintDivNum(Long64_t print_div_num){
    this->print_div_num = print_div_num;
}
