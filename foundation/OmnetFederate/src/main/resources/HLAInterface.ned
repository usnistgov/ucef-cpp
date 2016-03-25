//
// HLAInterface.ned
//

package OmnetFederate;

simple HLAInterface {

	parameters:
		string federation_name;
		string federate_name = default( "OmnetFederate" );
		double federate_step_size = default( 0.1 );
		double federate_lookahead = default( 0.01 );

	gates:
		input	hlaOut @directIn;
}
