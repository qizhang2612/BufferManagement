#include "multiple-queue.h"
#include "drop-tail-queue.h"
#include "ns3/simulator.h"
#include <iostream>
#include <fstream>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MultipleQueue");

NS_OBJECT_TEMPLATE_CLASS_DEFINE (MultipleQueue,Packet);
NS_OBJECT_TEMPLATE_CLASS_DEFINE (MultipleQueue,QueueDiscItem);

/**
 * Implementation of the templates declared above.
 */

template <typename Item>
TypeId
MultipleQueue<Item>::GetTypeId (void)
{
  static TypeId tid = TypeId (("ns3::MultipleQueue<" + GetTypeParamName<MultipleQueue<Item> > () + ">").c_str ())
    .SetParent<Queue<Item> > ()
    .SetGroupName ("Network")
    .template AddConstructor<MultipleQueue<Item> > ()
    .AddAttribute ("MaxSize",
                   "The max queue size",
                   QueueSizeValue (QueueSize ("100000000p")),
                   MakeQueueSizeAccessor (&QueueBase::SetMaxSize,
                                          &QueueBase::GetMaxSize),
                   MakeQueueSizeChecker ())
    .AddAttribute ("useqCnt",
                  "the subqueue numbers that use for test",
                  UintegerValue(8),
                  MakeUintegerAccessor(&MultipleQueue::m_useqCnt),
                  MakeUintegerChecker<uint32_t>())
    .AddAttribute ("dequeueType",
                  "the type used for dequeue",
                  UintegerValue(0),
                  MakeUintegerAccessor(&MultipleQueue::m_dequeueType),
                  MakeUintegerChecker<uint32_t>())
    .AddAttribute ("enqueueType",
                  "the type used for dequeue",
                  UintegerValue(0),
                  MakeUintegerAccessor(&MultipleQueue::m_enqueueType),
                  MakeUintegerChecker<uint32_t>())
    .AddAttribute ("po",
                  "use po or not ,0 means use dt ,1 means use po",
                  UintegerValue(0),
                  MakeUintegerAccessor(&MultipleQueue::po),
                  MakeUintegerChecker<uint32_t>())
    .AddAttribute ("w0",
                  "weight 0 for DRR",
                  UintegerValue(150),
                  MakeUintegerAccessor(&MultipleQueue::w0),
                  MakeUintegerChecker<uint32_t>())
    .AddAttribute ("w1",
                  "weight 1 for DRR",
                  UintegerValue(250),
                  MakeUintegerAccessor(&MultipleQueue::w1),
                  MakeUintegerChecker<uint32_t>())
    .AddAttribute ("w2",
                  "weight 2 for DRR",
                  UintegerValue(50),
                  MakeUintegerAccessor(&MultipleQueue::w2),
                  MakeUintegerChecker<uint32_t>())
  ;
  return tid;
}

template <typename Item>
MultipleQueue<Item>::MultipleQueue () :
  Queue<Item> (),
  NS_LOG_TEMPLATE_DEFINE ("MultipleQueue")
{
  if(m_showflag==true){
      std::cout<<"this is multiple queue."<<std::endl;
  }
  for (uint32_t i = 0; i < qCnt; i++)            //create the subqueue
  {
	  m_queues.push_back(CreateObject<DropTailQueue<Item> >());
    m_queues[i]->SetAttribute("MaxSize",QueueSizeValue (QueueSize (QueueSizeUnit::PACKETS,10000000)));
  }


  NS_LOG_FUNCTION (this);
}

template <typename Item>
MultipleQueue<Item>::~MultipleQueue ()
{
  if(m_showflag==true){
      std::cout<<"destroy, multiple queue."<<std::endl;
  }
  NS_LOG_FUNCTION (this);
}


//simple enqueue
template <typename Item>
bool
MultipleQueue<Item>::Enqueue (Ptr<Item> item)
{
  //NS_LOG_FUNCTION (this << item);
  //return DoEnqueue (end (), item);
  //--------------------------------------


  //std::cout<<"this is enqueue1 func."<<std::endl;
  //bool flag = m_queues[0]->Enqueue(item);

  //method 1
  //always use subqueue 0
  //bool flag = this->Enqueue(item,0);


  //method 2 use subqueue 0,1 round enqueue

  bool flag;
  if(m_enqueueType==0){           //RR
    uint32_t tempindex = (m_enindex+1)%(this->GetUseqCnt());
    flag = this->Enqueue(item,tempindex);
  }else if(m_enqueueType==1){     //TOS          
    flag = this->EnqueueTos(item);
  }else{
    std::cout<<"the enqueue type is wrong."<<std::endl;
    flag = false;
  }
  
  


  // std::string sstr = "queue.txt";
  // std::ofstream fout(sstr,std::ios::app);
  // fout<<"time: "<<Simulator::Now ().As (Time::S)<<" queue no: "<<tempindex<<" queue length: "<<this->m_queues[tempindex]->GetNBytes()<<" threshold "<<this->GetThreshold()<<" enqueueflag "<<flag;
  // fout<<std::endl;

  //bool flag0 = this->Enqueue(item,0);
  //bool flag1 = this->Enqueue(item,1);

  // if(flag==true){
  //   std::cout<<"subqueue 0 enqueue1 successful"<<std::endl;
  // }else{
  //   std::cout<<"subqueue 0 enqueue1 fail"<<std::endl;
  // }
  

  
  return flag;
}

