
// This code has been generated by the C2W code generator.
// Do not edit manually!

package TESDemoIntegrationModel;

import java.util.HashSet;
import java.util.Set;

import hla.rti.*;

import c2w.hla.*;

/**
* The StopDNSPoisoningAttack class implements the StopDNSPoisoningAttack interaction in the
* TESDemoIntegrationModel simulation.
*/
public class StopDNSPoisoningAttack extends AttacksNotYetImplemented {

	/**
	* Default constructor -- creates an instance of the StopDNSPoisoningAttack interaction
	* class with default parameter values.
	*/
	public StopDNSPoisoningAttack() { }

	
	
	private static int _affectedHost_handle;
	private static int _dnsHost_handle;
	private static int _listenerHost_handle;
	
	
	/**
	* Returns the handle (RTI assigned) of the "affectedHost" parameter of
	* its containing interaction class.
	*
	* @return the handle (RTI assigned) of the "affectedHost" parameter
	*/
	public static int get_affectedHost_handle() { return _affectedHost_handle; }
	
	/**
	* Returns the handle (RTI assigned) of the "dnsHost" parameter of
	* its containing interaction class.
	*
	* @return the handle (RTI assigned) of the "dnsHost" parameter
	*/
	public static int get_dnsHost_handle() { return _dnsHost_handle; }
	
	/**
	* Returns the handle (RTI assigned) of the "listenerHost" parameter of
	* its containing interaction class.
	*
	* @return the handle (RTI assigned) of the "listenerHost" parameter
	*/
	public static int get_listenerHost_handle() { return _listenerHost_handle; }
	
	
	
	private static boolean _isInitialized = false;

	private static int _handle;

	/**
	* Returns the handle (RTI assigned) of the StopDNSPoisoningAttack interaction class.
	* Note: As this is a static method, it is NOT polymorphic, and so, if called on
	* a reference will return the handle of the class pertaining to the reference,\
	* rather than the handle of the class for the instance referred to by the reference.
	* For the polymorphic version of this method, use {@link #getClassHandle()}.
	*/
	public static int get_handle() { return _handle; }

	/**
	* Returns the fully-qualified (dot-delimited) name of the StopDNSPoisoningAttack
	* interaction class.
	* Note: As this is a static method, it is NOT polymorphic, and so, if called on
	* a reference will return the name of the class pertaining to the reference,\
	* rather than the name of the class for the instance referred to by the reference.
	* For the polymorphic version of this method, use {@link #getClassName()}.
	*/
	public static String get_class_name() { return "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack"; }

	/**
	* Returns the simple name (the last name in the dot-delimited fully-qualified
	* class name) of the StopDNSPoisoningAttack interaction class.
	*/
	public static String get_simple_class_name() { return "StopDNSPoisoningAttack"; }

	private static Set< String > _datamemberNames = new HashSet< String >();
	private static Set< String > _allDatamemberNames = new HashSet< String >();

	/**
	* Returns a set containing the names of all of the non-hidden parameters in the
	* StopDNSPoisoningAttack interaction class.
	* Note: As this is a static method, it is NOT polymorphic, and so, if called on
	* a reference will return a set of parameter names pertaining to the reference,\
	* rather than the parameter names of the class for the instance referred to by
	* the reference.  For the polymorphic version of this method, use
	* {@link #getParameterNames()}.
	*/
	public static Set< String > get_parameter_names() {
		return new HashSet< String >( _datamemberNames );
	}


	/**
	* Returns a set containing the names of all of the parameters in the
	* StopDNSPoisoningAttack interaction class.
	* Note: As this is a static method, it is NOT polymorphic, and so, if called on
	* a reference will return a set of parameter names pertaining to the reference,\
	* rather than the parameter names of the class for the instance referred to by
	* the reference.  For the polymorphic version of this method, use
	* {@link #getParameterNames()}.
	*/
	public static Set< String > get_all_parameter_names() {
		return new HashSet< String >( _allDatamemberNames );
	}


	

