// This is the NCR SCOT Message Category (Source) File.
//
// This file was developed from a preliminary version of nmas.mc from
// the NCR Store Minder Development Kit 1.1.  A cross-functional team
// designed nmas.mc to be a template or guide for new development.  If
// a new category is needed, add it to the end of the categories listed.
//
// PLEASE ... update this file based on guidelines in the Device Manager
// Implementation Specification 497-0002234, section 1.5.5.7.5.
//
// Category descriptions should be 20 characters or less in order to fit
// in the field set-up by the NT Event Viewer.
//
//----------------------------------------------------------------------------
// SM_CAT_STARTUP_INIT:
// Events related to the startup or initialization of a component.  Could
// contain Informational success statuses as well as warning or error level
// failure statuses.
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: SM_CAT_STARTUP_INIT
//
// MessageText:
//
//  Startup/Init
//
#define SM_CAT_STARTUP_INIT              0x00000001L

//----------------------------------------------------------------------------
// SM_CAT_STOP_ABORT:
// Events related to the normal or abnormal termination of a component.
// Could contain Informational success or failure statuses.
//
// MessageId: SM_CAT_STOP_ABORT
//
// MessageText:
//
//  Stop/Abort
//
#define SM_CAT_STOP_ABORT                0x00000002L

//----------------------------------------------------------------------------
// SM_CAT_CONFIGURATION:
// Events regarding the configuration of a component.  This might include
// configuration entries within the registry, contents of .INI or .CFG files
// (etc.).  This could also include problems with command line parameters
// for OS drivers.  Events within this category focus on the data content of
// the configuration, not the APIs required to access the data.  Therefore,
// errors from ReadRegistry APIs would be logged as System Call errors.
// However, if the configuration cannot be found, a Configuration log is
// appropriate.
//
// MessageId: SM_CAT_CONFIGURATION
//
// MessageText:
//
//  Configuration
//
#define SM_CAT_CONFIGURATION             0x00000003L

//----------------------------------------------------------------------------
// SM_CAT_INPUT_PARAMETER:
// Events regarding application command line parameters, API parameters, or
// other input sources such as data files.   Examples would include a missing
// required command line parameter, a parameter to an OLE control which is
// invalid, or a corrupt or missing input file.
//
// MessageId: SM_CAT_INPUT_PARAMETER
//
// MessageText:
//
//  Input/Parameter
//
#define SM_CAT_INPUT_PARAMETER           0x00000004L

//----------------------------------------------------------------------------
// SM_CAT_SYSTEM_CALL:
// Events arising from Operating System Calls.  This includes any significant
// events which need to be logged due to the results of OS APIs.  For example,
// a failed call to CreateWindow in the windows environment.
//
// MessageId: SM_CAT_SYSTEM_CALL
//
// MessageText:
//
//  System Call
//
#define SM_CAT_SYSTEM_CALL               0x00000005L

//----------------------------------------------------------------------------
// SM_CAT_3rdPARTY_INTERFACE:
// Events stemming from interfacing with a third party component.  This would
// not include POS peripherals or standard PC devices such as disk, sound, or
// video.  Examples of a Third Party Interface category event would include
// errors returned from a third party software package such as ISAM or a
// Report Generation Control for Visual Basic or Visual C++.
//
// MessageId: SM_CAT_3rdPARTY_INTERFACE
//
// MessageText:
//
//  3rd Party Interface
//
#define SM_CAT_3rdPARTY_INTERFACE        0x00000006L

//----------------------------------------------------------------------------
// SM_CAT_INTERNAL_INTERFACE:
// Events related to the use of internal interfaces.  These events will most
// commonly report problems with the use of interfaces provided by the source.
// An example would include invalid function calls or illegal operations.
//
// MessageId: SM_CAT_INTERNAL_INTERFACE
//
// MessageText:
//
//  Internal Interface
//
#define SM_CAT_INTERNAL_INTERFACE        0x00000007L

//----------------------------------------------------------------------------
// SM_CAT_DATA_CAPTURE:
// Events which capture some data or data structure.  An example of this would
// be dumping a security structure each time a new cashier logs onto the
// system or dumping the hardware registers of a peripheral when an error
// state is encountered.
//
// MessageId: SM_CAT_DATA_CAPTURE
//
// MessageText:
//
//  Data Capture/Debug
//
#define SM_CAT_DATA_CAPTURE              0x00000008L

//----------------------------------------------------------------------------
// SM_CAT_LAN_WAN_COMM:
// Events generated by LAN or WAN Communications.  This does not include link
// level communications from device drivers to peripherals.  Most communi-
// cations are now handled by the OS and will be logged independently of NCR
// platform software, however any ISV or NCR developed solutions should use
// this category.
//
// MessageId: SM_CAT_LAN_WAN_COMM
//
// MessageText:
//
//  LAN/WAN Comm
//
#define SM_CAT_LAN_WAN_COMM              0x00000009L

