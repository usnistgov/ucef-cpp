//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package MapperEx.OmnetFederate;


//
// MapperExNetwork.ned
//

package MapperEx.OmnetFederate;

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
    // delay = 10us;
    // datarate = 0.01Gbps;
    ber = 0;
}

network MapperExNetwork
{
    @display("bgb=708,404");
    submodules:
        configurator: IPv4NetworkConfigurator {
            @display("p=174,29");
        }
        hlaInterface: HLAInterface {
            @display("p=49,29;i=block/dispatch");
        }
        BankOfAmerica: C2WStandardHost {
            @display("p=156,151;i=,yellow");
        }
        SocieteGenerale: C2WStandardHost {
            @display("p=548,49;i=,yellow");
        }
        ICICIBank: C2WStandardHost {
            @display("p=594,151;i=,yellow");
        }
        Moodys: C2WStandardHost {
            @display("p=282,320;i=,red");
        }
        StandardAndPoors: C2WStandardHost {
            @display("p=516,320;i=,red");
        }
        router1: C2WRouter {
            @display("p=327,150;i=old/router2,lavender");
        }
        router2: C2WRouter {
            @display("p=400,100;i=old/router2,lavender");
        }
        router3: C2WRouter {
            @display("p=400,181;i=old/router2,lavender");
        }
        router4: C2WRouter {
            @display("p=460,150;i=old/router2,lavender");
        }
        router5: C2WRouter {
            @display("p=327,247;i=old/router2,lavender");
        }
        router6: C2WRouter {
            @display("p=460,247;i=old/router2,lavender");
        }
    connections:
        BankOfAmerica.ethg++ <--> FO1 <--> router1.ethg++;
        router2.ethg++ <--> FO1 <--> SocieteGenerale.ethg++;
        router2.ethg++ <--> FO1 <--> router1.ethg++;
        router2.ethg++ <--> FO1 <--> router4.ethg++;
        router1.ethg++ <--> FO1 <--> router3.ethg++;
        router3.ethg++ <--> FO1 <--> router4.ethg++;
        router4.ethg++ <--> FO1 <--> ICICIBank.ethg++;
        router3.ethg++ <--> FO1 <--> router5.ethg++;
        router3.ethg++ <--> FO1 <--> router6.ethg++;
        router5.ethg++ <--> FO1 <--> Moodys.ethg++;
        router6.ethg++ <--> FO1 <--> StandardAndPoors.ethg++;
}