	static {
		_classNameSet.add( "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack" );
		_classNameClassMap.put( "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack", StopDNSPoisoningAttack.class );
		
		_datamemberClassNameSetMap.put( "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack", _datamemberNames );
		_allDatamemberClassNameSetMap.put( "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack", _allDatamemberNames );

		
		
		_datamemberNames.add( "affectedHost" );
		
		_datamemberNames.add( "dnsHost" );
		
		_datamemberNames.add( "listenerHost" );
		
		
		
		
		
		_allDatamemberNames.add( "affectedHost" );
		_allDatamemberNames.add( "sourceFed" );
		_allDatamemberNames.add( "dnsHost" );
		_allDatamemberNames.add( "uniqueCommandID" );
		_allDatamemberNames.add( "listenerHost" );
		_allDatamemberNames.add( "originFed" );
		_allDatamemberNames.add( "federateFilter" );
		_allDatamemberNames.add( "actualLogicalGenerationTime" );
		
		
		_datamemberTypeMap.put( "affectedHost", "String" );
		_datamemberTypeMap.put( "dnsHost", "String" );
		_datamemberTypeMap.put( "listenerHost", "String" );
	
	

	}


	private static String initErrorMessage = "Error:  InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack:  could not initialize:  ";
	protected static void init( RTIambassador rti ) {
		if ( _isInitialized ) return;
		_isInitialized = true;
		
		AttacksNotYetImplemented.init( rti );
		
		boolean isNotInitialized = true;
		while( isNotInitialized ) {
			try {
				_handle = rti.getInteractionClassHandle( "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack" );
				isNotInitialized = false;
			} catch ( FederateNotExecutionMember f ) {
				System.err.println( initErrorMessage + "Federate Not Execution Member" );
				f.printStackTrace();
				return;				
			} catch ( NameNotFound n ) {
				System.err.println( initErrorMessage + "Name Not Found" );
				n.printStackTrace();
				return;				
			} catch ( Exception e ) {
				e.printStackTrace();
				try { Thread.sleep( 50 ); } catch( Exception e1 ) { }					
			}
		}

		_classNameHandleMap.put( "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack", get_handle() );
		_classHandleNameMap.put( get_handle(), "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack" );
		_classHandleSimpleNameMap.put( get_handle(), "StopDNSPoisoningAttack" );

		
		isNotInitialized = true;
		while( isNotInitialized ) {
			try {
							
				_affectedHost_handle = rti.getParameterHandle( "affectedHost", get_handle() );			
				_dnsHost_handle = rti.getParameterHandle( "dnsHost", get_handle() );			
				_listenerHost_handle = rti.getParameterHandle( "listenerHost", get_handle() );
				isNotInitialized = false;
			} catch ( FederateNotExecutionMember f ) {
				System.err.println( initErrorMessage + "Federate Not Execution Member" );
				f.printStackTrace();
				return;				
			} catch ( InteractionClassNotDefined i ) {
				System.err.println( initErrorMessage + "Interaction Class Not Defined" );
				i.printStackTrace();
				return;				
			} catch ( NameNotFound n ) {
				System.err.println( initErrorMessage + "Name Not Found" );
				n.printStackTrace();
				return;				
			} catch ( Exception e ) {
				e.printStackTrace();
				try { Thread.sleep( 50 ); } catch( Exception e1 ) { }					
			}
		}
			
			
		_datamemberNameHandleMap.put( "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack,affectedHost", get_affectedHost_handle() );
		_datamemberNameHandleMap.put( "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack,dnsHost", get_dnsHost_handle() );
		_datamemberNameHandleMap.put( "InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack,listenerHost", get_listenerHost_handle() );
			
			
		_datamemberHandleNameMap.put( get_affectedHost_handle(), "affectedHost" );
		_datamemberHandleNameMap.put( get_dnsHost_handle(), "dnsHost" );
		_datamemberHandleNameMap.put( get_listenerHost_handle(), "listenerHost" );
		
	}


