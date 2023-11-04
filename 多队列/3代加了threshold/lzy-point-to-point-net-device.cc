/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007, 2008 University of Washington
 *
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

#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/mac48-address.h"
#include "ns3/llc-snap-header.h"
#include "ns3/error-model.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "lzy-point-to-point-net-device.h"
#include "lzy-point-to-point-channel.h"
#include "ppp-header.h"
#include <iostream>
#include <fstream>
namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LzyPointToPointNetDevice");

NS_OBJECT_ENSURE_REGISTERED (LzyPointToPointNetDevice);

TypeId 
LzyPointToPointNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LzyPointToPointNetDevice")
    .SetParent<NetDevice> ()
    .SetGroupName ("PointToPoint")
    .AddConstructor<LzyPointToPointNetDevice> ()
    .AddAttribute ("Mtu", "The MAC-level Maximum Transmission Unit",
                   UintegerValue (DEFAULT_MTU),
                   MakeUintegerAccessor (&LzyPointToPointNetDevice::SetMtu,
                                         &LzyPointToPointNetDevice::GetMtu),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("Address", 
                   "The MAC address of this device.",
                   Mac48AddressValue (Mac48Address ("ff:ff:ff:ff:ff:ff")),
                   MakeMac48AddressAccessor (&LzyPointToPointNetDevice::m_address),
                   MakeMac48AddressChecker ())
    .AddAttribute ("DataRate", 
                   "The default data rate for point to point links",
                   DataRateValue (DataRate ("32768b/s")),
                   MakeDataRateAccessor (&LzyPointToPointNetDevice::m_bps),
                   MakeDataRateChecker ())
    .AddAttribute ("ReceiveErrorModel", 
                   "The receiver error model used to simulate packet loss",
                   PointerValue (),
                   MakePointerAccessor (&LzyPointToPointNetDevice::m_receiveErrorModel),
                   MakePointerChecker<ErrorModel> ())
    .AddAttribute ("InterframeGap", 
                   "The time to wait between packet (frame) transmissions",
                   TimeValue (Seconds (0.0)),
                   MakeTimeAccessor (&LzyPointToPointNetDevice::m_tInterframeGap),
                   MakeTimeChecker ())

    //
    // Transmit queueing discipline for the device which includes its own set
    // of trace hooks.
    //
    .AddAttribute ("TxQueue", 
                   "A queue to use as the transmit queue in the device.",
                   PointerValue (),
                   MakePointerAccessor (&LzyPointToPointNetDevice::m_queue),
                   MakePointerChecker<Queue<Packet> > ())

    //
    // Trace sources at the "top" of the net device, where packets transition
    // to/from higher layers.
    //
    .AddTraceSource ("MacTx", 
                     "Trace source indicating a packet has arrived "
                     "for transmission by this device",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_macTxTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("MacTxDrop", 
                     "Trace source indicating a packet has been dropped "
                     "by the device before transmission",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_macTxDropTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("MacPromiscRx", 
                     "A packet has been received by this device, "
                     "has been passed up from the physical layer "
                     "and is being forwarded up the local protocol stack.  "
                     "This is a promiscuous trace,",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_macPromiscRxTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("MacRx", 
                     "A packet has been received by this device, "
                     "has been passed up from the physical layer "
                     "and is being forwarded up the local protocol stack.  "
                     "This is a non-promiscuous trace,",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_macRxTrace),
                     "ns3::Packet::TracedCallback")
#if 0
    // Not currently implemented for this device
    .AddTraceSource ("MacRxDrop", 
                     "Trace source indicating a packet was dropped "
                     "before being forwarded up the stack",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_macRxDropTrace),
                     "ns3::Packet::TracedCallback")
#endif
    //
    // Trace sources at the "bottom" of the net device, where packets transition
    // to/from the channel.
    //
    .AddTraceSource ("PhyTxBegin", 
                     "Trace source indicating a packet has begun "
                     "transmitting over the channel",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_phyTxBeginTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyTxEnd", 
                     "Trace source indicating a packet has been "
                     "completely transmitted over the channel",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_phyTxEndTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyTxDrop", 
                     "Trace source indicating a packet has been "
                     "dropped by the device during transmission",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_phyTxDropTrace),
                     "ns3::Packet::TracedCallback")
#if 0
    // Not currently implemented for this device
    .AddTraceSource ("PhyRxBegin", 
                     "Trace source indicating a packet has begun "
                     "being received by the device",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_phyRxBeginTrace),
                     "ns3::Packet::TracedCallback")
