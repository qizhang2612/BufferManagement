#include "ns3/drop-tail-queue.h"
#include "ns3/packet.h"
#include "ns3/multiple-queue.h"
#include "ns3/lzy-point-to-point-net-device.h"

using namespace ns3;
int main(int argc,char*argv[]){


    Ptr<DropTailQueue<Packet>> queue = CreateObject<DropTailQueue<Packet> >();
    Ptr<Packet> p1;
    p1 = Create<Packet> (100);

    queue->SetAttribute("MaxSize",QueueSizeValue (QueueSize (QueueSizeUnit::PACKETS,10000000)));
    std::cout<<queue->GetNPackets ()<<std::endl;
    std::cout<<queue->GetMaxSize ()<<std::endl;
    
    queue->Enqueue(p1);
    std::cout<<queue->GetNPackets ()<<std::endl;
    std::cout<<queue->GetNBytes ()<<std::endl;

    std::cout<<"----------------------------------------------------------------------------"<<std::endl;
    Ptr<MultipleQueue<Packet> >lzyqueue = CreateObject<MultipleQueue<Packet> >();
    std::cout<<"the showfunctinflag is "<<lzyqueue->GetShowFunctionFlag()<<std::endl;

    lzyqueue->SetFuctionFlag(true);
    std::cout<<"the showfunctinflag is "<<lzyqueue->GetShowFunctionFlag()<<std::endl;
    
    //lzyqueue->m_queues[0]->Enqueue(p1);
    lzyqueue->Enqueue(p1);
    std::cout<<lzyqueue->m_queues[0]->GetNBytes ()<<std::endl;

    std::cout<<lzyqueue->GetNBytes()<<std::endl;

    std::cout<<lzyqueue->m_nBytes<<std::endl;

    Ptr<Packet> p2 = lzyqueue->Dequeue();
    std::cout<<lzyqueue->m_queues[0]->GetNBytes ()<<std::endl;

    std::cout<<lzyqueue->m_nBytes<<std::endl;
    std::cout<<p2->GetSize()<<std::endl;


    //Ptr<LzyPointToPointNetDevice> mydev = CreateObject<LzyPointToPointNetDevice>();
    //mydev->SetQueue(lzyqueue);



    return 0;
}