//
// AddRouteEntryMsg.msg
//

cplusplus {{
#include "../HLAInterface.h"
}};

class noncobject HLAInterface::RouteEntrySP;

message AddRouteEntryMsg extends cPacket {
    HLAInterface::RouteEntrySP routeEntrySP;
}