#endif
    .AddTraceSource ("PhyRxEnd", 
                     "Trace source indicating a packet has been "
                     "completely received by the device",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_phyRxEndTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyRxDrop", 
                     "Trace source indicating a packet has been "
                     "dropped by the device during reception",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_phyRxDropTrace),
                     "ns3::Packet::TracedCallback")

    //
    // Trace sources designed to simulate a packet sniffer facility (tcpdump).
    // Note that there is really no difference between promiscuous and 
    // non-promiscuous traces in a point-to-point link.
    //
    .AddTraceSource ("Sniffer", 
                    "Trace source simulating a non-promiscuous packet sniffer "
                     "attached to the device",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_snifferTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PromiscSniffer", 
                     "Trace source simulating a promiscuous packet sniffer "
                     "attached to the device",
                     MakeTraceSourceAccessor (&LzyPointToPointNetDevice::m_promiscSnifferTrace),
                     "ns3::Packet::TracedCallback")
  ;
  return tid;
}


LzyPointToPointNetDevice::LzyPointToPointNetDevice () 
  :
    m_txMachineState (READY),
    m_channel (0),
    m_linkUp (false),
    m_currentPkt (0)
{
  NS_LOG_FUNCTION (this);
}

LzyPointToPointNetDevice::~LzyPointToPointNetDevice ()
{
  NS_LOG_FUNCTION (this);
}

void
LzyPointToPointNetDevice::AddHeader (Ptr<Packet> p, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << p << protocolNumber);
  PppHeader ppp;
  ppp.SetProtocol (EtherToPpp (protocolNumber));
  p->AddHeader (ppp);
}

bool
LzyPointToPointNetDevice::ProcessHeader (Ptr<Packet> p, uint16_t& param)
{
  NS_LOG_FUNCTION (this << p << param);
  PppHeader ppp;
  p->RemoveHeader (ppp);
  param = PppToEther (ppp.GetProtocol ());
  return true;
}

void
LzyPointToPointNetDevice::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_node = 0;
  m_channel = 0;
  m_receiveErrorModel = 0;
  m_currentPkt = 0;
  m_queue = 0;
  NetDevice::DoDispose ();
}

void
LzyPointToPointNetDevice::SetDataRate (DataRate bps)
{
  NS_LOG_FUNCTION (this);
  m_bps = bps;
}

void
LzyPointToPointNetDevice::SetInterframeGap (Time t)
{
  NS_LOG_FUNCTION (this << t.GetSeconds ());
  m_tInterframeGap = t;
}

bool
LzyPointToPointNetDevice::TransmitStart (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  NS_LOG_LOGIC ("UID is " << p->GetUid () << ")");

  //
  // This function is called to start the process of transmitting a packet.
  // We need to tell the channel that we've started wiggling the wire and
  // schedule an event that will be executed when the transmission is complete.
  //
  NS_ASSERT_MSG (m_txMachineState == READY, "Must be READY to transmit");
  m_txMachineState = BUSY;
  m_currentPkt = p;
  m_phyTxBeginTrace (m_currentPkt);

  Time txTime = m_bps.CalculateBytesTxTime (p->GetSize ());
  Time txCompleteTime = txTime + m_tInterframeGap;

  NS_LOG_LOGIC ("Schedule TransmitCompleteEvent in " << txCompleteTime.GetSeconds () << "sec");
  Simulator::Schedule (txCompleteTime, &LzyPointToPointNetDevice::TransmitComplete, this);

  bool result = m_channel->TransmitStart (p, this, txTime);
  std::cout<<"the packet size: "<<p->GetSize()<<std::endl;
  if (result == false)
    {
      m_phyTxDropTrace (p);
    }
  return result;
}

