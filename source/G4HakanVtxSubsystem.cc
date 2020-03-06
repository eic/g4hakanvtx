#include "G4HakanVtxSubsystem.h"

#include "G4HakanVtxDetector.h"
#include "G4HakanVtxDisplayAction.h"
#include "G4HakanVtxSteppingAction.h"

#include <phparameter/PHParameters.h>
#include <phparameter/PHParametersContainer.h>

#include <g4main/PHG4DisplayAction.h>
#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4SteppingAction.h>  // for PHG4SteppingAction

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>    // for PHIODataNode
#include <phool/PHNode.h>          // for PHNode
#include <phool/PHNodeIterator.h>  // for PHNodeIterator
#include <phool/PHObject.h>        // for PHObject
#include <phool/getClass.h>

#include <cmath>  // for isfinite

using namespace std;

//_______________________________________________________________________
G4HakanVtxSubsystem::G4HakanVtxSubsystem(const std::string &name)
  : PHG4DetectorGroupSubsystem(name)
  , m_Detector(nullptr)
  , m_SteppingAction(nullptr)
  , m_DisplayAction(nullptr)
{
  // call base class method which will set up parameter infrastructure
  // and call our SetDefaultParameters() method
  InitializeParameters();
}

//_______________________________________________________________________
G4HakanVtxSubsystem::~G4HakanVtxSubsystem()
{
  delete m_DisplayAction;
}

//_______________________________________________________________________
int G4HakanVtxSubsystem::InitRunSubsystem(PHCompositeNode *topNode)
{
  PHNodeIterator iter(topNode);
  PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "DST"));
  m_DisplayAction = new G4HakanVtxDisplayAction(Name());
  PHNodeIterator dstIter(dstNode);
  if (GetParamsContainer()->GetParameters(-1)->get_int_param("active"))
  {
    PHCompositeNode *DetNode = dynamic_cast<PHCompositeNode *>(dstIter.findFirst("PHCompositeNode", SuperDetector()));
    if (!DetNode)
    {
      DetNode = new PHCompositeNode(SuperDetector());
      dstNode->addNode(DetNode);
    }
    string g4hitnodename = "G4HIT_" + SuperDetector();
    PHG4HitContainer *g4_hits = findNode::getClass<PHG4HitContainer>(DetNode, g4hitnodename);
    if (!g4_hits)
    {
      g4_hits = new PHG4HitContainer(g4hitnodename);
      DetNode->addNode(new PHIODataNode<PHObject>(g4_hits, g4hitnodename, "PHObject"));
    }
  }
  // create detector
  m_Detector = new G4HakanVtxDetector(this, topNode, GetParamsContainer(), Name());
  m_Detector->OverlapCheck(CheckOverlap());
   m_Detector->SuperDetector(SuperDetector());
  // create stepping action if detector is active
  if (GetParamsContainer()->GetParameters(-1)->get_int_param("active"))
  {
    m_SteppingAction = new G4HakanVtxSteppingAction(m_Detector, GetParamsContainer());
  }
  return 0;
}

//_______________________________________________________________________
int G4HakanVtxSubsystem::process_event(PHCompositeNode *topNode)
{
  // pass top node to stepping action so that it gets
  // relevant nodes needed internally
  if (m_SteppingAction)
  {
    m_SteppingAction->SetInterfacePointers(topNode);
  }
  return 0;
}

void G4HakanVtxSubsystem::Print(const string &what) const
{
  if (m_Detector)
  {
    m_Detector->Print(what);
  }
  return;
}

//_______________________________________________________________________
PHG4Detector *G4HakanVtxSubsystem::GetDetector(void) const
{
  return m_Detector;
}

