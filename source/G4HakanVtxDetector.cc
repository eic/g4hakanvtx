#include "G4HakanVtxDetector.h"

#include "G4HakanVtxDisplayAction.h"

#include <phparameter/PHParameters.h>
#include <phparameter/PHParametersContainer.h>

#include <g4main/PHG4Detector.h>       // for PHG4Detector
#include <g4main/PHG4DisplayAction.h>  // for PHG4DisplayAction
#include <g4main/PHG4Subsystem.h>

#include <Geant4/G4Box.hh>
#include <Geant4/G4LogicalVolume.hh>
#include <Geant4/G4Material.hh>
#include <Geant4/G4PVPlacement.hh>
#include <Geant4/G4RotationMatrix.hh>
#include <Geant4/G4SubtractionSolid.hh>
#include <Geant4/G4SystemOfUnits.hh>
#include <Geant4/G4ThreeVector.hh>
#include <Geant4/G4Trap.hh>
#include <Geant4/G4Tubs.hh>
#include <Geant4/G4VisAttributes.hh>

#include <cmath>
#include <iostream>  // for operator<<, endl, bas...

class G4VSolid;
class PHCompositeNode;

using namespace std;

G4HakanVtxDetector::G4HakanVtxDetector(PHG4Subsystem *subsys,
                                       PHCompositeNode *Node,
                                       PHParametersContainer *params,
                                       const std::string &dnam)
  : PHG4Detector(subsys, Node, dnam)
  , m_ParamsContainer(params)
  , m_DisplayAction(dynamic_cast<G4HakanVtxDisplayAction *>(subsys->GetDisplayAction()))
{
  const PHParameters *par = m_ParamsContainer->GetParameters(-1);
  m_IsActiveFlag = par->get_int_param("active");
  m_IsAbsorberActiveFlag = par->get_int_param("absorberactive");
  m_Layers = par->get_int_param("central_layers");
}

//_______________________________________________________________
//_______________________________________________________________
int G4HakanVtxDetector::IsInDetector(G4VPhysicalVolume *volume) const
{
  set<G4VPhysicalVolume *>::const_iterator iter = m_PhysicalVolumesSet.find(volume);
  if (iter != m_PhysicalVolumesSet.end())
  {
    return 1;
  }
  return 0;
}

void G4HakanVtxDetector::ConstructMe(G4LogicalVolume *logicWorld)
{
  for (int ilayer = 0; ilayer < m_Layers; ilayer++)
  {
    // get parameters for this layer
    const PHParameters *par = m_ParamsContainer->GetParameters(ilayer);
    double cb_VTX_ladder_DZ = par->get_double_param("Dz") * cm;
    double cb_VTX_ladder_DY = par->get_double_param("Dy") * cm;
    double cb_VTX_ladder_Thickness = par->get_double_param("Dx") * cm;
    double dR = par->get_double_param("Rin") * cm;
    double myL = 2 * M_PI * dR;
    int laddersCount = myL / cb_VTX_ladder_DY;  //Calculate number of staves, from radius and stave width. No overlap defined, but I guess this does ciel implicitly.
    for (int i = 0; i < 2; i++)
    {
      double LN = cb_VTX_ladder_DY * laddersCount;
      double LN1 = cb_VTX_ladder_DY * (laddersCount + 1 + i);
      if (LN / LN1 > 0.8)
      {
        laddersCount++;
      }
    }

    double cb_VTX_ladder_deltaphi = 2 * M_PI / laddersCount;
    string solidname = "cb_VTX_ladder_Solid_" + to_string(ilayer);
    G4VSolid *solid = new G4Box(solidname, cb_VTX_ladder_Thickness / 2., cb_VTX_ladder_DY / 2., cb_VTX_ladder_DZ / 2.);
    string logical_name = "cb_VTX_ladder_Logic_" + to_string(ilayer);
    G4LogicalVolume *logical = new G4LogicalVolume(solid, G4Material::GetMaterial("G4_Si"), logical_name);
    m_DisplayAction->AddVolume(logical, ilayer);

    for (int ia = 0; ia < laddersCount; ia++)
    {
      double phi = (ia * (cb_VTX_ladder_deltaphi));
      double x = -dR * cos(phi);
      double y = -dR * sin(phi);
      G4RotationMatrix rot;
      rot.rotateZ(cb_VTX_ladder_deltaphi * ia);
      rot.rotateZ(par->get_double_param("deltashi") * deg);
      ostringstream physname;
      physname << "cb_VTX_ladder_Phys_" << ilayer << "_" << ia;
      G4VPhysicalVolume *phy = new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(x, y, 0)),
                                                 logical, physname.str(),
                                                 logicWorld, 0, false, OverlapCheck());

      m_PhysicalVolumesSet.insert(phy);
    }
  }
  ConstructLaddersEndcaps(logicWorld);
  return;
}

