//
// Net2.ned
//
package MultiNet.OmnetFederate2;

import ned.DatarateChannel;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;

import OmnetFederate.BasicUdpApp;
import OmnetFederate.C2WStandardHost;
import OmnetFederate.C2WRouter;
import OmnetFederate.HLAInterface;
import OmnetFederate.BasicUdpAppWrapper;

channel FO1 extends DatarateChannel
{
    delay = 0.1us;
    datarate = 1Gbps;
    ber = 0;
}

network Net2
{
    @display("bgb=708,404");
    submodules:
        configurator: IPv4NetworkConfigurator {
            @display("p=174,29");
        }
        hlaInterface: HLAInterface {
            @display("p=49,29;i=block/dispatch");
        }
        SomeHost: C2WStandardHost {
            @display("p=147,231;i=,yellow");
        }
        F3: C2WStandardHost {
            @display("p=562,68;i=,yellow");
        }
        inGateway: C2WStandardHost {
            @display("p=49,108;i=,red");
        }
        outGateway: C2WStandardHost {
            @display("p=669,249;i=,red");
        }
        router1: C2WRouter {
            @display("p=146,107;i=old/router2,lavender");
        }
        router2: C2WRouter {
            @display("p=367,187;i=old/router2,lavender");
        }
        router3: C2WRouter {
            @display("p=582,248;i=old/router2,lavender");
        }
    connections:
        inGateway.ethg++ <--> FO1 <--> router1.ethg++;
        router1.ethg++ <--> FO1 <--> SomeHost.ethg++;
        router1.ethg++ <--> FO1 <--> router2.ethg++;
        SomeHost.ethg++ <--> FO1 <--> router2.ethg++;
        router2.ethg++ <--> FO1 <--> F3.ethg++;
        router2.ethg++ <--> FO1 <--> router3.ethg++;
        router3.ethg++ <--> FO1 <--> outGateway.ethg++;
}
