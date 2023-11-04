#include "ns3/ipv4-header.h"
#include "iostream"
using namespace ns3;

int main(int argc,char* agrv[]){
    std::cout<<"test header"<<std::endl;
    Ipv4Header h;
    h.SetTos(2);
    uint8_t a = h.GetTos();
    std::cout<<a<<std::endl;
    std::cout<<h.GetSource()<<std::endl;

}