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
  , m_MyVolume(nullptr)
  , m_VisAtt(nullptr)
  , m_Colour(nullptr)
{
}

G4HakanVtxDisplayAction::~G4HakanVtxDisplayAction()
{
  delete m_VisAtt;
  delete m_Colour;
}

void G4HakanVtxDisplayAction::ApplyDisplayAction(G4VPhysicalVolume *physvol)
{
  // check if vis attributes exist, if so someone else has set them and we do nothing
  if (m_MyVolume->GetVisAttributes())
  {
    return;
  }
  m_VisAtt = new G4VisAttributes();
  if (m_Colour)
  {
    m_VisAtt->SetColour(m_Colour->GetRed(),
                        m_Colour->GetGreen(),
                        m_Colour->GetBlue(),
                        m_Colour->GetAlpha());
    m_VisAtt->SetVisibility(true);
    m_VisAtt->SetForceSolid(true);
  }
  m_MyVolume->SetVisAttributes(m_VisAtt);
  return;
}

void G4HakanVtxDisplayAction::SetColor(const double red, const double green, const double blue, const double alpha)
{
  if (isfinite(red) && isfinite(green) && isfinite(blue) && isfinite(alpha))
  {
    m_Colour = new G4Colour(red, green, blue, alpha);
  }
  return;
}
