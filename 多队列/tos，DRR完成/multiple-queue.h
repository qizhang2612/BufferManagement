/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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

#ifndef MULTIPLE_H
#define MULTIPLE_H

#include "ns3/queue.h"


namespace ns3 {

/**
 * \ingroup queue
 *
 * \brief A FIFO packet queue that drops tail-end packets on overflow
 */
template <typename Item>
class MultipleQueue : public Queue<Item>
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief DropTailQueue Constructor
   *
   * Creates a droptail queue with a maximum size of 100 packets by default
   */
  MultipleQueue ();

  virtual ~MultipleQueue ();

  virtual bool Enqueue (Ptr<Item> item);      //RR 
  virtual Ptr<Item> Dequeue (void);           //RR
  virtual Ptr<Item> Remove (void);
  virtual Ptr<const Item> Peek (void) const;

  virtual bool Enqueue (Ptr<Item> item, uint32_t index);
  virtual Ptr<Item> Dequeue (uint32_t index);

  virtual Ptr<Item> Remove (uint32_t index);
  virtual Ptr<const Item> Peek (uint32_t) const;


  virtual Ptr<Item> DequeuePQ (void);
  virtual Ptr<Item> DequeueDRR (void); 
  virtual Ptr<Item> DequeueDRR2 (void);

  //tos enqueue
  uint32_t m_tos = 100;  
  virtual bool EnqueueTos (Ptr<Item> item);  //TOS
  uint32_t m_enqueueType;          //choose different enqueue type


  //new message;
  static const unsigned qCnt = 8;  //the number of a subqueue in the multiple queue
  uint32_t m_useqCnt;              //use for lzy test, only use m_useqCnt subqueues
  uint32_t m_dequeueType;          //choose different dequeue type

  //the message for DRR

  uint32_t w0,w1,w2;

//must use int ,uint don't have nagivate number
  int m_weight[qCnt] = {1100,20,150,0,0,0,0,0};
  int m_tempweight[qCnt] = {0,0,0,0,0,0,0,0};
  void SetWeight(int weight[]);
  void GetWeight(int weight[]);

  



  // bool IsZero(int weight []);    //
  // virtual Ptr<Item> DequeueWRR1 (void);  
  // virtual Ptr<Item> DequeueWRR (void);     //WRR use WRR1

  std::vector<Ptr<Queue<Item> > > m_queues; // uc queues
  double threshold = 100000000;
  void SetThreshold(double th);
  double GetThreshold();

  uint32_t m_enindex = -1;      //the last enqueue index
  uint32_t m_deindex = -1;      //the last dequeue index

  void SetEnIndex(uint32_t enindex);
  uint32_t GetEnIndex();

  void SetDeIndex(uint32_t deindex);
  uint32_t GetDeIndex();

  void SetUseqCnt(uint32_t useqCnt);
  uint32_t GetUseqCnt();  

  void SetDequeueType(uint32_t dequeueType);
  uint32_t GetDequeueType(); 

  void SetTos(uint32_t tos);
  uint32_t GetTos();

  void SetEnqueueType(uint32_t enqueueType);
  uint32_t GetEnqueueType();  

  // uint32_t m_nBytes = 0;
  // uint32_t m_nTotalReceivedBytes = 0;
  // uint32_t m_nPackets = 0;
  // uint32_t m_nTotalReceivedPackets = 0;

private:
  bool m_showflag = false;
  
  // using Queue<Item>::begin;
  // using Queue<Item>::end;
  // using Queue<Item>::DoEnqueue;
  // using Queue<Item>::DoDequeue;
  // using Queue<Item>::DoRemove;
  // using Queue<Item>::DoPeek;

  NS_LOG_TEMPLATE_DECLARE;     //!< redefinition of the log component
};


} // namespace ns3

#endif /* DROPTAIL_H */