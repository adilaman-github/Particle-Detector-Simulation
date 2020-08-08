
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "Analysis.hh"


EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fTlen(0.),
  fEdep(0.)
{} 


EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
  fTlen = 0.;
}


void EventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  fRunAction->AddTlen(fTlen);

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // Fill histograms
  analysisManager->FillH1(0, fEdep);
  //analysisManager->FillH1(1, fTlen);
}

