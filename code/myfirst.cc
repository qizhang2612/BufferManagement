/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"


#include "ns3/traffic-control-module.h"
#include "ns3/traffic-control-layer.h"
// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);
   std::cout<<nodes.Get(0)->GetNDevices ()<<std::endl;
  InternetStackHelper stack;
  stack.Install (nodes);
 
  // TrafficControlHelper tchPfifo;
  // uint16_t handle = tchPfifo.SetRootQueueDisc ("ns3::PfifoFastQueueDisc");
  // tchPfifo.AddInternalQueues (handle, 3, "ns3::DropTailQueue", "MaxPackets", UintegerValue (1000));

  // tchPfifo.Install(devices);


  
  // //uint16_t handle =  tchFifo.SetRootQueueDisc ("ns3::FifoQueueDisc");
  
  // // //tchFifo.AddInternalQueues (handle, 3, "ns3::DropTailQueue", "MaxPackets", QueueSizeValue (QueueSize (QueueSizeUnit::PACKETS, 100)));
  TrafficControlHelper tchFifo;
  tchFifo.SetRootQueueDisc ("ns3::FifoQueueDisc","MaxSize", StringValue ("1000p"));
  tchFifo.Install(devices);

   Ptr<TrafficControlLayer> tc = nodes.Get (1)->GetObject<TrafficControlLayer> ();
   if(tc==0){
     std::cout<<"fail"<<std::endl;
   }else{
     std::cout<<"succeed"<<std::endl;
   }
   std::cout<<nodes.Get(0)->GetNDevices ()<<std::endl;
  //  std::cout<<nodes.Get(1)->GetDevice(0)->GetTypeId()<<std::endl;
  //  std::cout<<nodes.Get(1)->GetDevice(1)->GetTypeId()<<std::endl;
        

  // TrafficControlHelper tchPfifo;
  // uint16_t handle = tchPfifo.SetRootQueueDisc ("ns3::PfifoFastQueueDisc");
  // tchPfifo.AddInternalQueues (handle, 3, "ns3::DropTailQueue", "MaxSize", StringValue ("1000p"));
  // tchPfifo.Install(devices);



  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