void G4HakanVtxDetector::ConstructLaddersEndcaps(G4LogicalVolume *motherlogic)
{
  int forward_id_offset = m_ParamsContainer->GetParameters(-1)->get_int_param("forward_id_offset");
  double Rzshift = m_ParamsContainer->GetParameters(-1)->get_double_param("Rzshift") * cm;
  double fVTX_END_EDZ = m_ParamsContainer->GetParameters(-1)->get_double_param("fVTX_END_EDZ") * cm;
  double fVTX_END_EDX1 = m_ParamsContainer->GetParameters(-1)->get_double_param("fVTX_END_EDX1") * cm;
  double fVTX_END_EDX2 = m_ParamsContainer->GetParameters(-1)->get_double_param("fVTX_END_EDX2") * cm;
  for (int lay = 0; lay < m_ParamsContainer->GetParameters(-1)->get_int_param("forward_layers"); ++lay)
  {
    const PHParameters *par = m_ParamsContainer->GetParameters(lay + forward_id_offset);
    string solidname = "Solid_VTX_ladder_END_E" + to_string(lay);
    G4VSolid *solid = new G4Trap(solidname, fVTX_END_EDZ,
                                 par->get_double_param("fVTX_END_EDY") * cm + lay * 2., fVTX_END_EDX1, fVTX_END_EDX2);
    string logicname = "Logic_VTX_ladder_END_E" + to_string(lay);
    G4LogicalVolume *logical = new G4LogicalVolume(solid, G4Material::GetMaterial("G4_Si"), logicname);
    m_DisplayAction->AddVolume(logical, lay + forward_id_offset);
    double Fdeltaphi = par->get_double_param("Fdeltaphi") * deg;
    double Ftheta = par->get_double_param("Ftheta") * deg;
    double RxF = par->get_double_param("RxF") * cm;
    double RyF = RxF;
    double RxF2 = RxF;
    double RyF2 = RyF;
    double RzF = -par->get_double_param("RzF") * cm - Rzshift;
    double RzF2 = -RzF;
    for (int ia = 0; ia < par->get_int_param("NUMF"); ia++)
    {
      G4RotationMatrix rot;
      double phi = (ia * (Fdeltaphi)) / rad;  // convert to rad (G4 internal units are rad at time of this writing)
      double x = -RxF * cos(phi);
      double y = -RyF * sin(phi);
      double z = RzF;
      rot.rotateX(Ftheta);
      rot.rotateZ(-90 * deg + (Fdeltaphi * (ia + 1)));
      //WORKING       rm1[lay][ia].rotateX(-60*deg);
      //WORKING       rm1[lay][ia].rotateZ(-90+(cb_VTX_ladder_deltaphi*(ia+1)));
      string physname = "VTX_ladderEnd_" + to_string(lay) + "_" + to_string(ia);
      G4VPhysicalVolume *phy = new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(x, y, z)),
                                                 logical, physname,
                                                 motherlogic, 0, false, OverlapCheck());
      m_PhysicalVolumesSet.insert(phy);
    }

    solidname = "Solid_VTX_ladder_END_H" + to_string(lay);
    solid = new G4Trap(solidname, fVTX_END_EDZ,
                       par->get_double_param("fVTX_END_EDY") * cm, fVTX_END_EDX1, fVTX_END_EDX2);
    logicname = "Logic_VTX_ladder_END_H" + to_string(lay);
    logical = new G4LogicalVolume(solid, G4Material::GetMaterial("G4_Si"), logicname);
    m_DisplayAction->AddVolume(logical, lay + forward_id_offset);
    for (int ia = 0; ia < par->get_int_param("NUMF"); ia++)
    {
      G4RotationMatrix rot;
      double phi = (ia * (Fdeltaphi)) / rad;  // convert to rad (G4 internal units are rad at time of this writing)
      double x = -RxF2 * cos(phi);
      double y = -RyF2 * sin(phi);
      double z = RzF2;
      rot.rotateX(Ftheta);
      rot.rotateZ(-90 * deg + (Fdeltaphi * (ia + 1)));
      //WORKING       rm1[lay][ia].rotateX(-60*deg);
      //WORKING       rm1[lay][ia].rotateZ(-90+(cb_VTX_ladder_deltaphi*(ia+1)));
      string physname = "VTX_ladderEnd2_" + to_string(lay) + "_" + to_string(ia);
      G4VPhysicalVolume *phy = new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(x, y, z)),
                                                 logical, physname,
                                                 motherlogic, 0, false, OverlapCheck());
      m_PhysicalVolumesSet.insert(phy);
    }
  }
}

void G4HakanVtxDetector::Print(const std::string &what) const
{
  cout << "Hakans VTX Detector:" << endl;
  if (what == "ALL" || what == "VOLUME")
  {
    cout << "Version 0.1" << endl;
    cout << "Parameters:" << endl;
    m_ParamsContainer->Print();
  }
  return;
}
