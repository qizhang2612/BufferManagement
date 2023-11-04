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
  
  virtual bool Enqueue (Ptr<Item> item);
  virtual Ptr<Item> Dequeue (void);
  virtual Ptr<Item> Remove (void);
  virtual Ptr<const Item> Peek (void) const;
  // virtual bool Enqueue (Ptr<Item> item);
  // virtual Ptr<Item> Dequeue (void);
  // virtual Ptr<Item> Remove (void);
  // virtual Ptr<const Item> Peek (void) const;

  // bool Enqueue(Ptr<Item> item, uint32_t index);
  // Ptr<Item> Dequeue (uint32_t index);

  // bool GetShowFunctionFlag();
  // void SetFuctionFlag(bool showflag);

  //new message;
  static const unsigned qCnt = 8;  //the number of a subqueue in the multiple queue
  std::vector<Ptr<Queue<Item> > > m_queues; // uc queues

  // uint32_t m_nBytes = 0;
  // uint32_t m_nTotalReceivedBytes = 0;
  // uint32_t m_nPackets = 0;
  // uint32_t m_nTotalReceivedPackets = 0;

private:
  bool m_showflag = false;
  
  using Queue<Item>::begin;
  using Queue<Item>::end;
  using Queue<Item>::DoEnqueue;
  using Queue<Item>::DoDequeue;
  using Queue<Item>::DoRemove;
  using Queue<Item>::DoPeek;

  NS_LOG_TEMPLATE_DECLARE;     //!< redefinition of the log component
};


} // namespace ns3

#endif /* DROPTAIL_H */