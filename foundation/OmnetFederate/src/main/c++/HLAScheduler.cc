/*
 * HLAScheduler.cpp
 *
 */

#include <HLAScheduler.h>
#include <HLAInterface.h>

Register_Class(HLAScheduler);

HLAScheduler::HLAScheduler() : cScheduler() {}

void HLAScheduler::startRun() { }
void HLAScheduler::endRun() { }
void HLAScheduler::executionResumed() { }

// getNextEvent - the main method of the scheduler
cMessage* HLAScheduler::getNextEvent()
{
    // first try to see if an event is scheduled
    cMessage* msg = sim->msgQueue.peekFirst();
    if (!msg)
    {
        throw new cTerminationException(eENDEDOK);
    }

    HLAInterface *hlaInterfacePtr = HLAInterface::get_InstancePtr();

    // if no event is scheduled before the logical time,
    // request that the RTI advance the time
    double timeDifference = msg->getArrivalTime().dbl() - hlaInterfacePtr->getCurrentTimeLocal();
	while( timeDifference > 0 ) {

		// move time ahead one step
		double step = std::min( timeDifference, hlaInterfacePtr->getStepSize() );
		hlaInterfacePtr->advanceTimeStep( step );

		msg = sim->msgQueue.peekFirst();
	    timeDifference = msg->getArrivalTime().dbl() - hlaInterfacePtr->getCurrentTimeLocal();
	}

    // let OMNET++ simulate the message transmission
    return msg;
}
