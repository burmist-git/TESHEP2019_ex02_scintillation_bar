//My
#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

//G4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Torus.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4Para.hh"
#include "G4Paraboloid.hh"
#include "G4EllipticalTube.hh"
#include "G4ExtrudedSolid.hh"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Color.hh"
#include "G4TwoVector.hh"
#include "G4SDManager.hh"
#include "globals.hh"
//magnetic field
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4UserLimits.hh"
//GDML
//#include <G4GDMLParser.hh>

//root 
#include "TMath.h"

DetectorConstruction::DetectorConstruction()
{
  //magField = new MagneticField();
  worldVisAtt = new G4VisAttributes();
  quartzVisAtt = new G4VisAttributes();
  aerogelVisAtt = new G4VisAttributes();
  sensitiveVisAtt = new G4VisAttributes();
  pmtboxVisAtt = new G4VisAttributes();
  absVisAtt = new G4VisAttributes();
  // Define Materials to be used
  DefineMaterials();
}

DetectorConstruction::~DetectorConstruction()
{
  //delete magField;
  delete worldVisAtt;
  delete quartzVisAtt;
  delete sensitiveVisAtt;
  delete pmtboxVisAtt;
  delete absVisAtt;
  delete stepLimit;
}

void DetectorConstruction::DefineMaterials()
{
  G4String symbol;
  G4double a, z, density;
  G4int ncomponents, natoms;
  G4double fractionmass;

  // Define elements
  G4Element* H = 
    new G4Element("Hydrogen", symbol = "H", z = 1., a = 1.01*g/mole);
  G4Element* C = 
    new G4Element("Carbon",   symbol = "C", z = 6., a = 12.01*g/mole);
  G4Element* N = 
    new G4Element("Nitrogen", symbol = "N", z = 7., a = 14.01*g/mole);
  G4Element* O =
    new G4Element("Oxygen",   symbol = "O", z = 8., a = 16.00*g/mole);
  G4Element* Si = 
    new G4Element("Silicon",  symbol = "Si", z = 14., a = 28.09*g/mole);
  G4Element* Al = 
    new G4Element("Aluminum", symbol = "Al", z = 13., a = 26.98*g/mole);
  G4Element* F = 
    new G4Element("Fluorine", symbol = "F", z = 9., a = 19.0*g/mole);
  G4Element* Ti = 
    new G4Element("Titanium", symbol = "Ti", z = 22., a = 47.87*g/mole);

  //
  polystyrene_scint = new G4Material("polystyrene_scint", density = 1.200*g/cm3, ncomponents = 2);
  polystyrene_scint->AddElement(H, natoms = 1);
  polystyrene_scint->AddElement(C, natoms = 2);

  //
  polystyrene_fibre = new G4Material("polystyrene_fibre", density = 1.200*g/cm3, ncomponents = 2);
  polystyrene_fibre->AddElement(H, natoms = 1);
  polystyrene_fibre->AddElement(C, natoms = 2);

  //
  acrylic_cladding = new G4Material("acrylic_cladding", density = 1.200*g/cm3, ncomponents = 2);
  acrylic_cladding->AddElement(H, natoms = 1);
  acrylic_cladding->AddElement(C, natoms = 2);

  //
  glue = new G4Material("glue", density = 1.200*g/cm3, ncomponents = 2);
  glue->AddElement(H, natoms = 1);
  glue->AddElement(C, natoms = 2);
  
  // Quartz Material (SiO2_cladd)
  SiO2_cladd = new G4Material("quartzCladd", density = 2.200*g/cm3, ncomponents = 2);
  SiO2_cladd->AddElement(Si, natoms = 1);
  SiO2_cladd->AddElement(O , natoms = 2);

  // Quartz Material (SiO2_coat)
  SiO2_coat = new G4Material("quartzCoat", density = 2.200*g/cm3, ncomponents = 2);
  SiO2_coat->AddElement(Si, natoms = 1);
  SiO2_coat->AddElement(O , natoms = 2);

  // Quartz Material (SiO2)
  SiO2 = new G4Material("quartz", density = 2.200*g/cm3, ncomponents = 2);
  SiO2->AddElement(Si, natoms = 1);
  SiO2->AddElement(O , natoms = 2);

  // Aerogel Material (SiO2)
  Aerogel = new G4Material("Aerogel", density = 2000*g/m3, ncomponents = 2);
  Aerogel->AddElement(Si, natoms = 1);
  Aerogel->AddElement(O , natoms = 2);

  // C4F10
  C4F10 = new G4Material("fluorocarbon", density = 1.8/1000*g/cm3, ncomponents = 2);
  C4F10->AddElement(C, natoms = 4);
  C4F10->AddElement(F, natoms = 10);

  // Air
  Air = new G4Material("Air", density = 1.290*mg/cm3, ncomponents = 2);
  Air->AddElement(N, fractionmass = 0.7);
  Air->AddElement(O, fractionmass = 0.3);

  // Aluminum
  Aluminum = new G4Material("Aluminum", density = 2.7*g/cm3, ncomponents = 1);
  Aluminum->AddElement(Al, fractionmass = 1.0);

  // Aluminum of the mirror
  AluminumMirr = new G4Material("mirrAluminum", density = 2.7*g/cm3, ncomponents = 1);
  AluminumMirr->AddElement(Al, fractionmass = 1.0);

  //
  TiO2 = new G4Material("TiO2", density = 5.0*g/cm3, ncomponents = 2);
  TiO2->AddElement(Ti, natoms = 1);
  TiO2->AddElement(O, natoms = 2);
  
  /*
  // Assign Materials
  world.material = Air;
  secA.material = SiO2;
  secB.material = SiO2;
  secC.material = SiO2;
  secWin.material = SiO2;
  fiberCorr.material = SiO2;
  fiberClad.material = SiO2_cladd;
  //fiberCoat.material = SiO2_coat;
  fiberCoat.material = Aluminum;
  sensitive.material = Aluminum;
  abs1.material = Aluminum;
  */

  //
  // Generate and Add Material Properties Table
  //						
  const G4int num = 36;
  G4double WaveLength[num];
  G4double Absorption[num];      // Default value for absorption
  G4double AirAbsorption[num];
  G4double AirRefractiveIndex[num];
  G4double PhotonEnergy[num];

  // Absorption of quartz per 1m
  G4double QuartzAbsorption[num] =
    {0.999572036,0.999544661,0.999515062,0.999483019,0.999448285,
     0.999410586,0.999369611,0.999325013,0.999276402,0.999223336,
     0.999165317,0.999101778,0.999032079,0.998955488,0.998871172,
     0.998778177,0.99867541 ,0.998561611,0.998435332,0.998294892,
     0.998138345,0.997963425,0.997767484,0.997547418,0.99729958 ,
     0.99701966 ,0.99670255 ,0.996342167,0.995931242,0.995461041,
     0.994921022,0.994298396,0.993577567,0.992739402,0.991760297,
     0.990610945};

  G4double C4F10RefractiveIndex[num];
  G4double AerogelRefractiveIndex[num];

  for (int i=0; i<num; i++) {
    WaveLength[i] = (300 + i*10)*nanometer;
    // Aerogel and C4F10
    Absorption[i] = 100*m;
    // Air
    AirAbsorption[i] = 4.*cm;
    AirRefractiveIndex[i] = 1.;
    // C4F10
    C4F10RefractiveIndex[i] = 1.0014;
    // Aerogel
    AerogelRefractiveIndex[i] = 1.03;
    PhotonEnergy[num - (i+1)] = twopi*hbarc/WaveLength[i];
    // Absorption is given per length and G4 needs mean free path
    // length, calculate it here
    // mean free path length - taken as probablility equal 1/e
    // that the photon will be absorbed
    QuartzAbsorption[i] = (-1)/log(QuartzAbsorption[i])*100*cm;
    //EpotekAbsorption[i] = (-1)/log(EpotekAbsorption[i])*
    //epotekBarJoint.thickness;
  }

  G4double QuartzRefractiveIndex[num] =
    {1.456535,1.456812,1.4571  ,1.457399,1.457712,1.458038,
     1.458378,1.458735,1.459108,1.4595  ,1.459911,1.460344,
     1.460799,1.46128 ,1.461789,1.462326,1.462897,1.463502,
     1.464146,1.464833,1.465566,1.46635 ,1.46719 ,1.468094,
     1.469066,1.470116,1.471252,1.472485,1.473826,1.475289,
     1.476891,1.478651,1.480592,1.482739,1.485127,1.487793};

  G4double CladdingRefractiveIndex[num];

  for(int i=0; i<num; i++){
    CladdingRefractiveIndex[i] = TMath::Sqrt(QuartzRefractiveIndex[i]*QuartzRefractiveIndex[i]-0.58*0.58);
  }

  // Assign absorption and refraction to materials : polystyrene_scint
  // Quartz
  G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
  QuartzMPT->AddProperty("RINDEX", PhotonEnergy, QuartzRefractiveIndex, num);
  QuartzMPT->AddProperty("ABSLENGTH", PhotonEnergy, QuartzAbsorption, num);

  // C4F10
  G4MaterialPropertiesTable* C4F10MPT = new G4MaterialPropertiesTable();
  C4F10MPT->AddProperty("RINDEX", PhotonEnergy, C4F10RefractiveIndex, num);
  C4F10MPT->AddProperty("ABSLENGTH", PhotonEnergy, Absorption, num);

  // Aerogel
  G4MaterialPropertiesTable* AerogelMPT = new G4MaterialPropertiesTable();
  AerogelMPT->AddProperty("RINDEX", PhotonEnergy, AerogelRefractiveIndex, num);
  AerogelMPT->AddProperty("ABSLENGTH", PhotonEnergy, Absorption, num);
  
  // Cladding (of the fiber) only for the fiber aplication
  G4MaterialPropertiesTable* CladdingMPT = new G4MaterialPropertiesTable();
  CladdingMPT->AddProperty("RINDEX", PhotonEnergy, CladdingRefractiveIndex, num);
  CladdingMPT->AddProperty("ABSLENGTH", PhotonEnergy, QuartzAbsorption, num);

  // Air
  G4MaterialPropertiesTable* AirMPT = new G4MaterialPropertiesTable();
  AirMPT->AddProperty("RINDEX", PhotonEnergy, AirRefractiveIndex, num);
  AirMPT->AddProperty("ABSLENGTH", PhotonEnergy, AirAbsorption, num);

  // Assign this material to the bars
  SiO2->SetMaterialPropertiesTable(QuartzMPT);
  SiO2_cladd->SetMaterialPropertiesTable(CladdingMPT);
  C4F10->SetMaterialPropertiesTable(C4F10MPT);
  Aerogel->SetMaterialPropertiesTable(AerogelMPT);
  Air->SetMaterialPropertiesTable(AirMPT);

  //
  // ------------ Generate & Add Material Properties Table ------------
  //
  G4double photonEnergy_scint[] =
    { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
      2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
      2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
      2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
      2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
      3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
      3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
      3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };
  
  const G4int nEntries_scint = sizeof(photonEnergy_scint)/sizeof(G4double);
  
  //
  // Scint
  //
  G4double refractiveIndex1_scint[] =
    { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
      1.346,  1.3465, 1.347,  1.3475, 1.348,
      1.3485, 1.3492, 1.35,   1.3505, 1.351,
      1.3518, 1.3522, 1.3530, 1.3535, 1.354,
      1.3545, 1.355,  1.3555, 1.356,  1.3568,
      1.3572, 1.358,  1.3585, 1.359,  1.3595,
      1.36,   1.3608};

  assert(sizeof(refractiveIndex1_scint) == sizeof(photonEnergy_scint));

  G4double absorption_scint[] =
    {3.448*cm,  4.082*cm,  6.329*cm,  9.174*cm, 12.346*cm, 13.889*cm,
     15.152*cm, 17.241*cm, 18.868*cm, 20.000*cm, 26.316*cm, 35.714*cm,
     45.455*cm, 47.619*cm, 52.632*cm, 52.632*cm, 55.556*cm, 52.632*cm,
     52.632*cm, 47.619*cm, 45.455*cm, 41.667*cm, 37.037*cm, 33.333*cm,
     30.000*cm, 28.500*cm, 27.000*cm, 24.500*cm, 22.000*cm, 19.500*cm,
     17.500*cm, 14.500*cm };

  G4double attenuationabs = 0.3;
  //G4double attenuationabs = 6.0;
  for(Int_t ii = 0;ii<nEntries_scint;ii++)
    absorption_scint[ii] = absorption_scint[ii]*attenuationabs;
    
  assert(sizeof(absorption_scint) == sizeof(photonEnergy_scint));
  
  G4double scintilFast_scint[] =
    { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00 };
  
  assert(sizeof(scintilFast_scint) == sizeof(photonEnergy_scint));
  
  G4double scintilSlow_scint[] =
    { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
      7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
      3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
      4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
      7.00, 6.00, 5.00, 4.00 };
  
  assert(sizeof(scintilSlow_scint) == sizeof(photonEnergy_scint));

  G4MaterialPropertiesTable* myMPT1_scint = new G4MaterialPropertiesTable();
  
  myMPT1_scint->AddProperty("RINDEX",       photonEnergy_scint, refractiveIndex1_scint,nEntries_scint)
              ->SetSpline(true);
  myMPT1_scint->AddProperty("ABSLENGTH",    photonEnergy_scint, absorption_scint,     nEntries_scint)
              ->SetSpline(true);
  myMPT1_scint->AddProperty("FASTCOMPONENT",photonEnergy_scint, scintilFast_scint,     nEntries_scint)
              ->SetSpline(true);
  myMPT1_scint->AddProperty("SLOWCOMPONENT",photonEnergy_scint, scintilSlow_scint,     nEntries_scint)
              ->SetSpline(true);

  myMPT1_scint->AddConstProperty("SCINTILLATIONYIELD",50./MeV);
  myMPT1_scint->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT1_scint->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT1_scint->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT1_scint->AddConstProperty("YIELDRATIO",0.8);

  G4cout << "myMPT1_scint : G4MaterialPropertiesTable" << G4endl;
  myMPT1_scint->DumpTable();

  polystyrene_scint->SetMaterialPropertiesTable(myMPT1_scint);
  glue->SetMaterialPropertiesTable(myMPT1_scint);


  //--------------------------------------------------
  //  PMMA for WLSfibers
  //--------------------------------------------------

  G4double photonEnergy_wls[] =
  {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  const G4int nEntries_wls = sizeof(photonEnergy_wls)/sizeof(G4double);
  
  G4double refractiveIndexWLSfiber[] =
  { 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60};

  assert(sizeof(refractiveIndexWLSfiber) == sizeof(photonEnergy_wls));

  G4double absWLSfiber[] =
  {5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,1.10*m,
   1.10*m,1.10*m,1.10*m,1.10*m,1.10*m,1.10*m, 1.*mm, 1.*mm, 1.*mm, 1.*mm,
    1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm};

  assert(sizeof(absWLSfiber) == sizeof(photonEnergy_wls));

  G4double emissionFib[] =
  {0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
   3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
   12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
   15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
   0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};

  assert(sizeof(emissionFib) == sizeof(photonEnergy_wls));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptWLSfiber = new G4MaterialPropertiesTable();
  mptWLSfiber->
           AddProperty("RINDEX",photonEnergy_wls,refractiveIndexWLSfiber,nEntries_wls);
  // mptWLSfiber->AddProperty("ABSLENGTH",photonEnergy_wls,absWLSfiber,nEntries_wls);
  mptWLSfiber->AddProperty("WLSABSLENGTH",photonEnergy_wls,absWLSfiber,nEntries_wls);
  mptWLSfiber->AddProperty("WLSCOMPONENT",photonEnergy_wls,emissionFib,nEntries_wls);
  mptWLSfiber->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);

  polystyrene_fibre->SetMaterialPropertiesTable(mptWLSfiber);

  
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

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
  
  G4RotationMatrix Ra;
  G4ThreeVector Ta;
  G4Transform3D Tr;

  // 
  // Define World Volume
  //
  G4VSolid *world_solid = new G4Box("World",world_sizeX/2.0,world_sizeY/2.0,world_sizeZ/2.0);
  G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid,Air,"World");
  G4VPhysicalVolume *world_physical = new G4PVPlacement(0,G4ThreeVector(),world_logical,"World",0,false,0);

  //
  // Sensitive volume
  //
  G4VSolid *sensitive_solid = new G4Box("Sensitive", sensitive_sizeX/2.0, sensitive_sizeY/2.0, sensitive_sizeZ/2.0);
  G4LogicalVolume *sensitive_logical = new G4LogicalVolume(sensitive_solid, Aluminum,"Sensitive");
  Ta.setX(0.0);
  Ta.setY(+barBase/4);
  Ta.setZ(fiberLength/2.0+sensitive_sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  new G4PVPlacement(Tr,                //Transformation
		    sensitive_logical, //its logical volume				 
		    "Sensitive",       //its name
		    world_logical,     //its mother volume
		    false,	       //no boolean operation
		    0);	               //copy number
  //
  Ta.setX(0.0);
  Ta.setY(+barBase/4);
  Ta.setZ(-fiberLength/2.0-sensitive_sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  new G4PVPlacement(Tr,                //Transformation
		    sensitive_logical, //its logical volume				 
		    "Sensitive",       //its name
		    world_logical,     //its mother volume
		    false,	       //no boolean operation
		    0);	               //copy number
  
  
  //
  // Scintillator with hole
  //
  // Tringle volume
  G4VSolid* solidScintillatorBox = new G4Box("solidScintillatorBox", barBase/2/TMath::Sqrt(2.0), barBase/2/TMath::Sqrt(2.0), barLength/2);
  // round hole
  G4VSolid *solidHole = new G4Tubs("solidHole", 0.0, barHoleR, barLength, 0, 360.0*deg);
  // box
  G4VSolid *soliBox = new G4Box("soliBox", barBase/2, barBase/2, barLength);

  G4RotationMatrix Ra_sub;
  G4ThreeVector Ta_sub;
  G4Transform3D Tr_sub;
  Ta_sub.setX(+barBase/4*TMath::Cos(TMath::Pi()/4.0));
  Ta_sub.setY(+barBase/4*TMath::Sin(TMath::Pi()/4.0));
  Ta_sub.setZ(0.0);
  //Ra_sub.rotateY(90.0*deg);
  Tr_sub = G4Transform3D(Ra_sub, Ta_sub);
  G4SubtractionSolid* solidScintillatorHoleBox = new G4SubtractionSolid("solidScintillatorHoleBox", solidScintillatorBox, solidHole, Tr_sub);
  //Ra_sub.rotateY(-90.0*deg);

  Ta_sub.setX(-barBase/2*TMath::Cos(TMath::Pi()/4.0));
  Ta_sub.setY(-barBase/2*TMath::Sin(TMath::Pi()/4.0));
  Ta_sub.setZ(0.0);
  Ra_sub.rotateZ(45.0*deg);
  Tr_sub = G4Transform3D(Ra_sub, Ta_sub);
  G4SubtractionSolid* solidScintillatorHole = new G4SubtractionSolid("solidScintillatorHoleBox", solidScintillatorHoleBox, soliBox, Tr_sub);
  
  G4LogicalVolume* logicScintillatorHole = new G4LogicalVolume( solidScintillatorHole, polystyrene_scint, "logicScintillatorHole");
  Ta.setX(0.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Ra.rotateZ(45*deg);
  Tr = G4Transform3D(Ra, Ta);
  new G4PVPlacement(Tr,                    //Transformation
		    logicScintillatorHole, //its logical volume				 
		    "scintillatorHole",    //its name
		    world_logical,         //its mother  volume
		    false,                 //no boolean operation
		    0);	                   //copy number
  Ra.rotateZ(-45*deg);

  //
  // Fibre, cladding and glue
  //

  //
  G4VSolid *solidFibre = new G4Tubs("solidFibre", 0.0, (fiberOuterR-claddThickness), fiberLength/2.0, 0, 360.0*deg);
  //G4LogicalVolume* logicFibre = new G4LogicalVolume( solidFibre, SiO2, "logicFibre");
  G4LogicalVolume* logicFibre = new G4LogicalVolume( solidFibre, polystyrene_fibre, "logicFibre");
  Ta.setX(0.0);
  Ta.setY(+barBase/4);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  new G4PVPlacement(Tr,            //Transformation
		    logicFibre,    //its logical volume				 
		    "fibre",       //its name
		    world_logical, //its mother  volume
		    false,         //no boolean operation
		    0);	           //copy number

  //
  G4VSolid *solidFibreCladd = new G4Tubs("solidFibreCladd", (fiberOuterR - claddThickness), fiberOuterR, fiberLength/2.0, 0, 360.0*deg);
  G4LogicalVolume* logicFibreCladd = new G4LogicalVolume( solidFibreCladd, SiO2_cladd, "logicFibleCladd");
  Ta.setX(0.0);
  Ta.setY(+barBase/4);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  new G4PVPlacement(Tr,              //Transformation
		    logicFibreCladd, //its logical volume				 
		    "fibreCladd",    //its name
		    world_logical,   //its mother  volume
		    false,           //no boolean operation
		    0);	             //copy number

  //
  G4VSolid *solidFibreGlue = new G4Tubs("solidFibreGlue", fiberOuterR, barHoleR, barLength/2.0, 0, 360.0*deg);
  G4LogicalVolume* logicFibreGlue = new G4LogicalVolume( solidFibreGlue, glue, "logicFibleGlue");
  Ta.setX(0.0);
  Ta.setY(+barBase/4);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  new G4PVPlacement(Tr,              //Transformation
		    logicFibreGlue, //its logical volume				 
		    "fibreGlue",    //its name
		    world_logical,   //its mother  volume
		    false,           //no boolean operation
		    0);	             //copy number
  
  //
  // Bar coating
  //

  G4double  fExtrusionPolish = 1.;
  G4double  fExtrusionReflectivity = 1.;
  
  G4OpticalSurface* TiO2Surface = new G4OpticalSurface("TiO2Surface",
                                                       glisur,
                                                       ground,
                                                       dielectric_metal,
                                                       fExtrusionPolish);
  
  G4MaterialPropertiesTable* TiO2SurfaceProperty =
                                             new G4MaterialPropertiesTable();

  G4double p_TiO2[] = {2.00*eV, 4.2*eV};
  const G4int nbins = sizeof(p_TiO2)/sizeof(G4double);
  
  G4double refl_TiO2[] = {fExtrusionReflectivity,fExtrusionReflectivity};
  assert(sizeof(refl_TiO2) == sizeof(p_TiO2));
  G4double effi_TiO2[] = {0, 0};
  assert(sizeof(effi_TiO2) == sizeof(p_TiO2));

  TiO2SurfaceProperty -> AddProperty("REFLECTIVITY",p_TiO2,refl_TiO2,nbins);
  TiO2SurfaceProperty -> AddProperty("EFFICIENCY",p_TiO2,effi_TiO2,nbins);

  TiO2Surface -> SetMaterialPropertiesTable(TiO2SurfaceProperty);

  G4VSolid *solidBarCoating_base = new G4Box("solidBarCoating_base", barCoating_base_sizeX/2.0, barCoating_base_sizeY/2.0, barCoating_base_sizeZ/2.0);
  G4LogicalVolume* logicBarCoating_base = new G4LogicalVolume( solidBarCoating_base, TiO2, "logicBarCoating_base");
  Ta.setX(0.0);
  Ta.setY(-barCoating_base_sizeY/2.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  new G4PVPlacement(Tr,              //Transformation
		    logicBarCoating_base, //its logical volume				 
		    "barCoating_base",    //its name
		    world_logical,   //its mother  volume
		    false,           //no boolean operation
		    0);	             //copy number
  new G4LogicalSkinSurface("TiO2Surface",logicBarCoating_base,TiO2Surface);
  
  //Side rigth
  G4VSolid *solidBarCoating_side_r = new G4Box("solidBarCoating_side_r", barCoating_side_sizeX/2.0, barCoating_side_sizeY/2.0, barCoating_side_sizeZ/2.0);
  G4LogicalVolume* logicBarCoating_side_r = new G4LogicalVolume( solidBarCoating_side_r, TiO2, "logicBarCoating_side_r");
  Ta.setX((+barBase/2*TMath::Cos(TMath::Pi()/4.0) + barCoating_side_sizeY/2.0)*TMath::Cos(TMath::Pi()/4.0));
  Ta.setY((+barBase/2*TMath::Cos(TMath::Pi()/4.0) + barCoating_side_sizeY/2.0)*TMath::Sin(TMath::Pi()/4.0));
  Ta.setZ(0.0);
  Ra.rotateZ(-45.0*deg);
  Tr = G4Transform3D(Ra, Ta);
  new G4PVPlacement(Tr,                     //Transformation
		    logicBarCoating_side_r, //its logical volume				 
		    "barCoating_side_r",    //its name
		    world_logical,          //its mother volume
		    false,                  //no boolean operation
		    0);	                    //copy number
  Ra.rotateZ(45.0*deg);
  new G4LogicalSkinSurface("TiO2Surface",logicBarCoating_side_r,TiO2Surface);

  //Side left
  G4VSolid *solidBarCoating_side_l = new G4Box("solidBarCoating_side_l", barCoating_side_sizeX/2.0, barCoating_side_sizeY/2.0, barCoating_side_sizeZ/2.0);
  G4LogicalVolume* logicBarCoating_side_l = new G4LogicalVolume( solidBarCoating_side_l, TiO2, "logicBarCoating_side_l");
  Ta.setX(-(+barBase/2*TMath::Cos(TMath::Pi()/4.0) + barCoating_side_sizeY/2.0)*TMath::Sin(TMath::Pi()/4.0));
  Ta.setY((+barBase/2*TMath::Cos(TMath::Pi()/4.0) + barCoating_side_sizeY/2.0)*TMath::Sin(TMath::Pi()/4.0));
  Ta.setZ(0.0);
  Ra.rotateZ(45.0*deg);
  Tr = G4Transform3D(Ra, Ta);
  new G4PVPlacement(Tr,                     //Transformation
		    logicBarCoating_side_l, //its logical volume				 
		    "barCoating_side_l",    //its name
		    world_logical,          //its mother volume
		    false,                  //no boolean operation
		    0);	                    //copy number
  new G4LogicalSkinSurface("TiO2Surface",logicBarCoating_side_l,TiO2Surface);
  Ra.rotateZ(-45.0*deg);

  // 
  // Sensitive detector definition
  //
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SensitiveDetector* aSD = new SensitiveDetector("fTOF");
  SDman->AddNewDetector(aSD);
  sensitive_logical->SetSensitiveDetector(aSD);

  // Surface for killing photons at borders
  const G4int num1 = 2;
  G4double Ephoton[num1] = {1.5*eV, 5.8*eV};

  G4OpticalSurface* OpVolumeKillSurface =
    new G4OpticalSurface("VolumeKillSurface");
  OpVolumeKillSurface->SetType(dielectric_metal);
  OpVolumeKillSurface->SetFinish(polished);
  OpVolumeKillSurface->SetModel(glisur);


  G4double ReflectivityKill[num1] = {0., 0.};
  G4double EfficiencyKill[num1] = {1., 1.};
  G4MaterialPropertiesTable* VolumeKill = new G4MaterialPropertiesTable();
  VolumeKill->AddProperty("REFLECTIVITY", Ephoton, ReflectivityKill, num1);
  VolumeKill->AddProperty("EFFICIENCY",   Ephoton, EfficiencyKill,   num1);
  OpVolumeKillSurface->SetMaterialPropertiesTable(VolumeKill);
  new G4LogicalSkinSurface("SensitiveSurface", 
  			   sensitive_logical, OpVolumeKillSurface);


  
  /*
  G4double barCoating_base_sizeX = barBase;
  G4double barCoating_base_sizeY = 1.0*mm;
  G4double barCoating_base_sizeZ = barLength;

  G4double barCoating_side_sizeX = barBase/2.0/TMath::Cos(45.0*deg);
  G4double barCoating_side_sizeY = 1.0*mm;
  G4double barCoating_side_sizeZ = barLength;
  */

  /*

  //
  // C4F10
  //
  G4VSolid *c4f10_body_solid = new G4Box("c4f10_body",c4f10_body_sizeX/2.0,c4f10_body_sizeY/2.0,c4f10_body_sizeZ/2.0);
  G4LogicalVolume *c4f10_body_logical = new G4LogicalVolume(c4f10_body_solid,C4F10,"c4f10_body");
  Ta.setX(c4f10_body_X0);
  Ta.setY(c4f10_body_Y0);
  Ta.setZ(c4f10_body_Z0);
  Tr = G4Transform3D(Ra, Ta);
  G4VPhysicalVolume *c4f10_body_physical = new G4PVPlacement(Tr,                 //Transformation
							     c4f10_body_logical, //its logical volume				 
							     "Sensitive",        //its name
							     al_body_logical,    //its mother  volume
							     false,              //no boolean operation
							     0);	         //copy number
  c4f10_body_physical->GetName();

  //
  // Aerogel
  //
  G4VSolid *aerogel_body_solid = new G4Box("aerogel_body",aerogel_body_sizeX/2.0,aerogel_body_sizeY/2.0,aerogel_body_sizeZ/2.0);
  G4LogicalVolume *aerogel_body_logical = new G4LogicalVolume(aerogel_body_solid,Aerogel,"aerogel_body");
  Ta.setX(aerogel_body_X0);
  Ta.setY(aerogel_body_Y0);
  Ta.setZ(aerogel_body_Z0);
  Tr = G4Transform3D(Ra, Ta);
  */

  /*
  G4VPhysicalVolume *aerogel_body_physical = new G4PVPlacement(Tr,                   //Transformation
							       aerogel_body_logical, //its logical volume				 
							       "aerogel_body",       //its name
							       c4f10_body_logical,   //its mother  volume
							       false,                //no boolean operation
							       0);	             //copy number
  aerogel_body_physical->GetName();
  */

  /*
  //
  // Sensitive volume
  //
  G4VSolid *sensitive_solid = new G4Box("Sensitive", sensitive_sizeX/2.0, sensitive_sizeY/2.0, sensitive_sizeZ/2.0);
  G4LogicalVolume *sensitive_logical = new G4LogicalVolume(sensitive_solid, Aluminum,"Sensitive");
  Ta.setX(sensitive_X0);
  Ta.setY(sensitive_Y0);
  Ta.setZ(sensitive_Z0);
  Ra.rotateX(-sensitive_angle);
  Tr = G4Transform3D(Ra, Ta);
  G4VPhysicalVolume *sensitive_physical_top = new G4PVPlacement(Tr,                //Transformation
								sensitive_logical, //its logical volume				 
								"Sensitive",       //its name
								c4f10_body_logical,     //its mother  volume
								false,	           //no boolean operation
								0);	           //copy number
  Ra.rotateX(sensitive_angle);
  sensitive_physical_top->GetName();

  Ta.setX(sensitive_X0);
  Ta.setY(-sensitive_Y0);
  Ta.setZ(sensitive_Z0);
  Ra.rotateX(sensitive_angle);
  Tr = G4Transform3D(Ra, Ta);
  G4VPhysicalVolume *sensitive_physical_bot = new G4PVPlacement(Tr,                //Transformation
								sensitive_logical, //its logical volume				 
								"Sensitive",       //its name
								c4f10_body_logical,     //its mother  volume
								false,	           //no boolean operation
								0);	           //copy number
  Ra.rotateX(-sensitive_angle);
  sensitive_physical_bot->GetName();

  //
  // Spherical mirrors top
  //
  G4VSolid *sphericalMirror_body_solid = new G4Sphere("sphericalMirror_body",
						      sphericalMirror_R,
						      sphericalMirror_R + sphericalMirror_Thikness,
						      sphericalMirror_pSPhi,
						      sphericalMirror_pDPhi,
						      sphericalMirror_pSTheta,
						      sphericalMirror_pDTheta);

  G4VSolid *box_solid = new G4Box("box_solid",820*mm/2.0,600*mm/2.0,50*cm/2.0);

  G4IntersectionSolid *sphMirror = new G4IntersectionSolid("sphMirror",sphericalMirror_body_solid,box_solid,
							   0,G4ThreeVector(0*cm,0.0*cm, sphericalMirror_R));
  
  G4LogicalVolume *sphericalMirror_body_logical = new G4LogicalVolume(sphMirror,AluminumMirr,"sphericalMirror_body");
  G4LogicalVolume *sphericalMirror_body_logical_h = new G4LogicalVolume(sphericalMirror_body_solid,AluminumMirr,"sphericalMirror_body");

  sphericalMirror_body_logical->GetName();
  sphericalMirror_body_logical_h->GetName();
  
  Ta.setX(sphericalMirror_body_X0);
  Ta.setY(sphericalMirror_body_Y0);
  Ta.setZ(sphericalMirror_body_Z0);
  Ra.rotateX(sphericalMirror_angle);
  Tr = G4Transform3D(Ra, Ta);
  G4VPhysicalVolume *sphericalMirror_body_physical_top = new G4PVPlacement(Tr,                           //Transformation
									   sphericalMirror_body_logical, //its logical volume				 
									   "sphericalMirror_body",       //its name
									   c4f10_body_logical,           //its mother  volume
									   false,                        //no boolean operation
									   0);	                         //copy number
  Ra.rotateX(-sphericalMirror_angle);
  Tr = G4Transform3D(Ra, Ta);

  sphericalMirror_body_physical_top->GetName();

  Ta.setX(sphericalMirror_body_X0);
  Ta.setY(-sphericalMirror_body_Y0);
  Ta.setZ(sphericalMirror_body_Z0);
  Ra.rotateX(-sphericalMirror_angle);
  Tr = G4Transform3D(Ra, Ta);
  G4VPhysicalVolume *sphericalMirror_body_physical_bot = new G4PVPlacement(Tr,                           //Transformation
									   sphericalMirror_body_logical, //its logical volume				 
									   "sphericalMirror_body",       //its name
									   c4f10_body_logical,           //its mother  volume
									   false,                        //no boolean operation
									   0);	                     //copy number
  Ra.rotateX(sphericalMirror_angle);
  Tr = G4Transform3D(Ra, Ta);
  sphericalMirror_body_physical_bot->GetName();

  G4VSolid *flatMirror_solid = new G4Box("flatMirror_solid",flatMirror_lx/2.0,flatMirror_ly/2.0,flatMirror_lz/2.0);
  G4LogicalVolume *flatMirror_logical = new G4LogicalVolume(flatMirror_solid,AluminumMirr,"flatMirror");
  G4VPhysicalVolume *flatMirror_physical = new G4PVPlacement(0,G4ThreeVector(),world_logical,"flatMirror",0,false,0);
  Ta.setX(flatMirror_X0);
  Ta.setY(flatMirror_Y0);
  Ta.setZ(flatMirror_Z0);
  Ra.rotateX(flatMirror_angle);
  Tr = G4Transform3D(Ra, Ta);
  G4VPhysicalVolume *flatMirror_physical_top = new G4PVPlacement(Tr,                  //Transformation
								 flatMirror_logical,  //its logical volume				 
								 "flatMirror_body",   //its name
								 c4f10_body_logical,  //its mother  volume
								 false,               //no boolean operation
								 0);	          //copy number
  Ra.rotateX(-flatMirror_angle);
  Ta.setX(flatMirror_X0);
  Ta.setY(-flatMirror_Y0);
  Ta.setZ(flatMirror_Z0);
  Ra.rotateX(-flatMirror_angle);
  Tr = G4Transform3D(Ra, Ta);
  G4VPhysicalVolume *flatMirror_physical_bot = new G4PVPlacement(Tr,                  //Transformation
								 flatMirror_logical,  //its logical volume				 
								 "flatMirror_body",   //its name
								 c4f10_body_logical,  //its mother  volume
								 false,               //no boolean operation
								 0);	          //copy number
  Ra.rotateX(flatMirror_angle);

  flatMirror_physical->GetName();
  flatMirror_physical_top->GetName();
  flatMirror_physical_bot->GetName();

  //
  // Set Visualization Attributes
  //
  //G4Color blue        = G4Color(0., 0., 1.);
  //G4Color green       = G4Color(0., 1., 0.);
  G4Color red         = G4Color(1., 0., 0.);
  G4Color white       = G4Color(1., 1., 1.);
  //G4Color cyan        = G4Color(0., 1., 1.);
  G4Color DircColor   = G4Color(0.0, 0.0, 1.0, 0.2);
  G4Color SensColor   = G4Color(0.0, 1.0, 1.0, 0.1);

  worldVisAtt->SetColor(white);
  worldVisAtt->SetVisibility(true);
  quartzVisAtt->SetColor(DircColor);
  quartzVisAtt->SetVisibility(true);
  
  sensitiveVisAtt->SetColor(red);
  sensitiveVisAtt->SetVisibility(true);
  absVisAtt->SetColor(SensColor);
  absVisAtt->SetVisibility(true);


  sphericalMirror_body_logical->SetVisAttributes(quartzVisAtt);
  flatMirror_logical->SetVisAttributes(quartzVisAtt);

  sensitive_logical->SetVisAttributes(sensitiveVisAtt);
  al_body_logical->SetVisAttributes(absVisAtt);
  c4f10_body_logical->SetVisAttributes(absVisAtt);

  aerogel_body_logical->SetVisAttributes(quartzVisAtt);

  //world.logical->SetVisAttributes(worldVisAtt);
  */
  
  //
  // Define Optical Borders
  //

  /*
  // Surface for killing photons at borders
  const G4int num1 = 2;
  G4double Ephoton[num1] = {1.5*eV, 5.8*eV};

  G4OpticalSurface* OpVolumeKillSurface =
    new G4OpticalSurface("VolumeKillSurface");
  OpVolumeKillSurface->SetType(dielectric_metal);
  OpVolumeKillSurface->SetFinish(polished);
  OpVolumeKillSurface->SetModel(glisur);


  G4double ReflectivityKill[num1] = {0., 0.};
  G4double EfficiencyKill[num1] = {1., 1.};
  G4MaterialPropertiesTable* VolumeKill = new G4MaterialPropertiesTable();
  VolumeKill->AddProperty("REFLECTIVITY", Ephoton, ReflectivityKill, num1);
  VolumeKill->AddProperty("EFFICIENCY",   Ephoton, EfficiencyKill,   num1);
  OpVolumeKillSurface->SetMaterialPropertiesTable(VolumeKill);
  new G4LogicalSkinSurface("SensitiveSurface", 
  			   sensitive_logical, OpVolumeKillSurface);
  



  // Define mirror surface
  const G4int num2 = 36;
  G4double EfficiencyMirrors[num2];
  G4double WaveLength[num2];
  G4double PhotonEnergy[num2];
  G4double MirrorReflectivity[num2];
  for (G4int i=0; i<num2; i++) {
    WaveLength[i] = (300 + i*10)*nanometer;
    PhotonEnergy[num2 - (i+1)] = twopi*hbarc/WaveLength[i];
    EfficiencyMirrors[i] = 0.0;
    MirrorReflectivity[i]=0.85;
  }
  */
  /*
  G4double MirrorReflectivity[num2]=
    {0.87,0.88,0.885,0.89,0.895,0.9,0.905,0.91,0.915,0.92,0.923,0.9245,
     0.926,0.928,0.93,0.935,0.936,0.937,0.938,0.94,0.94,0.939,0.9382,
     0.938,0.937,0.937,0.936,0.935,0.934,0.932,0.93,0.928,0.926,0.924,
     0.922,0.92};
  */
  /*
  G4MaterialPropertiesTable* MirrorMPT = new G4MaterialPropertiesTable();
  MirrorMPT->AddProperty("RELECTIVITY", PhotonEnergy, MirrorReflectivity, num2);
  MirrorMPT->AddProperty("EFFICIENCY" , PhotonEnergy, EfficiencyMirrors,  num2);

  G4OpticalSurface* OpMirrorSurface = new G4OpticalSurface("MirrorSurface");
  OpMirrorSurface->SetType(dielectric_metal);
  OpMirrorSurface->SetFinish(polished);
  OpMirrorSurface->SetModel(glisur);
  new G4LogicalSkinSurface("MirrorSurfT",
			   sphericalMirror_body_logical, OpMirrorSurface);
  new G4LogicalSkinSurface("MirrorSurfT",
			   flatMirror_logical, OpMirrorSurface);

  OpMirrorSurface->SetMaterialPropertiesTable(MirrorMPT);
  ///////////////////////////////////////


  
  // 
  // Sensitive detector definition
  //
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SensitiveDetector* aSD = new SensitiveDetector("fTOF");
  SDman->AddNewDetector(aSD);
  sensitive_logical->SetSensitiveDetector(aSD);
  
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  G4double maxStep   = 0.1*mm;
  G4double maxLength = 2.0*m;
  G4double maxTime   = 20.0*ns; 
  G4double minEkin   = 1.0/100*MeV;
  G4double mionRang  = 0.01*mm;
  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  */
  /*
  secA.logical->SetUserLimits(stepLimit);
  secB.logical->SetUserLimits(stepLimit);
  secC.logical->SetUserLimits(stepLimit);
  secWin.logical->SetUserLimits(stepLimit);
  */

  //G4GDMLParser parser;
  //parser.Write("CpFM.gdml", world.physical);

  return world_physical;
}
