#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "Pythia8/Pythia.h"

int main()
{
  //defining a tree file(.root file).
  TFile *output = new TFile("tutorial5.root", "recreate");
  TTree *tree = new TTree("tree", "tree"); 
  
  int id, event, size, no;
  double charge, m, px, py, pz, e, pT;
  bool is_charged, is_neutral, may_decay, isLepton;
  
  //defining branches of the tree file.
  tree->Branch("event", &event, "event/I");
  tree->Branch("size", &size, "size/I");
  tree->Branch("no", &no, "no/I");
  tree->Branch("id", &id, "id/I");
  tree->Branch("charge", &charge, "charge/D");
  tree->Branch("is_charged", &is_charged, "is_charged/B");
  tree->Branch("is_neutral", &is_neutral, "is_neutral/B");
  tree->Branch("may_decay", &may_decay, "may_decay/B");
  tree->Branch("isLepton", &isLepton, "isLepton/B");
  tree->Branch("m", &m, "m/D");
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
      //a particle informations we want to get.
      no = j;
      id = pythia.event[j].id();
      charge = pythia.event[j].charge();
      is_charged = pythia.event[j].isCharged();
      is_neutral = pythia.event[j].isNeutral();
      may_decay = pythia.event[j].mayDecay();
      isLepton = pythia.event[j].isLepton();
      m = pythia.event[j].m();
      px = pythia.event[j].px();
      py = pythia.event[j].py();
      pz = pythia.event[j].pz();
      e = pythia.event[j].e();
      pT = sqrt(pow(px, 2) + pow(py, 2));

      tree->Fill();
      
    }  
  }  
  
  output->Write();
  output->Close();

  return 0;

}