template <typename Item>
bool
MultipleQueue<Item>::EnqueueTos (Ptr<Item> item){
  uint32_t tempindex = (this->GetTos())%(this->GetUseqCnt());
  bool flag = this->Enqueue(item,tempindex);
  return flag;
}

template <typename Item>
bool
MultipleQueue<Item>::Enqueue (Ptr<Item> item,uint32_t index)
{
  //NS_LOG_FUNCTION (this << item);
  //return DoEnqueue (end (), item);
  //--------------------------------------
  //use the threshold

  //std::cout<<"this is enqueue2 func."<<std::endl;

  if(index<0||index>=qCnt){
    std::cout<<"enqueue2 func fails, because the index is invalid."<<std::endl;
    return false;
  }
  std::cout<<"nnnnnnnnnnnnnnnn  in the multiple queue, the threshold is: "<<threshold<<std::endl;
  if(po==0){
    if(m_queues[index]->GetNBytes()+item->GetSize()>threshold){

      // inorder to enqueue in turns ,if queue 1 enqueue fails ,also need to change the enindex
      SetEnIndex(index);
      std::cout<<"*****enqueue2 func fails, because the threshold"<<std::endl;
      return false;
    }
  }
  

  bool flag = m_queues[index]->Enqueue(item);
  if(flag==true){
    //std::cout<<"subqueue 0 enqueue2 successful"<<std::endl;
    SetEnIndex(index);

  }else{
    //I think there is something wrong. so I add setenindex
    SetEnIndex(index);
    //std::cout<<"subqueue 0 enqueue2 fail"<<std::endl;
  }
  
  //change multiple queue message

  if(flag==true){
    this->SetMNBytes(this->GetNBytes()+item->GetSize());
    this->SetMNPackets(this->GetNPackets()+1);
  }

  
  return flag;
}

//simple dequeue
template <typename Item>
Ptr<Item>
MultipleQueue<Item>::Dequeue (void)
{
  //NS_LOG_FUNCTION (this);
  //Ptr<Item> item = DoDequeue (begin ());

  //NS_LOG_LOGIC ("Popped " << item);

  //return item;
  //-------------------------------------------


  //std::cout<<"this is dequeue1 func."<<std::endl;
  //Ptr<Item> lzyitem = m_queues[0]->Dequeue();


  //method 1, always use the subqueue 0
  //Ptr<Item> lzyitem = this->Dequeue(0);


  if(m_dequeueType==0){                     //RR
  //method 2, use subqueue 0,1;
    Ptr<Item> lzyitem = 0; 
    for(uint32_t i=1;i<=this->GetUseqCnt();i++){
      uint32_t tempindex = (m_deindex+i)%(this->GetUseqCnt());
      lzyitem = this->Dequeue(tempindex);
      if(lzyitem!=0){
        break;
      }
    }

    if(lzyitem!=0){
      //std::cout<<"subqueue 0 dequeue1 successful"<<std::endl;
    }else{
      //std::cout<<"subqueue 0 dequeue1 fail"<<std::endl;
    }

    
    return lzyitem;
  }else if(m_dequeueType==1){               //PQ
    Ptr<Item> lzyitem = 0; 
    lzyitem = this->DequeuePQ();
    return lzyitem;
  }else if(m_dequeueType==2){               //DRR
    Ptr<Item> lzyitem = 0; 
    lzyitem = this->DequeueDRR();          
    return lzyitem;
  }else{
    return 0;
  }

  
}


template <typename Item>
Ptr<Item>
MultipleQueue<Item>::DequeuePQ (void)
{

  Ptr<Item> lzyitem = 0; 
  for(uint32_t i=0;i<this->GetUseqCnt();i++){
    
    lzyitem = this->Dequeue(i);
    if(lzyitem!=0){
      break;
    }
  }

  if(lzyitem!=0){
    //std::cout<<"subqueue 0 dequeue1 successful"<<std::endl;
  }else{
    //std::cout<<"subqueue 0 dequeue1 fail"<<std::endl;
  }

  
  return lzyitem;
}


