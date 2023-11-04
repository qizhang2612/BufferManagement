#include "ns3/log.h"
#include "switch.h"
#include "iostream"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Switch");

NS_OBJECT_ENSURE_REGISTERED (Switch);

TypeId Switch::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Switch")
    .SetParent<Object> ()
    .SetGroupName("Network")
  ;
  return tid;
}

Switch::Switch(){
    m_alpha = 0.5;
    m_maxbuffer = 10000;
    m_usedbuffer = 0;
    m_threshold = m_alpha*(m_maxbuffer-m_usedbuffer);
}

Switch::~Switch(){
    NS_LOG_FUNCTION (this);
}

int Switch::GetThreshold(){
    return int(m_threshold);
}

bool Switch::GetPO(){
    return m_po;
}

void Switch::SetPO(bool po){
    m_po = po;
}

void Switch::Calculate(){
    m_threshold = m_alpha*(m_maxbuffer-m_usedbuffer);
}


void Switch::Set(double alpha,int maxbuffer,int usedbuffer){
    m_alpha = alpha;
    m_maxbuffer = maxbuffer;
    m_usedbuffer = usedbuffer;
    Calculate();              //calculate the threshold ;
}

void Switch::AddUsed(int size){      //this means a packet enqueue
    m_usedbuffer = m_usedbuffer + size;
    Calculate();
}

void Switch::DeleteUsed(int size){
    m_usedbuffer = m_usedbuffer - size;
    Calculate();
}













} 



// namespace ns3