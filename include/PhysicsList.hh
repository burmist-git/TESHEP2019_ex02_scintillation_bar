#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;
class G4OpWLS;

class PhysicsList: public G4VUserPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();
  
protected:
  // Construct particle and physics process
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

protected:
  // These methods construct particles
  void ConstructBosons();
  void ConstructLeptons();
  void ConstructMesons();
  void ConstructBaryons();

protected:
  // Construct phyisics processes and register them
  void ConstructGeneral();
  void ConstructEM();
  void ConstructOp();

protected:
  void SetNbOfPhotonsCerenkov(G4int);
  void SetVerbose(G4int verbose);
  
private:
  G4Cerenkov* theCerenkovProcess;
  G4Scintillation* theScintillationProcess;
  G4OpAbsorption* theAbsorptionProcess;
  G4OpRayleigh* theRayleighScatteringProcess;
  G4OpMieHG* theMieHGScatteringProcess;
  G4OpBoundaryProcess* theBoundaryProcess;
  G4OpWLS* theWLSProcess;
};

#endif