template <typename Item>
Ptr<Item>
MultipleQueue<Item>::DequeueDRR (void)
{
  // m_weight[0] = w0;
  // m_weight[1] = w1;
  // m_weight[2] = w2;

  Ptr<Item> lzyitem=0;
  while(this->GetNBytes()>0){
    lzyitem = DequeueDRR2();
    if(lzyitem!=0){
      break;
    }else{
      //m_deindex--;
    }
    //std::cout<<"flag1   "<<this->GetNBytes()<<std::endl;
  }
  return lzyitem;
}


template <typename Item>
Ptr<Item>
MultipleQueue<Item>::DequeueDRR2 (void)
{
  //std::cout<<"flag2"<<std::endl;
  Ptr<Item> lzyitem = 0; 
  for(uint32_t i=1;i<=this->GetUseqCnt();i++){
    uint32_t tempindex = (m_deindex+i)%(this->GetUseqCnt());
    m_tempweight[tempindex] += m_weight[tempindex];

    //std::cout<<"m_deindex "<<m_deindex<<" tempweight: "<<tempindex<<"  "<<m_tempweight[tempindex] <<std::endl;
    

    if(m_tempweight[tempindex]>0){
      std::cout<<"index "<<tempindex<<" weight "<<m_tempweight[tempindex]<<std::endl;;
      lzyitem = this->Dequeue(tempindex);
      if(lzyitem!=0){
        m_tempweight[tempindex] -= lzyitem->GetSize();
        break;
      }
    }
  }

  if(lzyitem!=0){
    //std::cout<<"subqueue 0 dequeue1 successful"<<std::endl;
  }else{
    //std::cout<<"don't find"<<std::endl;
  }

  
  return lzyitem;
}


// template <typename Item>
// Ptr<Item>
// MultipleQueue<Item>::DequeueWRR (void)
// {
//   Ptr<Item> lzyitem=0;
//   while(this->GetNBytes()>0){
//     lzyitem = DequeueWRR1();
//     if(lzyitem!=0){
//       break;
//     }
//   }
//   return lzyitem;
// }

// template <typename Item>
// Ptr<Item>
// MultipleQueue<Item>::DequeueWRR1 (void)
// {

//   if(this->IsZero(m_tempweight)==1){
//     this->GetWeight(m_tempweight);
//     m_deindex = -1;
//   }

//   for(uint32_t i=0;i<qCnt;i++){
//     std::cout<<" "<<m_tempweight[i];
//   }
//   std::cout<<std::endl;

//   Ptr<Item> lzyitem = 0; 
//   for(uint32_t i=1;i<this->GetUseqCnt();i++){
//       uint32_t tempindex = (m_deindex+i)%(this->GetUseqCnt());

//     // first check bytes, then check weight
//     if(m_queues[tempindex]->GetNBytes()>0){
//       if(m_tempweight[tempindex]>0){
//         m_tempweight[tempindex]--;
//         lzyitem = this->Dequeue(tempindex);
//         if(lzyitem!=0){

//           break;
//         }
//       }
//     }else{
//       if(m_tempweight[tempindex]>0){
//         m_tempweight[tempindex]--;
//       }
//     }
    
    
    
//   }

//   if(lzyitem==0){
//     this->GetWeight(m_tempweight);
//     m_deindex = -1;
//   }

  
//   return lzyitem;
// }






template <typename Item>
Ptr<Item>
MultipleQueue<Item>::Dequeue (uint32_t index)
{
  //NS_LOG_FUNCTION (this);
  //Ptr<Item> item = DoDequeue (begin ());

  //NS_LOG_LOGIC ("Popped " << item);

  //return item;
  //-------------------------------------------


  //std::cout<<"this is dequeue func2."<<std::endl;

  if(index<0||index>=qCnt){
    std::cout<<"dequeue func2 fails, because the index is invalid."<<std::endl;
    return 0;
  }

  Ptr<Item> lzyitem = m_queues[index]->Dequeue();
  if(lzyitem!=0){
    //std::cout<<"subqueue 0 dequeue2 successful"<<std::endl;
    SetDeIndex(index); //change the deindex

  }else{
    //std::cout<<"subqueue 0 dequeue2 fail"<<std::endl;
  }
  if(lzyitem!=0){
    this->SetMNBytes(this->GetNBytes()-lzyitem->GetSize());
    this->SetMNPackets(this->GetNPackets()+1);
  }

  
  return lzyitem;
}



