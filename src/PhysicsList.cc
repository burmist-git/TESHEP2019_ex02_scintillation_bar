﻿#include "G4ios.hh"
#include "globals.hh"
#include "PhysicsList.hh"
#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"

// Cerenkov and optical physics processes
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

// WLS physics
#include "G4OpWLS.hh"

using namespace CLHEP;

PhysicsList::PhysicsList() : G4VUserPhysicsList()
{
  theCerenkovProcess = 0;
  theAbsorptionProcess = 0;
  theRayleighScatteringProcess = 0;
  theBoundaryProcess = 0;
  defaultCutValue = 1.0*mm;
  SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{;}

void PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 
  
  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
}

void PhysicsList::ConstructBosons()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  
  // Gamma
  G4Gamma::GammaDefinition();
  
  // Optical Photons
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void PhysicsList::ConstructLeptons()
{
  // Leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void PhysicsList::ConstructMesons()
{
  // Mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4Eta::EtaDefinition();
  G4EtaPrime::EtaPrimeDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4KaonZero::KaonZeroDefinition();
  G4AntiKaonZero::AntiKaonZeroDefinition();
  G4KaonZeroLong::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();
}

void PhysicsList::ConstructBaryons()
{
  // Baryons
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
}

void PhysicsList::ConstructProcess()
{
  // Define transportation process
  
  AddTransportation();
  ConstructEM();
  ConstructGeneral();
  ConstructOp();
}


#include "G4GammaConversion.hh"
#include "G4ComptonScattering.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
//#include "G4MultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

#include "G4StepLimiter.hh"
#include "G4PhysicsListHelper.hh"

void PhysicsList::ConstructEM()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper(); 
 
  GetParticleIterator()->reset();
  
  while( (*GetParticleIterator())() ) {
    G4ParticleDefinition* particle = GetParticleIterator()->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "gamma") {
      // Gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
      //pmanager->AddDiscreteProcess(new G4StepLimiter()); 
      ph->RegisterProcess(new G4StepLimiter,         particle); 
   }
    else if (particleName == "e-") {
      //Electron
      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(), -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(), -1, 3, 3);
      //pmanager->AddProcess(new G4StepLimiter()); 
      ph->RegisterProcess(new G4StepLimiter,         particle);
    }
    else if (particleName == "e+") {
      // Positron
      // pmanager->AddProcess(new G4MultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(), -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(), -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation(), 0, -1, 4);
      //pmanager->AddProcess(new G4StepLimiter()); 
      ph->RegisterProcess(new G4StepLimiter,         particle);
    }
    else if (particleName == "mu+" || particleName == "mu-") {
      // Muon
      //pmanager->AddProcess(new G4MultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4MuMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation(), -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung(), -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction(), -1, -1, 4);
      //pmanager->AddProcess(new G4StepLimiter()); 
      ph->RegisterProcess(new G4StepLimiter,         particle);
    }
    else if ((!particle->IsShortLived()) &&
	     (particle->GetPDGCharge() != 0.) &&
	     (particle->GetParticleName() != "chargedgeantino")) {
      // All other charged particles except geantino
      //pmanager->AddProcess(new G4MultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4hMultipleScattering(), -1, 1, 1);
      // For some reason, this is causing a bus error!
      pmanager->AddProcess(new G4hIonisation(), -1, 2, 2);
      //pmanager->AddProcess(new G4StepLimiter());
      ph->RegisterProcess(new G4StepLimiter,         particle);
    }
  }
}



#include "G4Decay.hh"

void PhysicsList::ConstructGeneral()
{
  
  // Add decay process
  G4Decay* theDecayProcess = new G4Decay();
  GetParticleIterator()->reset();
  while( (*GetParticleIterator())() ) {
    G4ParticleDefinition* particle = GetParticleIterator()->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theDecayProcess);
    }
  }
}

void PhysicsList::ConstructOp()
{

  theWLSProcess = new G4OpWLS(); 
  
  //G4cout<<" 000 "<<G4endl;
  // Optical Photon Processes
  theCerenkovProcess = new G4Cerenkov("Cerenkov");
  //G4cout<<" 111 "<<G4endl;
  //theCerenkovProcess->DumpPhysicsTable();
  //G4cout<<" 222 "<<G4endl;
  SetVerbose(1);
  //G4cout<<" 333 "<<G4endl;
  theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
  //theCerenkovProcess->SetMaxNumPhotonsPerStep(1);
  theCerenkovProcess->SetTrackSecondariesFirst(true);
  //
  theScintillationProcess = new G4Scintillation("Scintillation");
  theScintillationProcess->SetScintillationYieldFactor(1.);
  theScintillationProcess->SetTrackSecondariesFirst(true);
  //
  theMieHGScatteringProcess = new G4OpMieHG();
  //
  theBoundaryProcess = new G4OpBoundaryProcess();
  //G4OpticalSurfaceModel theModel = unified;
  //theBoundaryProcess->SetModel(theModel);
  //G4cout<<" 111 "<<G4endl;

  // Use Birks Correction in the Scintillation process
  if(G4Threading::IsMasterThread())
  {
    G4EmSaturation* emSaturation =
              G4LossTableManager::Instance()->EmSaturation();
      theScintillationProcess->AddSaturation(emSaturation);
  }


  theWLSProcess->UseTimeProfile("delta");
  //theWLSProcess->UseTimeProfile("exponential");
  G4ProcessManager* pManager_opt =
                G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
  pManager_opt->AddDiscreteProcess(theWLSProcess);
  
  GetParticleIterator()->reset();
  while( (*GetParticleIterator())() ) {
    G4ParticleDefinition* particle = GetParticleIterator()->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (theCerenkovProcess->IsApplicable(*particle)) {
      G4cout << "Add Cerenkov process to " << particleName << G4endl;
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess, idxPostStep);
    }
    if (theScintillationProcess->IsApplicable(*particle)) {
      G4cout << "Add Scintillation process to " << particleName << G4endl;
      pmanager->AddProcess(theScintillationProcess);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(new G4OpAbsorption());
      pmanager->AddDiscreteProcess(new G4OpRayleigh());
      pmanager->AddDiscreteProcess(theMieHGScatteringProcess);
      pmanager->AddDiscreteProcess(theBoundaryProcess);
    }
  }
}

void PhysicsList::SetCuts()
{

  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
  SetCutsWithDefault();   

  if (verboseLevel > 0) {
    G4cout << "PhyiscsList::SetCuts:";
    G4cout << "CutLength:" << G4BestUnit(defaultCutValue, "Length") << G4endl;
    
    DumpCutValuesTable();
  }

}

void PhysicsList::SetVerbose(G4int verbose)
{
    theCerenkovProcess->SetVerboseLevel(verbose);
  //  theAbsorptionProcess->SetVerboseLevel(verbose);
  //  theRayleighScatteringProcess->SetVerboseLevel(verbose);
  //  theBoundaryProcess->SetVerboseLevel(verbose);
}  

void PhysicsList::SetNbOfPhotonsCerenkov(G4int MaxNumber)
{
  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumber);
}
