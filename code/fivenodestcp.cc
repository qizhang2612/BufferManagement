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
// n0 -----|      |-----n3
//          --n2--
// n1 -----|      |-----n4
//
//
//
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
  nodes.Create (5);

  Ptr<Node> sw = nodes.Get(2);
  sw->SetNodeType(1);


  NodeContainer node02,node12,node23,node24;
  node02 = NodeContainer(nodes.Get(0),nodes.Get(2));
  node12 = NodeContainer(nodes.Get(1),nodes.Get(2));
  node23 = NodeContainer(nodes.Get(2),nodes.Get(3));
  node24 = NodeContainer(nodes.Get(2),nodes.Get(4));
  
  LzyPointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  pointToPoint.SetQueue("ns3::DropTailQueue","MaxSize",QueueSizeValue (QueueSize (QueueSizeUnit::BYTES,2000)));

  NetDeviceContainer devices02,devices12,devices23,devices24;
  devices02 = pointToPoint.Install (node02);


  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  devices12 = pointToPoint.Install (node12);


  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  devices23 = pointToPoint.Install (node23);

  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  devices24 = pointToPoint.Install (node24);


  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address02,address12,address23,address24;
  address02.SetBase ("10.1.1.0", "255.255.255.0");
  address12.SetBase ("10.1.2.0", "255.255.255.0");
  address23.SetBase ("10.1.3.0", "255.255.255.0");
  address24.SetBase ("10.1.4.0", "255.255.255.0");



  Ipv4InterfaceContainer interfaces02 = address02.Assign (devices02);
  Ipv4InterfaceContainer interfaces12 = address12.Assign (devices12);
  Ipv4InterfaceContainer interfaces23 = address23.Assign (devices23);
  Ipv4InterfaceContainer interfaces24 = address24.Assign (devices24);

//----------------------------------------------------------------------------
      uint16_t port1 = 50001;
      Address sinkLocalAddress1 (InetSocketAddress (Ipv4Address::GetAny (), port1));
      PacketSinkHelper sinkHelper1 ("ns3::TcpSocketFactory", sinkLocalAddress1);
      ApplicationContainer sinkApp1 = sinkHelper1.Install (node23.Get (1));
      sinkApp1.Start (Seconds (0));
      sinkApp1.Stop (Seconds (5));

      OnOffHelper clientHelper1 ("ns3::TcpSocketFactory", Address ());
      clientHelper1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
      clientHelper1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
      clientHelper1.SetAttribute 
        ("DataRate", DataRateValue (DataRate ("10Mb/s")));
      clientHelper1.SetAttribute 
        ("PacketSize", UintegerValue (1000));

      ApplicationContainer clientApps1;
      AddressValue remoteAddress1
        (InetSocketAddress (interfaces23.GetAddress (1), port1));
      clientHelper1.SetAttribute ("Remote", remoteAddress1);
      clientApps1.Add (clientHelper1.Install (node02.Get (0)));
      clientApps1.Start (Seconds (1));
      clientApps1.Stop (Seconds (3));
//----------------------------------------------------------------------------


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
