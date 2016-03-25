//
// InteractionMsg.msg
//

cplusplus {{
#include <InteractionRoot.hpp>
}};

class noncobject InteractionRootSP;

message InteractionMsg extends cPacket {
	bool toHLA;
	int messageNo;
    InteractionRootSP interactionRootSP;
}
