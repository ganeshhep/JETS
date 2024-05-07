#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "Pythia8/Pythia.h"
#include <vector>

int main()
{
  //defining a tree file(.root file).
  TFile *output = new TFile("tutorial5_new.root", "recreate");
  TTree *tree = new TTree("tree", "tree"); 
  
  int id, event, size, event_num, n, prev, b_hadron_mother_id;
  double charge, px, py, pz, e, pT, b_hadron_px, b_hadron_py, b_hadron_pz, b_hadron_e;
  bool is_charged, is_neutral, may_decay, isLepton, mother_b_hadron, isHadron, ishadron, isbHadron;
  std::vector<int> vec_mother_list;
  
  //defining branches of the tree file.
  tree->Branch("n", &n, "n/I");
  tree->Branch("charge", &charge, "charge/D");
  tree->Branch("is_charged", &is_charged, "is_charged/B");
  tree->Branch("is_neutral", &is_neutral, "is_neutral/B");
  tree->Branch("isbHadron", &isbHadron, "isbHadron/B");
  tree->Branch("mother_b_hadron", &mother_b_hadron, "mother_b_hadron/B");
  tree->Branch("px", &px, "px/D");
  tree->Branch("py", &py, "py/D");
  tree->Branch("pz", &pz, "pz/D");
  tree->Branch("e", &e, "e/D");
  tree->Branch("b_hadron_px", &b_hadron_px, "b_hadron_px/D");
  tree->Branch("b_hadron_py", &b_hadron_py, "b_hadron_py/D");
  tree->Branch("b_hadron_pz", &b_hadron_pz, "b_hadron_pz/D");
  tree->Branch("b_hadron_e", &b_hadron_e, "b_hadron_e/D");

  int nevents = 5e4;
  Pythia8::Pythia pythia;      //every event is a proton-proton collision. 
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212"); 
  pythia.readString("Beams:eCM = 14.e3");
  pythia.readString("Top:gg2ttbar = on");
  pythia.readString("Top:qqbar2ttbar = on");
  pythia.readString("Top:ffbar2ttbar(s:gmZ) = on");
  pythia.readString("Top:gmgm2ttbar = on");
  pythia.readString("Top:ggm2ttbar = on");
  
  pythia.init();

  n = 0;
  prev = -1;
  for (int i = 0; i < nevents; i++)
  {
    if (!pythia.next()) continue;
 
    int entries = pythia.event.size(); //number of particles in an event.
    
    std::cout << "Event: " << i << std::endl;
    
    event = i;
    size = entries;
    for (int j = 0; j < entries; j++)
    {
      may_decay = pythia.event[j].mayDecay();
      isLepton = pythia.event[j].isLepton();
      isHadron = pythia.event[j].isHadron();
      pT = sqrt(pow(pythia.event[j].px(), 2) + pow(pythia.event[j].py(), 2));

      if (may_decay == 0 && isLepton == 0 && pT > 1){
          event_num = i;

          if (event_num != prev){
	    prev = event_num;
            n++;
          } else {
	        n = n;
	  }
	    
          id = pythia.event[j].id();
          charge = pythia.event[j].charge();
          is_charged = pythia.event[j].isCharged();
          is_neutral = pythia.event[j].isNeutral();
          isbHadron = 0;
          mother_b_hadron = 0;

          px = pythia.event[j].px();
          py = pythia.event[j].py();
          pz = pythia.event[j].pz();
          e = pythia.event[j].e();

          b_hadron_px = 0;
          b_hadron_py = 0;
          b_hadron_pz = 0;
          b_hadron_e = 0;

          if (isHadron == 1){
            if (pythia.event[pythia.event[j].mother1()].id() == 5){
	      isbHadron = 1;
            }
          }

	  vec_mother_list = pythia.event[j].motherList();

          for (int k = 0; k < vec_mother_list.size(); k++)
	    {
              ishadron = pythia.event[vec_mother_list.at(k)].isHadron();
	      if (ishadron == 1){
                b_hadron_mother_id = pythia.event[pythia.event[vec_mother_list.at(k)].mother1()].id();
                if (b_hadron_mother_id == 5){
                   b_hadron_px = pythia.event[vec_mother_list.at(k)].px();
                   b_hadron_py = pythia.event[vec_mother_list.at(k)].py();
                   b_hadron_pz = pythia.event[vec_mother_list.at(k)].pz();
                   b_hadron_e = pythia.event[vec_mother_list.at(k)].e();
                   mother_b_hadron = 1; 
                   break;
		}
	      }
            }
          
          tree->Fill();
      }

    }
   
  }  
  
  output->Write();
  output->Close();

  return 0;

}
