# Virtual LAN HAL Documentation

## Acronyms

- `HAL` \- Hardware Abstraction Layer
- `RDK-B` \- Reference Design Kit for Broadband Devices
- `OEM` \- Original Equipment Manufacture
- `VLAN` \- Virtual LAN

## Description
The diagram below describes a high-level software architecture of the VLAN (Virtual LAN) HAL module stack.

![VLAN HAL Architecture Diagram](images/VLAN_HAL_Architecture.png)

VLAN HAL is an abstraction layer, implemented to interact with the underlying software through a standard set of APIs to add / delete / print VLAN group / interface.

## Component Runtime Execution Requirements

### Initialization and Startup

There is no dependent API's is expected to be intialized for invoking VLAN HAL client module.

3rd party vendors will implement appropriately to meet operational requirements. This interface is expected to block if the hardware is not ready.

## Threading Model

The interface is not thread safe.

Any module which is invoking the VLAN HAL api should ensure calls are made in a thread safe manner.

Vendors can create internal threads/events to meet their operation requirements. These should be responsible to synchronize between the calls, events and cleaned up on closure.

## Process Model

All API's are expected to be called from multiple process.

## Memory Model

The client module is responsible to allocate and deallocate memory for necessary API's as specified in API Documentation.

Different 3rd party vendors allowed to allocate memory for internal operational requirements. In this case 3rd party implementations
should be responsible to deallocate internally.

TODO:
State a footprint requirement. Example: This should not exceed XXXX KB.

## Power Management Requirements

The HAL is not involved in any of the power management operation.
Any power management state transitions WILL AFFECT affect the operation of the VLAN HAL.

## Asynchronous Notification Model

There are no asynchronous notifications.

## Blocking calls

The API's are expected to work synchronously and should complete within a time period commensurate with the complexity of the operation and in accordance with any relevant specification.
Any calls that can fail due to the lack of a response should have a timeout period in accordance with any relevant documentation.
The upper layers will call this API from a single thread context, this API should not suspend.

TODO:
As we state that they should complete within a time period, we need to state what that time target is, and pull it from the spec if required. Define the timeout requirement.

## Internal Error Handling

All the VLAN HAL API's should return error synchronously as a return argument. HAL is responsible to handle system errors(e.g. out of memory) internally.

## Persistence Model

There is no requirement for HAL to persist any setting information. The caller is responsible to persist any settings related to their implementation.


## Non functional requirements

Following non functional requirement should be supported by the component.

## Logging and debugging requirements

The component should log all the error and critical informative messages, preferably using printf, syslog which helps to debug/triage the issues and understand the functional flow of the system.

The logging should be consistent across all HAL components.

If the vendor is going to log then it has to be logged in `xxx_vendor_hal.log` file name which can be placed in `/rdklogs/logs/` or `/var/tmp/` directory.

Logging should be defined with log levels as per Linux standard logging.

## Memory and performance requirements

The component should not be contributing more to memory and CPU utilization while performing normal operations and Commensurate with the operation required.


## Quality Control

VLAN HAL implementation should pass checks using any third party tools like `Coverity`, `Black duck`, `Valgrind` etc. without any issue to ensure quality.

There should not be any memory leaks/corruption introduced by HAL and underneath 3rd party software implementation.


## Licensing

VLAN HAL implementation is spected to released under the Apache License 2.0.

## Build Requirements

The source code should be able to be built under Linux Yocto environment and should be delivered as a shared library `libhal_vlan.so`.

## Variability Management

Changes to the interface will be controlled by versioning, vendors will be expected to implement to a fixed version of the interface, and based on SLA agreements move to later versions as demand requires.

Each API interface will be versioned using [Semantic Versioning 2.0.0](https://semver.org/), the vendor code will comply with a specific version of the interface.

## Platform or Product Customization

None

## Interface API Documentation

All HAL function prototypes and datatype definitions are available in `vlan_hal.h` file.

1. Components/Process must include `vlan_hal.h` to make use of VLAN HAL capabilities.
2. Components/Process should add linker dependency for `libhal_vlan`.

## Theory of operation and key concepts

Covered as per "Description" sections in the API documentation.

## Sequence Diagram

```mermaid
sequenceDiagram
participant Caller
participant VLAN HAL
participant Vendor
Note over Caller: Init once during bootup, Needed for Dependent API's. <br> Ignore this if caller doesn't have any Dependent API's
Caller->>VLAN HAL: vlan_hal_addxxxx()
VLAN HAL->>Vendor: 
Vendor ->>VLAN HAL: 
VLAN HAL->>Caller: vlan_hal_addxxxx() return

Caller->>VLAN HAL: vlan_hal_delxxxx()
VLAN HAL->>Vendor: 
Vendor ->>VLAN HAL: 
VLAN HAL->>Caller: vlan_hal_delxxxx() return

Caller->>VLAN HAL: vlan_hal_printGroup()
VLAN HAL->>Vendor: 
Vendor ->>VLAN HAL: 
VLAN HAL->>Caller: vlan_hal_printGroup() return

Caller->>VLAN HAL: vlan_hal_printAllGroup()
VLAN HAL->>Vendor: 
Vendor ->>VLAN HAL: 
VLAN HAL->>Caller: vlan_hal_printAllGroup() return

Caller->>VLAN HAL: vlan_hal_delete_all_Interfaces()
VLAN HAL->>Vendor: 
Vendor ->>VLAN HAL: 
VLAN HAL->>Caller: vlan_hal_delete_all_Interfaces() return

Caller->>VLAN HAL: insert_VLAN_ConfigEntry()
VLAN HAL->>Vendor: 
Vendor ->>VLAN HAL: 
VLAN HAL->>Caller: insert_VLAN_ConfigEntry() return

Caller->>VLAN HAL: delete_VLAN_ConfigEntry()
VLAN HAL->>Vendor: 
Vendor ->>VLAN HAL: 
VLAN HAL->>Caller: delete_VLAN_ConfigEntry() return

Caller->>VLAN HAL: get_vlanId_for_GroupName()
VLAN HAL->>Vendor: 
Vendor ->>VLAN HAL: 
VLAN HAL->>Caller: get_vlanId_for_GroupName() return

Caller->>VLAN HAL: print_all_vlanId_Configuration()
VLAN HAL->>Vendor: 
Vendor ->>VLAN HAL: 
VLAN HAL->>Caller: print_all_vlanId_Configuration() return
```