	private static boolean _isPublished = false;
	private static String publishErrorMessage = "Error:  InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack:  could not publish:  ";

	/**
	* Publishes the StopDNSPoisoningAttack interaction class for a federate.
	*
	* @param rti handle to the RTI, usu. obtained through the
	* {@link SynchronizedFederate#getRTI()} call
	*/
	public static void publish( RTIambassador rti ) {
		if ( _isPublished ) return;
		
		init( rti );

	

		synchronized( rti ) {
			boolean isNotPublished = true;
			while( isNotPublished ) {
				try {
					rti.publishInteractionClass( get_handle() );
					isNotPublished = false;
				} catch ( FederateNotExecutionMember f ) {
					System.err.println( publishErrorMessage + "Federate Not Execution Member" );
					f.printStackTrace();
					return;
				} catch ( InteractionClassNotDefined i ) {
					System.err.println( publishErrorMessage + "Interaction Class Not Defined" );
					i.printStackTrace();
					return;
				} catch ( Exception e ) {
					e.printStackTrace();
					try { Thread.sleep( 50 ); } catch( Exception e1 ) { }
				}
			}
		}
		
		_isPublished = true;
	}

	private static String unpublishErrorMessage = "Error:  InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack:  could not unpublish:  ";
	/**
	* Unpublishes the StopDNSPoisoningAttack interaction class for a federate.
	*
	* @param rti handle to the RTI, usu. obtained through the
	* {@link SynchronizedFederate#getRTI()} call
	*/
	public static void unpublish( RTIambassador rti ) {
		if ( !_isPublished ) return;
		
		init( rti );
		synchronized( rti ) {
			boolean isNotUnpublished = true;
			while( isNotUnpublished ) {
				try {
					rti.unpublishInteractionClass( get_handle() );
					isNotUnpublished = false;
				} catch ( FederateNotExecutionMember f ) {
					System.err.println( unpublishErrorMessage + "Federate Not Execution Member" );
					f.printStackTrace();
					return;
				} catch ( InteractionClassNotDefined i ) {
					System.err.println( unpublishErrorMessage + "Interaction Class Not Defined" );
					i.printStackTrace();
					return;
				} catch ( InteractionClassNotPublished i ) {
					System.err.println( unpublishErrorMessage + "Interaction Class Not Published" );
					i.printStackTrace();
					return;
				} catch ( Exception e ) {
					e.printStackTrace();
					try { Thread.sleep( 50 ); } catch( Exception e1 ) { }
				}
			}
		}
		
		_isPublished = false;
	}

	private static boolean _isSubscribed = false;
	private static String subscribeErrorMessage = "Error:  InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack:  could not subscribe:  ";
	/**
	* Subscribes a federate to the StopDNSPoisoningAttack interaction class.
	*
	* @param rti handle to the RTI, usu. obtained through the
	* {@link SynchronizedFederate#getRTI()} call
	*/
	public static void subscribe( RTIambassador rti ) {
		if ( _isSubscribed ) return;
		
		init( rti );
	
		
		synchronized( rti ) {
			boolean isNotSubscribed = true;
			while( isNotSubscribed ) {
				try {
					rti.subscribeInteractionClass( get_handle() );
					isNotSubscribed = false;
				} catch ( FederateNotExecutionMember f ) {
					System.err.println( subscribeErrorMessage + "Federate Not Execution Member" );
					f.printStackTrace();
					return;
				} catch ( InteractionClassNotDefined i ) {
					System.err.println( subscribeErrorMessage + "Interaction Class Not Defined" );
					i.printStackTrace();
					return;
				} catch ( Exception e ) {
					e.printStackTrace();
					try { Thread.sleep( 50 ); } catch( Exception e1 ) { }
				}
			}
		}
		
		_isSubscribed = true;
	}

