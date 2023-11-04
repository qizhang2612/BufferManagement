#include "multiple-queue.h"
#include "drop-tail-queue.h"
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
  uint32_t tempindex = (m_enindex+1)%2;
  bool flag = this->Enqueue(item,tempindex);


  // if(flag==true){
  //   std::cout<<"subqueue 0 enqueue1 successful"<<std::endl;
  // }else{
  //   std::cout<<"subqueue 0 enqueue1 fail"<<std::endl;
  // }
  

  
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
  if(m_queues[index]->GetNBytes()+item->GetSize()>threshold){
    std::cout<<"*****enqueue2 func fails, because the threshold"<<std::endl;
    return false;
  }

  bool flag = m_queues[index]->Enqueue(item);
  if(flag==true){
    //std::cout<<"subqueue 0 enqueue2 successful"<<std::endl;
    SetEnIndex(index);

  }else{
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


  //method 2, use subqueue 0,1;
  Ptr<Item> lzyitem = 0; 
  for(uint32_t i=1;i<=2;i++){
    uint32_t tempindex = (m_deindex+i)%2;
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
}

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
Ptr<const Item>
MultipleQueue<Item>::Peek (void) const
{
  //NS_LOG_FUNCTION (this);

  //return DoPeek (begin ());
  //---------------------------


  return m_queues[0]->Peek();
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