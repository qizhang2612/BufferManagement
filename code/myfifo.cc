#include "ns3/test.h"
#include "ns3/fifo-queue-disc.h"
#include "ns3/queue.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/object-factory.h"
#include <vector>
#include "iostream"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"
//#include "/home/lzy0129/work/ns-allinone-3.32/ns-3.32/src/network/model/broadcom-node.h"

using namespace ns3;

class FifoQueueDiscTestItem : public QueueDiscItem
{
public:
  /**
   * Constructor
   *
   * \param p the packet
   * \param addr the address
   */
  FifoQueueDiscTestItem (Ptr<Packet> p, const Address & addr);
  virtual ~FifoQueueDiscTestItem ();
  virtual void AddHeader (void);
  virtual bool Mark (void);

private:
  FifoQueueDiscTestItem ();
  /**
   * \brief Copy constructor
   * Disable default implementation to avoid misuse
   */
  FifoQueueDiscTestItem (const FifoQueueDiscTestItem &);
  /**
   * \brief Assignment operator
   * \return this object
   * Disable default implementation to avoid misuse
   */
  FifoQueueDiscTestItem &operator = (const FifoQueueDiscTestItem &);
};

FifoQueueDiscTestItem::FifoQueueDiscTestItem (Ptr<Packet> p, const Address & addr)
  : QueueDiscItem (p, addr, 0)
{
}

FifoQueueDiscTestItem::~FifoQueueDiscTestItem ()
{
}

void
FifoQueueDiscTestItem::AddHeader (void)
{
}

bool
FifoQueueDiscTestItem::Mark (void)
{
  return false;
}


int main(int argc, char *argv[])
{
    //Ptr<BroadcomNode> myswitch = CreateObject<BroadcomNode>();





    std::cout<<"this is my fifoqueuedisc."<<std::endl;
    Ptr<FifoQueueDisc> queue;
    queue = CreateObject<FifoQueueDisc> ();
    std::cout<<"the queue disc has "<<queue->GetNInternalQueues ()<<" internal queue"<<std::endl;


    uint32_t numPackets = 10;
    uint32_t pktSize = 1000;
    QueueSizeUnit mode = QueueSizeUnit::PACKETS;
    uint32_t modeSize = (mode == QueueSizeUnit::PACKETS ? 1 : pktSize);
    //queue->SetAttributeFailSafe ("MaxSize",QueueSizeValue (QueueSize (mode, numPackets*modeSize)));
    queue->SetAttributeFailSafe ("MaxSize",QueueSizeValue (QueueSize (mode, 0*modeSize)));



    ObjectFactory factory;
    factory.SetTypeId ("ns3::DropTailQueue<QueueDiscItem>");
    if (mode == QueueSizeUnit::PACKETS)
        {
        factory.Set ("MaxSize", QueueSizeValue (QueueSize (QueueSizeUnit::PACKETS, 10)));
        }
    else
        {
        factory.Set ("MaxSize", QueueSizeValue (QueueSize (QueueSizeUnit::BYTES, numPackets*pktSize)));
        }
    queue->AddInternalQueue (factory.Create<QueueDisc::InternalQueue> ());


    //queue->AddInternalQueue (factory.Create<QueueDisc::InternalQueue> ());



    Ptr<FifoQueueDisc> q = queue;
    //uint32_t qSize = numPackets*modeSize;

    queue->Initialize ();
    std::cout<<"the queue disc has "<<queue->GetNInternalQueues ()<<" internal queue"<<std::endl;
    //test1 enqueue and dequeue in one internal queue.
    std::vector<uint64_t> uids;
    Ptr<Packet> p;
    Ptr<QueueDiscItem> item;
    Address dest;
    //uint32_t modeSize = (q->GetMaxSize ().GetUnit () == QueueSizeUnit::PACKETS ? 1 : pktSize);
    
    std::cout<<"the queue value is "<<q->GetCurrentSize ().GetValue ()<<std::endl;
    std::cout<<"the queue disc has "<<q->GetNInternalQueues ()<<" internal queue"<<std::endl;

    for (uint32_t i = 1; i <= numPackets; i++)
    {
      p = Create<Packet> (pktSize);
      uids.push_back (p->GetUid ());
      q->Enqueue (Create<FifoQueueDiscTestItem> (p, dest));
      //NS_TEST_EXPECT_MSG_EQ (q->GetCurrentSize ().GetValue (), i * modeSize, "There should be " << i << " packet(s) in there");
      std::cout<<"the value of the queue is "<<q->GetCurrentSize ().GetValue ()<<" and there should be "<<i << " packet(s) in there"<<std::endl;
    }
    std::cout<<"the byte in internal queue "<<q->GetInternalQueue (0)->GetNBytes ()<<std::endl;
    std::cout<<"the byte in internal queue "<<q->GetInternalQueue (0)->GetNPackets ()<<std::endl;

    bool flag = q->Enqueue (Create<FifoQueueDiscTestItem> (p, dest));
    std::cout<<flag<<std::endl;
    for (uint32_t i = 1; i <= numPackets; i++)
    {
      item = q->Dequeue ();
      if(item!=0){
          std::cout<<"A packet should have been dequeued"<<std::endl;
      }
    // std::cout<<"There should be " << q->GetCurrentSize ().GetValue () << " packet(s) in there"<<std::endl;
    std::cout<<"There should be " << q->GetCurrentSize () << " packet(s) in there"<<std::endl;
    std::cout<<item->GetPacket ()->GetUid ()<<" was this the right packet"<<std::endl;



    //Ptr<BroadcomNode> switchnode;

    
      
    }
    return 0;
}
