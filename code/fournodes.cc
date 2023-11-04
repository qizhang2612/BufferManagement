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
// n1 -----|      |---!!--n4
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
  LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (5);

  Ptr<Node> sw = nodes.Get(2);
  sw->SetNodeType(1);
  sw->m_switch->Set(0.5,1024*1024,0);


  NodeContainer node02,node12,node23,node24;
  node02 = NodeContainer(nodes.Get(0),nodes.Get(2));
  node12 = NodeContainer(nodes.Get(1),nodes.Get(2));
  node23 = NodeContainer(nodes.Get(2),nodes.Get(3));
  node24 = NodeContainer(nodes.Get(2),nodes.Get(4));
  
  LzyPointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  //pointToPoint.SetQueue("ns3::DropTailQueue","MaxSize",QueueSizeValue (QueueSize (QueueSizeUnit::BYTES,1000000)));
  pointToPoint.SetQueue("ns3::MultipleQueue","useqCnt",StringValue ("2"),"dequeueType",StringValue("0"),"enqueueType",StringValue("1"));

  NetDeviceContainer devices02,devices12,devices23,devices24;
  devices02 = pointToPoint.Install (node02);
  std::cout<<"----"<<devices02.GetN()<<std::endl;

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

//----------------------------------------------------------------------------node0-->node3
  UdpServerHelper echoServer1 (9);

  ApplicationContainer serverApps1 = echoServer1.Install (node23.Get (1));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (30.0));

  UdpClientHelper echoClient1 (interfaces23.GetAddress (1), 9);

  InetSocketAddress rmt1 (interfaces23.GetAddress (1), 9);
  rmt1.SetTos (0x4);
  AddressValue remoteAddress1 (rmt1);
  echoClient1.SetAttribute ("RemoteAddress", remoteAddress1);



  //echoClient1.SetAttribute ("MaxPackets", UintegerValue (2000));
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps1 = echoClient1.Install (node02.Get (0));
  clientApps1.Start (Seconds (2.0));
  clientApps1.Stop (Seconds (30.0));
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------node1-->node4   I 
  UdpServerHelper echoServer2 (10);

  ApplicationContainer serverApps2 = echoServer2.Install (node24.Get (1));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (30.0));

  //UdpEchoClientHelper echoClient2 (interfaces24.GetAddress (1), 10);
  UdpEchoClientHelper echoClient2 (interfaces23.GetAddress (1), 10);

  InetSocketAddress rmt2 (interfaces23.GetAddress (1), 10);
  rmt2.SetTos (0x5);
  AddressValue remoteAddress2 (rmt2);
  echoClient2.SetAttribute ("RemoteAddress", remoteAddress2);


  //echoClient2.SetAttribute ("MaxPackets", UintegerValue (6000));
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps2 = echoClient2.Install (node12.Get (0));
  clientApps2.Start (Seconds (3.0));
  clientApps2.Start (Seconds (2.1));
  clientApps2.Stop (Seconds (30.0));
//----------------------------------------------------------------------------


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