template <typename Item>
Ptr<Item>
MultipleQueue<Item>::Remove (void)
{
  //NS_LOG_FUNCTION (this);

  //Ptr<Item> item = DoRemove (begin ());
  
  //NS_LOG_LOGIC ("Removed " << item);

  //return item;
  //-------------------------------

  Ptr<Item> lzyitem = m_queues[0]->Remove();
  return lzyitem;
}

template <typename Item>
Ptr<Item>
MultipleQueue<Item>::Remove (uint32_t index)
{
  //NS_LOG_FUNCTION (this);

  //Ptr<Item> item = DoRemove (begin ());
  
  //NS_LOG_LOGIC ("Removed " << item);

  //return item;
  //-------------------------------
  if(index<0||index>=m_useqCnt){
    std::cout<<"index is invalid."<<std::endl;
    return 0;
  }

  Ptr<Item> lzyitem = m_queues[index]->Remove();
  return lzyitem;
}

template <typename Item>
Ptr<const Item>
MultipleQueue<Item>::Peek (void) const
{
  //NS_LOG_FUNCTION (this);

  //return DoPeek (begin ());
  //---------------------------


  return m_queues[0]->Peek();
}

template <typename Item>
Ptr<const Item>
MultipleQueue<Item>::Peek (uint32_t index) const
{
  //NS_LOG_FUNCTION (this);

  //return DoPeek (begin ());
  //---------------------------
  if(index<0||index>=m_useqCnt){
    std::cout<<"index is invalid."<<std::endl;
    return 0;
  }

  return m_queues[index]->Peek();
}


template <typename Item>
void
MultipleQueue<Item>::SetThreshold(double th)
{
  threshold = th;
}

template <typename Item>
double
MultipleQueue<Item>::GetThreshold()
{
  return threshold;
}

template <typename Item>
void
MultipleQueue<Item>::SetEnIndex(uint32_t enindex)
{
  m_enindex = enindex;
}

template <typename Item>
uint32_t
MultipleQueue<Item>::GetEnIndex()
{
  return m_enindex;
}

template <typename Item>
void
MultipleQueue<Item>::SetDeIndex(uint32_t deindex)
{
  m_deindex = deindex;
}

template <typename Item>
uint32_t
MultipleQueue<Item>::GetDeIndex()
{
  return m_deindex;
}

template <typename Item>
void
MultipleQueue<Item>::SetUseqCnt(uint32_t useqCnt)
{
  m_useqCnt = useqCnt;
}

template <typename Item>
uint32_t
MultipleQueue<Item>::GetUseqCnt()
{
  return m_useqCnt;
}

template <typename Item>
void
MultipleQueue<Item>::SetDequeueType(uint32_t dequeueType)
{
  m_dequeueType = dequeueType;
}

template <typename Item>
uint32_t
MultipleQueue<Item>::GetDequeueType()
{
  return m_dequeueType;
}

template <typename Item>
void
MultipleQueue<Item>::SetEnqueueType(uint32_t enqueueType)
{
  m_enqueueType = enqueueType;
}

template <typename Item>
uint32_t
MultipleQueue<Item>::GetEnqueueType()
{
  return m_enqueueType;
}


template <typename Item>
void
MultipleQueue<Item>::SetWeight(int weight[])
{
  for(uint32_t i=0;i<m_useqCnt;i++){
    m_weight[i] = weight[i];
  }
}

template <typename Item>
void
MultipleQueue<Item>::GetWeight(int weight[])
{
  for(uint32_t i=0;i<m_useqCnt;i++){
    weight[i] = m_weight[i];
  }
}



template <typename Item>
void
MultipleQueue<Item>::SetTos(uint32_t tos)
{
  m_tos = tos;
}

template <typename Item>
uint32_t
MultipleQueue<Item>::GetTos()
{
  return m_tos;
}

// template <typename Item>
// bool
// MultipleQueue<Item>::IsZero(int weight[])
// {
//   for(uint32_t i=0;i<m_useqCnt;i++){
//     if(weight[i]!=0){
//       return false;
//     }
//   }
//   return true;
// }




// The following explicit template instantiation declarations prevent all the
// translation units including this header file to implicitly instantiate the
// DropTailQueue<Packet> class and the DropTailQueue<QueueDiscItem> class. The
// unique instances of these classes are explicitly created through the macros
// NS_OBJECT_TEMPLATE_CLASS_DEFINE (DropTailQueue,Packet) and
// NS_OBJECT_TEMPLATE_CLASS_DEFINE (DropTailQueue,QueueDiscItem), which are included
// in drop-tail-queue.cc
extern template class MultipleQueue<Packet>;
extern template class MultipleQueue<QueueDiscItem>;

} // namespace ns3