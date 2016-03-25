//
// ObjectMsg.msg
//

cplusplus {{
#include <ObjectRoot.hpp>
}};

class noncobject ObjectRootSP;

message ObjectMsg extends cPacket {
    ObjectRootSP objectRootSP;
}
