#include "ns3/switch.h"
#include "iostream"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "iostream"
using namespace ns3;

int main(int argc,char* agrv[]){
    std::cout<<"this is lzy switch class "<<std::endl;
    
    Ptr<Switch> s = CreateObject<Switch>();
    int th = 0;
    th = s->GetThreshold();
    std::cout<<"now the threshold "<<th<<std::endl;

    s->AddUsed(1000);
    th = s->GetThreshold();
    std::cout<<"a packet which size is 1000 enqueues  "<<th<<std::endl;
    std::cout<<"now the threshold "<<th<<std::endl;

    std::cout<<"-------------------------------"<<std::endl;

    Ptr<Node> lzy = CreateObject<Node>();
    std::cout<<lzy->GetNodeType()<<std::endl;
    lzy->SetNodeType(1);
    
    std::cout<<lzy->m_switch->GetThreshold()<<std::endl;
    

    return 0;

}