void
LzyPointToPointNetDevice::TransmitComplete (void)
{
  NS_LOG_FUNCTION (this);

  //
  // This function is called to when we're all done transmitting a packet.
  // We try and pull another packet off of the transmit queue.  If the queue
  // is empty, we are done, otherwise we need to start transmitting the
  // next packet.
  //
  NS_ASSERT_MSG (m_txMachineState == BUSY, "Must be BUSY if transmitting");
  m_txMachineState = READY;

  NS_ASSERT_MSG (m_currentPkt != 0, "PointToPointNetDevice::TransmitComplete(): m_currentPkt zero");

  m_phyTxEndTrace (m_currentPkt);
  m_currentPkt = 0;
  
  if(m_node->GetNodeType()==1){
         //write data in file
          std::string sstr = "data.txt";
          std::ofstream fout(sstr,std::ios::app);
          fout<<"time: "<<Simulator::Now ().As (Time::S)<<" device: "<<this->GetIfIndex()<<" queue length: "<<m_queue->GetNBytes()<<" threshold "<<m_node->m_switch->GetThreshold();
          fout<<std::endl;   
  }
  
  std::cout<<"666"<<std::endl;
  Ptr<Packet> p = m_queue->Dequeue ();
  std::cout<<"777"<<std::endl;
  if (p == 0)
    {
      NS_LOG_LOGIC ("No pending packets in device queue after tx complete");
      return;
    }else{
      //lzy change
          if(m_node->GetNodeType()==1){
            std::cout<<"flag2"<<std::endl;
          }
          //lzy change end
      if(m_node->GetNodeType()==1){
            m_node->m_switch->DeleteUsed(p->GetSize());
            std::cout<<"----- threshold = "<< m_node->m_switch->GetThreshold()<<std::endl;
          }
    }

  //
  // Got another packet off of the queue, so start the transmit process again.
  //
  m_snifferTrace (p);
  m_promiscSnifferTrace (p);
  TransmitStart (p);
}

bool
LzyPointToPointNetDevice::Attach (Ptr<LzyPointToPointChannel> ch)
{
  NS_LOG_FUNCTION (this << &ch);

  m_channel = ch;

  m_channel->Attach (this);

  //
  // This device is up whenever it is attached to a channel.  A better plan
  // would be to have the link come up when both devices are attached, but this
  // is not done for now.
  //
  NotifyLinkUp ();
  return true;
}

void
LzyPointToPointNetDevice::SetQueue (Ptr<Queue<Packet> > q)
{
  NS_LOG_FUNCTION (this << q);
  m_queue = q;
}

void
LzyPointToPointNetDevice::SetReceiveErrorModel (Ptr<ErrorModel> em)
{
  NS_LOG_FUNCTION (this << em);
  m_receiveErrorModel = em;
}

void
LzyPointToPointNetDevice::Receive (Ptr<Packet> packet)
{ //lzy
  std::cout<<"this is receive function."<<"in node "<<m_node->GetId()<<" and the device is "<<this->GetIfIndex()<<std::endl;
  NS_LOG_FUNCTION (this << packet);
  uint16_t protocol = 0;

  if (m_receiveErrorModel && m_receiveErrorModel->IsCorrupt (packet) ) 
    {
      // 
      // If we have an error model and it indicates that it is time to lose a
      // corrupted packet, don't forward this packet up, let it go.
      //
      m_phyRxDropTrace (packet);
    }
  else 
    {
      // 
      // Hit the trace hooks.  All of these hooks are in the same place in this 
      // device because it is so simple, but this is not usually the case in
      // more complicated devices.
      //
      m_snifferTrace (packet);
      m_promiscSnifferTrace (packet);
      m_phyRxEndTrace (packet);

      //
      // Trace sinks will expect complete packets, not packets without some of the
      // headers.
      //
      Ptr<Packet> originalPacket = packet->Copy ();

      //
      // Strip off the point-to-point protocol header and forward this packet
      // up the protocol stack.  Since this is a simple point-to-point link,
      // there is no difference in what the promisc callback sees and what the
      // normal receive callback sees.
      //
      ProcessHeader (packet, protocol);

      if (!m_promiscCallback.IsNull ())
        {
          m_macPromiscRxTrace (originalPacket);
          m_promiscCallback (this, packet, protocol, GetRemote (), GetAddress (), NetDevice::PACKET_HOST);
        }

      m_macRxTrace (originalPacket);
      m_rxCallback (this, packet, protocol, GetRemote ());
    }
}

Ptr<Queue<Packet> >
LzyPointToPointNetDevice::GetQueue (void) const
{ 
  NS_LOG_FUNCTION (this);
  return m_queue;
}

