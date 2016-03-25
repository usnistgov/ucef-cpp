//
// DropRouteEntryMsg
//

cplusplus {{
#include "../HLAInterface.h"
}};

class noncobject HLAInterface::RouteEntrySP;

message DropRouteEntryMsg extends cPacket {
    HLAInterface::RouteEntrySP routeEntrySP;
}