	private static String unsubscribeErrorMessage = "Error:  InteractionRoot.C2WInteractionRoot.ActionBase.OmnetCommand.AttacksNotYetImplemented.StopDNSPoisoningAttack:  could not unsubscribe:  ";
	/**
	* Unsubscribes a federate from the StopDNSPoisoningAttack interaction class.
	*
	* @param rti handle to the RTI, usu. obtained through the
	* {@link SynchronizedFederate#getRTI()} call
	*/
	public static void unsubscribe( RTIambassador rti ) {
		if ( !_isSubscribed ) return;

		init( rti );
		synchronized( rti ) {
			boolean isNotUnsubscribed = true;
			while( isNotUnsubscribed ) {
				try {
					rti.unsubscribeInteractionClass( get_handle() );
					isNotUnsubscribed = false;
				} catch ( FederateNotExecutionMember f ) {
					System.err.println( unsubscribeErrorMessage + "Federate Not Execution Member" );
					f.printStackTrace();
					return;
				} catch ( InteractionClassNotDefined i ) {
					System.err.println( unsubscribeErrorMessage + "Interaction Class Not Defined" );
					i.printStackTrace();
					return;
				} catch ( InteractionClassNotSubscribed i ) {
					System.err.println( unsubscribeErrorMessage + "Interaction Class Not Subscribed" );
					i.printStackTrace();
					return;
				} catch ( Exception e ) {
					e.printStackTrace();
					try { Thread.sleep( 50 ); } catch( Exception e1 ) { }
				}
			}
		}
		
		_isSubscribed = false;
	}

	/**
	* Return true if "handle" is equal to the handle (RTI assigned) of this class
	* (that is, the StopDNSPoisoningAttack interaction class).
	*
	* @param handle handle to compare to the value of the handle (RTI assigned) of
	* this class (the StopDNSPoisoningAttack interaction class).
	* @return "true" if "handle" matches the value of the handle of this class
	* (that is, the StopDNSPoisoningAttack interaction class).
	*/
	public static boolean match( int handle ) { return handle == get_handle(); }

	/**
	* Returns the handle (RTI assigned) of this instance's interaction class .
	* 
	* @return the handle (RTI assigned) if this instance's interaction class
	*/
	public int getClassHandle() { return get_handle(); }

	/**
	* Returns the fully-qualified (dot-delimited) name of this instance's interaction class.
	* 
	* @return the fully-qualified (dot-delimited) name of this instance's interaction class
	*/
	public String getClassName() { return get_class_name(); }

	/**
	* Returns the simple name (last name in its fully-qualified dot-delimited name)
	* of this instance's interaction class.
	* 
	* @return the simple name of this instance's interaction class 
	*/
	public String getSimpleClassName() { return get_simple_class_name(); }

	/**
	* Returns a set containing the names of all of the non-hiddenparameters of an
	* interaction class instance.
	*
	* @return set containing the names of all of the parameters of an
	* interaction class instance
	*/
	public Set< String > getParameterNames() { return get_parameter_names(); }

	/**
	* Returns a set containing the names of all of the parameters of an
	* interaction class instance.
	*
	* @return set containing the names of all of the parameters of an
	* interaction class instance
	*/
	public Set< String > getAllParameterNames() { return get_all_parameter_names(); }

	/**
	* Publishes the interaction class of this instance of the class for a federate.
	*
	* @param rti handle to the RTI, usu. obtained through the
	* {@link SynchronizedFederate#getRTI()} call
	*/
	public void publishInteraction( RTIambassador rti ) { publish( rti ); }

	/**
	* Unpublishes the interaction class of this instance of this class for a federate.
	*
	* @param rti handle to the RTI, usu. obtained through the
	* {@link SynchronizedFederate#getRTI()} call
	*/
	public void unpublishInteraction( RTIambassador rti ) { unpublish( rti ); }

	/**
	* Subscribes a federate to the interaction class of this instance of this class.
	*
	* @param rti handle to the RTI, usu. obtained through the
	* {@link SynchronizedFederate#getRTI()} call
	*/
	public void subscribeInteraction( RTIambassador rti ) { subscribe( rti ); }