void
LzyPointToPointNetDevice::NotifyLinkUp (void)
{
  NS_LOG_FUNCTION (this);
  m_linkUp = true;
  m_linkChangeCallbacks ();
}

void
LzyPointToPointNetDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (this);
  m_ifIndex = index;
}

uint32_t
LzyPointToPointNetDevice::GetIfIndex (void) const
{
  return m_ifIndex;
}

Ptr<Channel>
LzyPointToPointNetDevice::GetChannel (void) const
{
  return m_channel;
}

//
// This is a point-to-point device, so we really don't need any kind of address
// information.  However, the base class NetDevice wants us to define the
// methods to get and set the address.  Rather than be rude and assert, we let
// clients get and set the address, but simply ignore them.

void
LzyPointToPointNetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION (this << address);
  m_address = Mac48Address::ConvertFrom (address);
}

Address
LzyPointToPointNetDevice::GetAddress (void) const
{
  return m_address;
}

bool
LzyPointToPointNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION (this);
  return m_linkUp;
}

void
LzyPointToPointNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION (this);
  m_linkChangeCallbacks.ConnectWithoutContext (callback);
}

//
// This is a point-to-point device, so every transmission is a broadcast to
// all of the devices on the network.
//
bool
LzyPointToPointNetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

//
// We don't really need any addressing information since this is a 
// point-to-point device.  The base class NetDevice wants us to return a
// broadcast address, so we make up something reasonable.
//
Address
LzyPointToPointNetDevice::GetBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool
LzyPointToPointNetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

Address
LzyPointToPointNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_LOG_FUNCTION (this);
  return Mac48Address ("01:00:5e:00:00:00");
}

Address
LzyPointToPointNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);
  return Mac48Address ("33:33:00:00:00:00");
}

bool
LzyPointToPointNetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

bool
LzyPointToPointNetDevice::IsBridge (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool
LzyPointToPointNetDevice::Send (
  Ptr<Packet> packet, 
  const Address &dest, 
  uint16_t protocolNumber)
{ //lzy
  std::cout<<"this is send function."<<"in node "<<m_node->GetId()<<" and the device is "<<this->GetIfIndex()<<std::endl;
  NS_LOG_FUNCTION (this << packet << dest << protocolNumber);
  NS_LOG_LOGIC ("p=" << packet << ", dest=" << &dest);
  NS_LOG_LOGIC ("UID is " << packet->GetUid ());

  //
  // If IsLinkUp() is false it means there is no channel to send any packet 
  // over so we just hit the drop trace on the packet and return an error.
  //
  if (IsLinkUp () == false)
    {
      m_macTxDropTrace (packet);
      return false;
    }

  //
  // Stick a point to point protocol header on the packet in preparation for
  // shoving it out the door.
  //
  AddHeader (packet, protocolNumber);

  m_macTxTrace (packet);

  //
  // We should enqueue and dequeue the packet to hit the tracing hooks.
  //
  //lzy
  if(m_node->GetNodeType()==1){
    if(m_queue->GetNBytes()+packet->GetSize() > uint32_t(m_node->m_switch->GetThreshold())){
        std::cout<<"--------drop because threshold!!!!!! and device: "<<this->GetIfIndex()<<std::endl;
        return false;
    }else{
      //std::cout<<"~~~~~~~~time:" << Simulator::Now ().As (Time::S)<<" device: "<<this->GetIfIndex()<<" queue length is: "<<m_queue->GetNBytes()<<"--------"<<std::endl;
      if (m_queue->Enqueue (packet))
        { //std::cout<<"111"<<std::endl;
          std::cout<<"~~~~~~~~time:" << Simulator::Now ().As (Time::S)<<" device: "<<this->GetIfIndex()<<" queue length is: "<<m_queue->GetNBytes()<<"--------"<<std::endl;
          
          
          //write data in file
          std::string sstr = "data.txt";
          std::ofstream fout(sstr,std::ios::app);
          fout<<"time: "<<Simulator::Now ().As (Time::S)<<" device: "<<this->GetIfIndex()<<" queue length: "<<m_queue->GetNBytes()<<" threshold "<<m_node->m_switch->GetThreshold();
          fout<<std::endl;
          


          m_node->m_switch->AddUsed(packet->GetSize());
          std::cout<<"enqueue----- threshold = "<< m_node->m_switch->GetThreshold()<<" and the packet size is "<<packet->GetSize()<<std::endl;
          //
          // If the channel is ready for transition we send the packet right now
          // 
          if (m_txMachineState == READY)
            { 
              std::cout<<"flag1"<<std::endl;



              if(m_node->GetNodeType()==1){
                //write data in file
                  std::string sstr = "data.txt";
                  std::ofstream fout(sstr,std::ios::app);
                  fout<<"time: "<<Simulator::Now ().As (Time::S)<<" device: "<<this->GetIfIndex()<<" queue length: "<<m_queue->GetNBytes()<<" threshold "<<m_node->m_switch->GetThreshold();
                  fout<<std::endl;   
                }



              packet = m_queue->Dequeue ();
              m_node->m_switch->DeleteUsed(packet->GetSize());
              std::cout<<"dequeue----- threshold = "<< m_node->m_switch->GetThreshold()<<std::endl;
              m_snifferTrace (packet);
              m_promiscSnifferTrace (packet);
              bool ret = TransmitStart (packet);
              return ret;
            }
          return true;
        }
    }

  }else{
    std::cout<<"111"<<std::endl;
         bool flag = m_queue->Enqueue (packet);
         std::cout<<flag<<std::endl;
      if (flag)
        {std::cout<<"222"<<std::endl;
          //
          // If the channel is ready for transition we send the packet right now
          // 
          if (m_txMachineState == READY)
            {
              packet = m_queue->Dequeue ();
              std::cout<<"333"<<std::endl;
              m_snifferTrace (packet);
              m_promiscSnifferTrace (packet);
              std::cout<<"444"<<std::endl;
              bool ret = TransmitStart (packet);
              std::cout<<"555"<<std::endl;
              std::cout<<ret<<std::endl;
              return ret;
            }
          return true;
        }
  }
  

  // Enqueue may fail (overflow)
  std::cout<<"Enqueue may fail (overflow)"<<std::endl;
  m_macTxDropTrace (packet);
  return false;
}

bool
LzyPointToPointNetDevice::SendFrom (Ptr<Packet> packet, 
                                 const Address &source, 
                                 const Address &dest, 
                                 uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << source << dest << protocolNumber);
  return false;
}

