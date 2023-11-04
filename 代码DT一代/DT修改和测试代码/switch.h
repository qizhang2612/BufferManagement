#ifndef SWITCH_H
#define SWITCH_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "iostream"

namespace ns3 {
    class Switch : public Object{
        public:
            static TypeId GetTypeId (void);
            Switch();
            virtual ~Switch();
            int GetThreshold();
            void Set(double alpha,int maxbuffer,int usedbuffer);
            void DeleteUsed(int size);
            void AddUsed(int size);
            void Calculate();
        private:
            double m_threshold;
            double m_alpha;
            int m_maxbuffer;
            int m_usedbuffer;
    };

}
#endif