void G4HakanVtxSubsystem::SetDefaultParameters()
{
  int NLayers = 0;  // just for counting the layers, if one is added or subtracted we do not have to keep updating this
  // all units are in cm/degrees
  set_default_double_param(0, "Dx", 0.0281);
  set_default_double_param(0, "Dy", 1.5);
  set_default_double_param(0, "Dz", 27.);
  set_default_double_param(0, "Rin", 2.34);
  set_default_double_param(0, "deltashi", -7.);
  // set_default_double_param(0, "PixelDx", 1.);        // dz/10. = 1.
  // set_default_double_param(0, "PixelDy", 2. / 50.);  // dy/50
  NLayers++;

  set_default_double_param(1, "Dx", 0.0281);
  set_default_double_param(1, "Dy", 1.5);
  set_default_double_param(1, "Dz", 27.);
  set_default_double_param(1, "Rin", 4.68);
  set_default_double_param(1, "deltashi", -7.);
  // set_default_double_param(1, "PixelDx", 11. / 10.);  // dz/10
  // set_default_double_param(1, "PixelDy", 2. / 50.);   // dy/50
  NLayers++;

  set_default_double_param(2, "Dx", 0.0749);
  set_default_double_param(2, "Dy", 6.);
  set_default_double_param(2, "Dz", 84.);
  set_default_double_param(2, "Rin", 8.76);
  set_default_double_param(2, "deltashi", -7.);
  // set_default_double_param(2, "PixelDx", 18. / 50.);  // dz/50
  // set_default_double_param(2, "PixelDy", 4. / 10.);   // dy/10
  NLayers++;

  set_default_double_param(3, "Dx", 0.0749);
  set_default_double_param(3, "Dy", 6.);
  set_default_double_param(3, "Dz", 84.);
  set_default_double_param(3, "Rin", 13.38);
  set_default_double_param(3, "deltashi", -7.);
  // set_default_double_param(3, "PixelDx", 24. / 50.);  // dz/50
  // set_default_double_param(3, "PixelDy", 4. / 10.);   // dy/10
  NLayers++;

  set_default_double_param(4, "Dx", 2 * 0.0749);
  set_default_double_param(4, "Dy", 6.);
  set_default_double_param(4, "Dz", 84.);
  set_default_double_param(4, "Rin", 18.);
  set_default_double_param(4, "deltashi", -7.);
  // set_default_double_param(4, "PixelDx", 24. / 50.);  // dz/50
  // set_default_double_param(4, "PixelDy", 4. / 10.);   // dy/10
  NLayers++;

  int NFLayers = 0;
  set_default_int_param(10, "NUMF", 12);
  set_default_double_param(10, "fVTX_END_EDY", 12.);
  set_default_double_param(10, "Fdeltaphi", 30.);
  set_default_double_param(10, "Ftheta", 55);
  set_default_double_param(10, "RxF", 0.8);
  set_default_double_param(10, "RzF", 7.);
  NFLayers++;

  set_default_int_param(11, "NUMF", 18);
  set_default_double_param(11, "fVTX_END_EDY", 14.);
  set_default_double_param(11, "Fdeltaphi", 20.);
  set_default_double_param(11, "Ftheta", -45);
  set_default_double_param(11, "RxF", 1.);
  set_default_double_param(11, "RzF", 1.);
  NFLayers++;

  set_default_int_param(12, "NUMF", 20);
  set_default_double_param(12, "fVTX_END_EDY", 16.);
  set_default_double_param(12, "Fdeltaphi", 18.);
  set_default_double_param(12, "Ftheta", -38);
  set_default_double_param(12, "RxF", 1.1);
  set_default_double_param(12, "RzF", 5.5);
  NFLayers++;

  set_default_int_param(13, "NUMF", 24);
  set_default_double_param(13, "fVTX_END_EDY", 18.);
  set_default_double_param(13, "Fdeltaphi", 15.);
  set_default_double_param(13, "Ftheta", -40.);
  set_default_double_param(13, "RxF", 1.3);
  set_default_double_param(13, "RzF", 5.5);
  NFLayers++;

  // Here are the global parameters (detid = -1)
  set_default_int_param(-1, "active", 1);
  set_default_int_param(-1, "absorberactive", 0);
  set_default_int_param(-1, "blackhole", 0);
  set_default_int_param(-1, "central_layers", NLayers);
  set_default_int_param(-1, "forward_layers", NFLayers);
  set_default_int_param(-1, "forward_id_offset", 10);
  set_default_double_param(-1, "Rzshift", 24.);
  set_default_double_param(-1, "fVTX_END_EDZ", 0.05);
  set_default_double_param(-1, "fVTX_END_EDX1", 6.);
  set_default_double_param(-1, "fVTX_END_EDX2", 4.);
}
