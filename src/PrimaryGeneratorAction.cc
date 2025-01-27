//my
#include "PrimaryGeneratorAction.hh"
#include "VolumeStructures.hh"

//G4
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"

//root
#include "TMath.h"
#include "TRandom3.h"
#include "TVector3.h"

PrimaryGeneratorAction::PrimaryGeneratorAction() :
  _particleGun(0),
  _particleName("pi+"),
  _particleMomentum(3.0*GeV),
  _PhiAngle(0.0*deg),
  _ThetaAngle(0.0*deg),
  _singlePhoton(false)
{
  _particleGun = new G4ParticleGun(1);  
  _BunchXID = 0;
  _rnd = new TRandom3(123123);
  //backGen = new backgroundGen();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete _particleGun;
  //delete backGen;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle;
  // Correct for center of bar
  G4double xInit, yInit, zInit;
  G4double dX, dY, dZ;
  G4double Ekin, m;
  //G4int pdgID;
  //G4int i;
  //i = 0;

  //_particleMomentum = 7000.0*GeV;
  //_particleMomentum = 20.0*GeV;

  G4double  barLength = 110.0*cm;
  G4double  barBase = 3.3*cm;
  G4double  barHoleR = 1.5*mm;

  G4double  fiberLength = barLength + 5.0*cm;
  //G4double  fiberLength = barLength;
  G4double  fiberOuterR = 0.6*mm;
  G4double  claddThickness = 0.05*mm;

  G4double sensitive_sizeX = 6.0*mm;
  G4double sensitive_sizeY = 6.0*mm;
  G4double sensitive_sizeZ = 1.0*mm;

  G4double barCoating_base_sizeX = barBase;
  G4double barCoating_base_sizeY = 0.2*mm;
  G4double barCoating_base_sizeZ = barLength;

  G4double barCoating_side_sizeX = barBase/2.0/TMath::Cos(45.0*deg);
  G4double barCoating_side_sizeY = 0.2*mm;
  G4double barCoating_side_sizeZ = barLength;

  G4double world_sizeX = barBase + 20.0*cm;
  G4double world_sizeY = barBase + 10.0*cm;
  G4double world_sizeZ = fiberLength + sensitive_sizeZ*2.0 + 5.0*cm;
  
  //xInit = 0.0*cm;
  //yInit = -world_sizeY/2.0 + 50.0*mm;
  //zInit = 0.0*cm;

  xInit = _rnd->Uniform(-barBase/2.0,barBase/2.0);
  yInit =  world_sizeY/2.0 - 40.0*mm;
  zInit = _rnd->Uniform(-barLength/2.0,barLength/2.0);
  
  ///////////////////////
  _BunchXID++;
  particle = particleTable->FindParticle(_particleName);
  m = particle->GetPDGMass();
  Ekin = (TMath::Sqrt(_particleMomentum*_particleMomentum + m*m) - m);

  //G4cout<<"_particleMomentum = "<<_particleMomentum<<G4endl
  //	<<"_ThetaAngle       = "<<_ThetaAngle*180.0/TMath::Pi()<<G4endl
  //	<<"_PhiAngle         = "<<_PhiAngle*180.0/TMath::Pi()<<G4endl;

  _ThetaAngle = _ThetaAngle + (-1.0 + 2*G4UniformRand())*2*TMath::Pi()/360.0;//mearing of one degree
  _PhiAngle   = _PhiAngle   + (-1.0 + 2*G4UniformRand())*2*TMath::Pi()/360.0;//mearing of one degree

  generateThetaAndPhi();
  
  dX =  TMath::Sin(_ThetaAngle)*TMath::Cos(_PhiAngle);
  dY =  TMath::Sin(_ThetaAngle)*TMath::Sin(_PhiAngle);
  dZ =  TMath::Cos(_ThetaAngle);
  //
  //G4cout<<"_ThetaAngle "<<_ThetaAngle<<G4endl
  //	<<"_PhiAngle   "<<_PhiAngle<<G4endl;  
  TVector3 *v_dxyz = new TVector3(dX,dY,dZ);
  v_dxyz->RotateX(90*deg);
  //
  //G4cout<<"dX "<<dX<<G4endl
  //	<<"dY "<<dY<<G4endl
  //	<<"dZ "<<dZ<<G4endl;
  //G4cout<<"v_dxyz->x() "<<v_dxyz->x()<<G4endl
  //	<<"v_dxyz->y() "<<v_dxyz->y()<<G4endl
  //	<<"v_dxyz->z() "<<v_dxyz->z()<<G4endl;
  //  
  G4ThreeVector dir(v_dxyz->x(), v_dxyz->y(), v_dxyz->z());
  _particleGun->SetParticleDefinition(particle);
  _particleGun->SetParticleMomentumDirection(dir);
  _particleGun->SetParticleEnergy(Ekin);  
  _particleGun->SetParticlePosition(G4ThreeVector(xInit, yInit, zInit));
  //for(int i = 0;i<10;i++)
  _particleGun->GeneratePrimaryVertex(anEvent);

}

G4int PrimaryGeneratorAction::GenFlatInt(G4int iMin,G4int iMax){
  G4int val;
  val = (G4int)floor((iMax - iMin + 1)*G4UniformRand() + iMin);
  return val;
}

void PrimaryGeneratorAction::generateThetaAndPhi(){
  _PhiAngle = G4UniformRand()*2*TMath::Pi();
  _ThetaAngle = genCos2dist();
}

G4double PrimaryGeneratorAction::genCos2dist(){
  G4double theta = -999.0;//deg 
  G4double x = -999.0;
  G4double y = -999.0;
  while(theta==-999.0){
    x = G4UniformRand()*(70.0*TMath::Pi()/180.0); //rad
    y = G4UniformRand();
    if(TMath::Power(TMath::Cos(x),1.85)>y){
      theta = x;
    }
  }  
  return theta;
}
