#include "ns3/drop-tail-queue.h"
#include "ns3/packet.h"
#include "ns3/multiple-queue.h"
#include "ns3/lzy-point-to-point-net-device.h"


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"



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


    //Config::SetDefault ("ns3::MultipleQueue::m_useqCnt", UintegerValue (2));
    Ptr<MultipleQueue<Packet> >lzyqueue = CreateObject<MultipleQueue<Packet> >();
    std::cout<<lzyqueue->GetMaxSize ()<<std::endl;
    lzyqueue->SetAttribute("useqCnt",StringValue ("3"));

    int a[8] = {0,0,0,0,0,0,0,0};
    lzyqueue->GetWeight(a);
    for(uint32_t i=0;i<8;i++){
        std::cout<<a[i]<<" ";
    }
    // int b[8] = {1,1,1,0,0,0,0,0};
    // lzyqueue->SetWeight(b);

    // lzyqueue->GetWeight(a);
    // for(uint32_t i=0;i<8;i++){
    //     std::cout<<a[i]<<" ";
    // }
    // std::cout<<"----------------------------------------------------------------------------"<<std::endl;
    // a[0]--;
    // a[2]--;
    // a[1]--;
    // std::cout<<lzyqueue->IsZero(a)<<std::endl;

    // std::cout<<p1->GetUid()<<std::endl;
    // lzyqueue->Enqueue(p1,0);
    // Ptr<const Packet> lzy = lzyqueue->Peek(0);
    // std::cout<<lzy->GetUid()<<std::endl;

    // s


    lzyqueue->Enqueue(p1);
    lzyqueue->Enqueue(p1);
    lzyqueue->Enqueue(p1);
    lzyqueue->Enqueue(p1);
    lzyqueue->Enqueue(p1);
    lzyqueue->Enqueue(p1);
    lzyqueue->Enqueue(p1);
    lzyqueue->Enqueue(p1);
    lzyqueue->Enqueue(p1);

    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;

    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;
    lzyqueue->DequeueWRR();
    std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;



    // lzyqueue->Dequeue();
    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 2 has "<<lzyqueue->m_queues[2]->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 3 has "<<lzyqueue->m_queues[3]->GetNBytes()<<" bytes"<<std::endl;





    
    std::cout<<"the num used for testing is: "<<lzyqueue->GetUseqCnt()<<std::endl;
    
    
    // lzyqueue->Enqueue(p1);
    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;

    // lzyqueue->SetDeIndex(0);

    // lzyqueue->Dequeue();
    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;
    


    // lzyqueue->Enqueue(p1);
    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;

    // lzyqueue->Enqueue(p1);
    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;

    // lzyqueue->Enqueue(p1);
    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;

    // //std::cout<<"the enqueue index is: "<<lzyqueue->GetEnIndex()<<std::endl;

    // lzyqueue->Dequeue();
    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;

    // lzyqueue->Dequeue();
    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;

    // lzyqueue->Dequeue();
    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;
    // std::cout<<"subqueue 1 has "<<lzyqueue->m_queues[1]->GetNBytes()<<" bytes"<<std::endl;




    // std::cout<<"lzyqueue has "<<lzyqueue->GetNBytes()<<" bytes"<<std::endl;

    // std::cout<<"lzyqueue has "<<lzyqueue->GetNPackets()<<" packets"<<std::endl;

    // std::cout<<"subqueue 0 has "<<lzyqueue->m_queues[0]->GetNBytes()<<" bytes"<<std::endl;

    // std::cout<<"the enqueue index is: "<<lzyqueue->GetEnIndex()<<std::endl;


    
    // std::cout<<"the threshold hold in the "<<lzyqueue->GetThreshold()<<std::endl;
    // lzyqueue->SetThreshold(1000);
    // std::cout<<"the threshold hold in the "<<lzyqueue->GetThreshold()<<std::endl;

    // lzyqueue->m_queues[0]->Enqueue(p1);
    // std::cout<<"the size of subqueue 1 is: "<<lzyqueue->m_queues[0]->GetNBytes()<<std::endl;



    // std::cout<<"the showfunctinflag is "<<lzyqueue->GetShowFunctionFlag()<<std::endl;

    // lzyqueue->SetFuctionFlag(true);
    // std::cout<<"the showfunctinflag is "<<lzyqueue->GetShowFunctionFlag()<<std::endl;
    
    // //lzyqueue->m_queues[0]->Enqueue(p1);
    // lzyqueue->Enqueue(p1);
    // std::cout<<lzyqueue->m_queues[0]->GetNBytes ()<<std::endl;

    // std::cout<<lzyqueue->GetNBytes()<<std::endl;

    // std::cout<<lzyqueue->m_nBytes<<std::endl;

    // Ptr<Packet> p2 = lzyqueue->Dequeue();
    // std::cout<<lzyqueue->m_queues[0]->GetNBytes ()<<std::endl;

    // std::cout<<lzyqueue->m_nBytes<<std::endl;
    // std::cout<<p2->GetSize()<<std::endl;


    //Ptr<LzyPointToPointNetDevice> mydev = CreateObject<LzyPointToPointNetDevice>();
    //mydev->SetQueue(lzyqueue);



    return 0;
}