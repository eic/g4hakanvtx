#include "G4HakanVtxDisplayAction.h"

#include <g4main/PHG4DisplayAction.h>
#include <g4main/PHG4Utils.h>

#include <Geant4/G4Colour.hh>  // for G4Colour
#include <Geant4/G4LogicalVolume.hh>
#include <Geant4/G4VisAttributes.hh>

#include <cmath>  // for isfinite

using namespace std;

G4HakanVtxDisplayAction::G4HakanVtxDisplayAction(const std::string &name)
  : PHG4DisplayAction(name)
{
}

G4HakanVtxDisplayAction::~G4HakanVtxDisplayAction()
{
  for (auto &it : m_VisAttVec)
  {
    delete it;
  }
  m_VisAttVec.clear();
  m_VolMap.clear();
}

void G4HakanVtxDisplayAction::ApplyDisplayAction(G4VPhysicalVolume *physvol)
{
  // check if vis attributes exist, if so someone else has set them and we do nothing
  for (auto &it : m_VolMap)
  {
    if ((it.first)->GetVisAttributes())
    {
      continue;
    }
    G4VisAttributes *VisAtt = new G4VisAttributes();
    VisAtt->SetVisibility(true);
    VisAtt->SetForceSolid(true);
    switch (it.second)
    {
    case 0:
    case 1:
      VisAtt->SetColour(G4Color(0.0, 0.2, 0.8, 2.0));
      break;
    case 2:
      VisAtt->SetColour(G4Color(0.0, 0.2, 0.8, 0.7));
      break;
    default:
      VisAtt->SetColour(G4Color(0.0 + 0.1 * double(it.second - 3), 1., 1. - 0.1 * double(it.second - 3), 1.0));
      break;
    }
    (it.first)->SetVisAttributes(VisAtt);
    m_VisAttVec.push_back(VisAtt);
  }
  return;
}