	/**
	* Unsubscribes a federate from the interaction class of this instance of this class.
	*
	* @param rti handle to the RTI, usu. obtained through the
	* {@link SynchronizedFederate#getRTI()} call
	*/
	public void unsubscribeInteraction( RTIambassador rti ) { unsubscribe( rti ); }

	

	public String toString() {
		return "StopDNSPoisoningAttack("
			
			
			+ "affectedHost:" + get_affectedHost()
			+ "," + "dnsHost:" + get_dnsHost()
			+ "," + "listenerHost:" + get_listenerHost()
			+ ")";
	}
	



	
	
	private String _affectedHost = "";
	
	private String _dnsHost = "";
	
	private String _listenerHost = "";

	
	
	/**
	* Set the value of the "affectedHost" parameter to "value" for this parameter.
	*
	* @param value the new value for the "affectedHost" parameter
	*/
	public void set_affectedHost( String value ) { _affectedHost = value; }
	
	/**
	* Returns the value of the "affectedHost" parameter of this interaction.
	*
	* @return the value of the "affectedHost" parameter
	*/
	public String get_affectedHost() { return _affectedHost; }
	
	
	/**
	* Set the value of the "dnsHost" parameter to "value" for this parameter.
	*
	* @param value the new value for the "dnsHost" parameter
	*/
	public void set_dnsHost( String value ) { _dnsHost = value; }
	
	/**
	* Returns the value of the "dnsHost" parameter of this interaction.
	*
	* @return the value of the "dnsHost" parameter
	*/
	public String get_dnsHost() { return _dnsHost; }
	
	
	/**
	* Set the value of the "listenerHost" parameter to "value" for this parameter.
	*
	* @param value the new value for the "listenerHost" parameter
	*/
	public void set_listenerHost( String value ) { _listenerHost = value; }
	
	/**
	* Returns the value of the "listenerHost" parameter of this interaction.
	*
	* @return the value of the "listenerHost" parameter
	*/
	public String get_listenerHost() { return _listenerHost; }
	


	protected StopDNSPoisoningAttack( ReceivedInteraction datamemberMap, boolean initFlag ) {
		super( datamemberMap, false );
		if ( initFlag ) setParameters( datamemberMap );
	}
	
	protected StopDNSPoisoningAttack( ReceivedInteraction datamemberMap, LogicalTime logicalTime, boolean initFlag ) {
		super( datamemberMap, logicalTime, false );
		if ( initFlag ) setParameters( datamemberMap );
	}


	/**
	* Creates an instance of the StopDNSPoisoningAttack interaction class, using
	* "datamemberMap" to initialize its parameter values.
	* "datamemberMap" is usually acquired as an argument to an RTI federate
	* callback method, such as "receiveInteraction".
	*
	* @param datamemberMap data structure containing initial values for the
	* parameters of this new StopDNSPoisoningAttack interaction class instance
	*/
	public StopDNSPoisoningAttack( ReceivedInteraction datamemberMap ) {
		this( datamemberMap, true );
	}
	
	/**
	* Like {@link #StopDNSPoisoningAttack( ReceivedInteraction datamemberMap )}, except this
	* new StopDNSPoisoningAttack interaction class instance is given a timestamp of
	* "logicalTime".
	*
	* @param datamemberMap data structure containing initial values for the
	* parameters of this new StopDNSPoisoningAttack interaction class instance
	* @param logicalTime timestamp for this new StopDNSPoisoningAttack interaction class
	* instance
	*/
	public StopDNSPoisoningAttack( ReceivedInteraction datamemberMap, LogicalTime logicalTime ) {
		this( datamemberMap, logicalTime, true );
	}

	/**
	* Creates a new StopDNSPoisoningAttack interaction class instance that is a duplicate
	* of the instance referred to by StopDNSPoisoningAttack_var.
	*
	* @param StopDNSPoisoningAttack_var StopDNSPoisoningAttack interaction class instance of which
	* this newly created StopDNSPoisoningAttack interaction class instance will be a
	* duplicate
	*/
	public StopDNSPoisoningAttack( StopDNSPoisoningAttack StopDNSPoisoningAttack_var ) {
		super( StopDNSPoisoningAttack_var );
		
		
		set_affectedHost( StopDNSPoisoningAttack_var.get_affectedHost() );
		set_dnsHost( StopDNSPoisoningAttack_var.get_dnsHost() );
		set_listenerHost( StopDNSPoisoningAttack_var.get_listenerHost() );
	}