Ptr<Node>
LzyPointToPointNetDevice::GetNode (void) const
{
  return m_node;
}

void
LzyPointToPointNetDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this);
  m_node = node;
}

bool
LzyPointToPointNetDevice::NeedsArp (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

void
LzyPointToPointNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

void
LzyPointToPointNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  m_promiscCallback = cb;
}

bool
LzyPointToPointNetDevice::SupportsSendFrom (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

void
LzyPointToPointNetDevice::DoMpiReceive (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  Receive (p);
}

Address 
LzyPointToPointNetDevice::GetRemote (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_channel->GetNDevices () == 2);
  for (std::size_t i = 0; i < m_channel->GetNDevices (); ++i)
    {
      Ptr<NetDevice> tmp = m_channel->GetDevice (i);
      if (tmp != this)
        {
          return tmp->GetAddress ();
        }
    }
  NS_ASSERT (false);
  // quiet compiler.
  return Address ();
}

bool
LzyPointToPointNetDevice::SetMtu (uint16_t mtu)
{
  NS_LOG_FUNCTION (this << mtu);
  m_mtu = mtu;
  return true;
}

uint16_t
LzyPointToPointNetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION (this);
  return m_mtu;
}

uint16_t
LzyPointToPointNetDevice::PppToEther (uint16_t proto)
{
  NS_LOG_FUNCTION_NOARGS();
  switch(proto)
    {
    case 0x0021: return 0x0800;   //IPv4
    case 0x0057: return 0x86DD;   //IPv6
    default: NS_ASSERT_MSG (false, "PPP Protocol number not defined!");
    }
  return 0;
}

uint16_t
LzyPointToPointNetDevice::EtherToPpp (uint16_t proto)
{
  NS_LOG_FUNCTION_NOARGS();
  switch(proto)
    {
    case 0x0800: return 0x0021;   //IPv4
    case 0x86DD: return 0x0057;   //IPv6
    default: NS_ASSERT_MSG (false, "PPP Protocol number not defined!");
    }
  return 0;
}


} // namespace ns3
