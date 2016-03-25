/*
 * HLAScheduler.hpp
 *
 */

#ifndef HLAScheduler_hpp
#define HLAScheduler_hpp

#include <omnetpp.h>

/**
 * Extends the basic OMNeT scheduler to step forward in
 * time based on the RTI's permission
 */
class HLAScheduler: public cScheduler
{
public:
    HLAScheduler();
    virtual ~HLAScheduler() {}

    virtual void startRun();
    virtual void endRun();
    virtual void executionResumed();

    virtual cMessage *getNextEvent();
};

Register_Class(HLAScheduler);

#endif /* HLAScheduler_hpp */