	/**
	* Returns the value of the parameter whose name is "datamemberName"
	* for this interaction.
	*
	* @param datamemberName name of parameter whose value is to be
	* returned
	* @return value of the parameter whose name is "datamemberName"
	* for this interaction
	*/
	public Object getParameter( String datamemberName ) {
		
		
		
		if (  "affectedHost".equals( datamemberName )  ) return get_affectedHost();
		else if (  "dnsHost".equals( datamemberName )  ) return get_dnsHost();
		else if (  "listenerHost".equals( datamemberName )  ) return get_listenerHost();
		else return super.getParameter( datamemberName );
	}
	
	/**
	* Returns the value of the parameter whose handle (RTI assigned)
	* is "datamemberHandle" for this interaction.
	*
	* @param datamemberHandle handle (RTI assigned) of parameter whose
	* value is to be returned
	* @return value of the parameter whose handle (RTI assigned) is
	* "datamemberHandle" for this interaction
	*/
	public Object getParameter( int datamemberHandle ) {
		
				
		
		if ( get_affectedHost_handle() == datamemberHandle ) return get_affectedHost();
		else if ( get_dnsHost_handle() == datamemberHandle ) return get_dnsHost();
		else if ( get_listenerHost_handle() == datamemberHandle ) return get_listenerHost();
		else return super.getParameter( datamemberHandle );
	}
	
	protected boolean setParameterAux( int param_handle, String val ) {
		boolean retval = true;		
		
			
		
		if ( param_handle == get_affectedHost_handle() ) set_affectedHost( val );
		else if ( param_handle == get_dnsHost_handle() ) set_dnsHost( val );
		else if ( param_handle == get_listenerHost_handle() ) set_listenerHost( val );
		else retval = super.setParameterAux( param_handle, val );
		
		return retval;
	}
	
	protected boolean setParameterAux( String datamemberName, String val ) {
		boolean retval = true;
		
			
		
		if (  "affectedHost".equals( datamemberName )  ) set_affectedHost( val );
		else if (  "dnsHost".equals( datamemberName )  ) set_dnsHost( val );
		else if (  "listenerHost".equals( datamemberName )  ) set_listenerHost( val );	
		else retval = super.setParameterAux( datamemberName, val );
		
		return retval;
	}
	
	protected boolean setParameterAux( String datamemberName, Object val ) {
		boolean retval = true;
		
		
		
		if (  "affectedHost".equals( datamemberName )  ) set_affectedHost( (String)val );
		else if (  "dnsHost".equals( datamemberName )  ) set_dnsHost( (String)val );
		else if (  "listenerHost".equals( datamemberName )  ) set_listenerHost( (String)val );		
		else retval = super.setParameterAux( datamemberName, val );
		
		return retval;
	}

	protected SuppliedParameters createSuppliedDatamembers() {
		SuppliedParameters datamembers = super.createSuppliedDatamembers();

	
		
		
			datamembers.add( get_affectedHost_handle(), get_affectedHost().getBytes() );
		
			datamembers.add( get_dnsHost_handle(), get_dnsHost().getBytes() );
		
			datamembers.add( get_listenerHost_handle(), get_listenerHost().getBytes() );
		
	
		return datamembers;
	}

	
	public void copyFrom( Object object ) {
		super.copyFrom( object );
		if ( object instanceof StopDNSPoisoningAttack ) {
			StopDNSPoisoningAttack data = (StopDNSPoisoningAttack)object;
			
			
				_affectedHost = data._affectedHost;
				_dnsHost = data._dnsHost;
				_listenerHost = data._listenerHost;
			
		}
	}
}