//----------------------------------------------------------------------------
// SM_CAT_SECURITY:
// Events related to security issues.  This could include successful and
// unsuccessful logon attempts, attempts to access supervisor functions, etc.
//
// MessageId: SM_CAT_SECURITY
//
// MessageText:
//
//  Security
//
#define SM_CAT_SECURITY                  0x0000000AL

//----------------------------------------------------------------------------
// SM_CAT_FILE_IO:
// Events generated from any type of File I/O operations.   This includes
// File Creations, Opens, Reads, Writes, etc.. 
//
// MessageId: SM_CAT_FILE_IO
//
// MessageText:
//
//  File I/O
//
#define SM_CAT_FILE_IO                   0x0000000BL

//----------------------------------------------------------------------------
// SM_CAT_RESOURCE:
// Events resulting from utilizing memory or other allocated or limited
// resources within the system.   The actual events which could be logged
// in this category will vary based on the operating system being used.
// This category basically covers all resources aside from IPC related
// resources.  The following types of resources are examples of what would
// be reported on within this category: Memory, Shared Memory, Memory Mapped
// Files, Threads, Processes, Queues, Lists, etc.
//
// MessageId: SM_CAT_RESOURCE
//
// MessageText:
//
//  Memory/Resources
//
#define SM_CAT_RESOURCE                  0x0000000CL

//----------------------------------------------------------------------------
// SM_CAT_IPC:
// Events resulting from utilizing Interprocess Communication (IPC) resources
// within the system. The actual events which could be logged in this category
// will vary based on the operating system being used. The following types of
// resources are examples of what would be reported on within this category:
// Events, Pipes, Semaphores, Signals, Messages, Critical Sections, etc.
//
// MessageId: SM_CAT_IPC
//
// MessageText:
//
//  IPC
//
#define SM_CAT_IPC                       0x0000000DL

//----------------------------------------------------------------------------
// SM_CAT_PERIPHERAL_COMM:
// Events related to the communication channels of a peripheral.  This cate-
// gory will be primarily used by device drivers or controls unless an appli-
// cation is programming directly to the peripheral.  Examples of events in
// this category would include:  Timeouts, Parity Errors, Transmits, Receives,
// collisions, dropped characters, etc.   This category would be used to
// report RS232, RS-422, RS-485, USB, and proprietary interface problems.
//
// MessageId: SM_CAT_PERIPHERAL_COMM
//
// MessageText:
//
//  Peripheral Comm
//
#define SM_CAT_PERIPHERAL_COMM           0x0000000EL

//----------------------------------------------------------------------------
// SM_CAT_HARDWARE:
// Events generated from components which deal directly with hardware
// devices.   Typically these events will be logged by device drivers or
// peripheral controls.  These logs might report significant hardware
// statuses, state transitions, device failures, etc. 
//
// MessageId: SM_CAT_HARDWARE
//
// MessageText:
//
//  Hardware
//
#define SM_CAT_HARDWARE                  0x0000000FL

//----------------------------------------------------------------------------
// SM_CAT_DEVICE_PERIPHERAL:
// Events generated from accessing devices/peripherals.  This category will
// be used primarily by applications to report the status of peripheral
// operations.   Events resulting from accessing TAPS, NICE, OPOS, or JPOS
// device drivers or controls are good examples of what would be logged in
// this category.
//
// MessageId: SM_CAT_DEVICE_PERIPHERAL
//
// MessageText:
//
//  Device/Peripheral
//
#define SM_CAT_DEVICE_PERIPHERAL         0x00000010L

//----------------------------------------------------------------------------
// SM_CAT_OPERATOR:
// Events related to the operations of a component.  This category covers a
// wide variety of operator induced error events.  Any event which relates to
// the user∆s invalid operational usage of a component.  Examples could
// include, a cashier pressing an invalid key, etc.
//
// MessageId: SM_CAT_OPERATOR
//
// MessageText:
//
//  Operational/User
//
#define SM_CAT_OPERATOR                  0x00000011L

//----------------------------------------------------------------------------
// SM_CAT_INTERNAL_ERROR:
// Events related to failures within the source.  These events will most
// commonly report problems internal to the source.  Unlike the Internal
// interface, where an interface condition was violated in some way, this
// type of event basically indicates that something went wrong.
//
// MessageId: SM_CAT_INTERNAL_ERROR
//
// MessageText:
//
//  Internal Error
//
#define SM_CAT_INTERNAL_ERROR            0x00000012L

