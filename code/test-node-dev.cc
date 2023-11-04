#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"


#include "ns3/traffic-control-module.h"
#include "ns3/traffic-control-layer.h"

using namespace ns3;

int
main (int argc, char *argv[])
{
    Ptr<Node> a = Create<Node>();
    Ptr<PointToPointNetDevice> devA = Create<PointToPointNetDevice> ();
    devA->SetAddress (Mac48Address::Allocate ());
    a->AddDevice (devA);

    std::cout<<a->GetNDevices ()<<std::endl;
}