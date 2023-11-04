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
#include "iostream"

// Default Network Topology
//
//       
// n0 -------------- n1------------n2
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
  LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (3);

  Ptr<Node> sw = nodes.Get(1);
  
  sw->SetNodeType(1);
  sw->m_switch->Set(0.5,1024*1024,0);

  NodeContainer node01,node12;
  node01 = NodeContainer(nodes.Get(0),nodes.Get(1));
  node12 = NodeContainer(nodes.Get(1),nodes.Get(2));
  
  LzyPointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  pointToPoint.SetQueue("ns3::MultipleQueue","useqCnt",StringValue ("2"),"dequeueType",StringValue ("2"),"w0",StringValue ("1500"),"w1",StringValue ("50"));

  NetDeviceContainer devices01,devices12;
  devices01 = pointToPoint.Install (node01);


  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  devices12 = pointToPoint.Install (node12);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address01,address12;
  address01.SetBase ("10.1.1.0", "255.255.255.0");
  address12.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces01 = address01.Assign (devices01);
  Ipv4InterfaceContainer interfaces12 = address12.Assign (devices12);

  //UdpEchoServerHelper echoServer (9);
  UdpServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (node12.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (30.0));
  //UdpEchoClientHelper echoClient (interfaces12.GetAddress (1), 9);
  UdpClientHelper echoClient (interfaces12.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (node01.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (30.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
