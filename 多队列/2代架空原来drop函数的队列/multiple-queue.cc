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



template <typename Item>
bool
MultipleQueue<Item>::Enqueue (Ptr<Item> item)
{
  NS_LOG_FUNCTION (this << item);
  std::cout<<"this is enqueue func."<<std::endl;
  bool flag = m_queues[0]->Enqueue(item);
  if(flag==true){
    std::cout<<"subqueue 0 enqueue successful"<<std::endl;
  }else{
    std::cout<<"subqueue 0 enqueue fail"<<std::endl;
  }
  

  //return DoEnqueue (end (), item);
  return flag;
}

template <typename Item>
Ptr<Item>
MultipleQueue<Item>::Dequeue (void)
{
  NS_LOG_FUNCTION (this);
  std::cout<<"this is dequeue func."<<std::endl;
  Ptr<Item> lzyitem = m_queues[0]->Dequeue();
  if(lzyitem!=0){
    std::cout<<"subqueue 0 dequeue successful"<<std::endl;
  }else{
    std::cout<<"subqueue 0 dequeue fail"<<std::endl;
  }

  //Ptr<Item> item = DoDequeue (begin ());

  //NS_LOG_LOGIC ("Popped " << item);

  //return item;
  return lzyitem;
}

template <typename Item>
Ptr<Item>
MultipleQueue<Item>::Remove (void)
{
  NS_LOG_FUNCTION (this);

  //Ptr<Item> item = DoRemove (begin ());
  Ptr<Item> lzyitem = m_queues[0]->Remove();
  //NS_LOG_LOGIC ("Removed " << item);

  //return item;
  return lzyitem;
}

template <typename Item>
Ptr<const Item>
MultipleQueue<Item>::Peek (void) const
{
  NS_LOG_FUNCTION (this);

  //return DoPeek (begin ());
  return m_queues[0]->Peek();
}






// only use the subqueue 0 for test
// template <typename Item>
// bool
// MultipleQueue<Item>::Enqueue (Ptr<Item> item)
// {
//   if(m_showflag==true){
//       std::cout<<"enqueue function, this is multiple queue."<<std::endl;
//   }
//   NS_LOG_FUNCTION (this << item);

//   bool flag = Enqueue(item,0);
//   return flag;

// //   return DoEnqueue (end (), item);
// }

// template <typename Item>
// bool
// MultipleQueue<Item>::Enqueue (Ptr<Item> item,uint32_t index)
// {
//   if(m_showflag==true){
//       std::cout<<"enqueue function, this is multiple queue."<<std::endl;
//   }
//   if(index<0||index>=qCnt){
//       std::cout<<"<<the index of subqueue is wrong"<<std::endl;
//       return 0;
//   }
//   //bool flag = false; 
//   //flag = m_queues[index]->Enqueue(item);
//   if(m_queues[index]->Enqueue(item)){
//       std::cout<<"enqueue successful"<<std::endl;
//       uint32_t size = item->GetSize ();
//       m_nBytes += size;
//       m_nTotalReceivedBytes += size;

//       m_nPackets++;
//       m_nTotalReceivedPackets++;
//       return true;
//   }else{
//       std::cout<<"enqueue fails in subqueue "<<index<<std::endl;
//       return false;
//   }

//   NS_LOG_FUNCTION (this << item);
//   return true;

// //   return DoEnqueue (end (), item);
// }
// //only use the subqueue 0 for test
// template <typename Item>
// Ptr<Item>
// MultipleQueue<Item>::Dequeue (void)
// {
//   if(m_showflag==true){
//       std::cout<<"dequeue function, this is multiple queue."<<std::endl;
//   }
//   NS_LOG_FUNCTION (this);

//   Ptr<Item> item = Dequeue(0);

//   NS_LOG_LOGIC ("Popped " << item);

//   return item;
// }

// template <typename Item>
// Ptr<Item>
// MultipleQueue<Item>::Dequeue (uint32_t index)
// {
//   if(m_showflag==true){
//       std::cout<<"dequeue function, this is multiple queue."<<std::endl;
//   }
//   NS_LOG_FUNCTION (this);

//   Ptr<Item> item = m_queues[index]->Dequeue();
//   m_nBytes -= item->GetSize ();
//   m_nPackets--;

//   NS_LOG_LOGIC ("Popped " << item);

//   return item;
// }

// template <typename Item>
// Ptr<Item>
// MultipleQueue<Item>::Remove (void)
// {
//   if(m_showflag==true){
//       std::cout<<"remove function, this is multiple queue."<<std::endl;
//   }
//   NS_LOG_FUNCTION (this);

//   Ptr<Item> item = m_queues[0]->Remove();

//   NS_LOG_LOGIC ("Removed " << item);

//   return item;
// }

// template <typename Item>
// Ptr<const Item>
// MultipleQueue<Item>::Peek (void) const
// {
//   if(m_showflag==true){
//       std::cout<<"peek function, this is multiple queue."<<std::endl;
//   }
//   m_queues[0]->Peek();

//   NS_LOG_FUNCTION (this);

//   return 0;
// }

// template <typename Item>
// bool
// MultipleQueue<Item>::GetShowFunctionFlag()
// {
//   return m_showflag;
// }

// template <typename Item>
// void
// MultipleQueue<Item>::SetFuctionFlag(bool showflag)
// {
//   m_showflag = showflag;
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