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
  
  int id, event, size, event_num;
  int bCodes[28] = {511, 521, 531, 541, 5122, 5132, 5142, 5232, 5242, 5332, 5342, 5412, 5414, 5422, 5424, 5432, 5434, 5442, 5444, 5512, 5514, 5522, 5524, 5532, 5534, 5542, 5544, 5544};
  double px, py, pz, e, pT;
  bool may_decay, isLepton, isbHadron;
  std::vector<int> vec_daughter_list;
  
  //defining branches of the tree file.
  tree->Branch("event", &event, "event/I");
  tree->Branch("may_decay", &may_decay, "may_decay/B");
  tree->Branch("isLepton", &isLepton, "isLepton/B");
  tree->Branch("isbHadron", &isbHadron, "isbHadron/B");
  tree->Branch("px", &px, "px/D");
  tree->Branch("py", &py, "py/D");
  tree->Branch("pz", &pz, "pz/D");
  tree->Branch("e", &e, "e/D");
  tree->Branch("pT", &pT, "pT/D");

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
      
      id = pythia.event[j].id();

      px = pythia.event[j].px();
      py = pythia.event[j].py();
      pz = pythia.event[j].pz();
      e = pythia.event[j].e();

      vec_daughter_list = pythia.event[j].daughterList();

      pT = sqrt(pow(pythia.event[j].px(), 2) + pow(pythia.event[j].py(), 2));

      isbHadron = 0;

      for (int k = 0; k < 28; k++){
        if (id == bCodes[k]){
          isbHadron = 1;
          for (int l = 0; l < vec_daughter_list.size(); l++){
	    int daughter_id = pythia.event[vec_daughter_list.at(l)].id();
            for (int m = 0; m < 28; m++){
	      if (daughter_id == bCodes[m]){
		isbHadron = 0;
	      }
	    }
	  }
        }
      }
   
      tree->Fill();

    }
   
  }  
  
  output->Write();
  output->Close();

  return 0;

}
