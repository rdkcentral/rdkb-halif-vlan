/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**
* @file vlan_hal.h
* @brief vlan_hal provides the function prototypes used for VLAN abstraction layer.
*/


#ifndef __VLAN_HAL_H__
#define __VLAN_HAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef BOOL
#define BOOL  unsigned char
#endif

#ifndef CHAR
#define CHAR  char
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

#ifndef INT
#define INT   int
#endif

#ifndef UINT
#define UINT  unsigned int
#endif

#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif

#ifndef ENABLE
#define ENABLE   1
#endif

#ifndef RETURN_OK
#define RETURN_OK   0
#endif

#ifndef RETURN_ERR
#define RETURN_ERR   -1
#endif

/**
 * @defgroup VLAN_HAL Virtual LAN HAL
 *
 * @brief This module provides a Hardware Abstraction Layer (HAL) for managing
 * Virtual Local Area Networks (VLANs) on the system. It offers a set of APIs to
 * create, delete, and modify VLAN groups, add and remove interfaces from VLANs,
 * and query the VLAN configuration. This HAL interfaces with the underlying
 * system's VLAN implementation, abstracting the details and providing a consistent
 * interface for VLAN management across different platforms. It is designed to be
 * used by higher-level networking components and applications to easily configure
 * and control VLANs on the device.
 *
 */

/**
 * @addtogroup VLAN_HAL_TYPES
 * @{
 */

//defines for charecter length for VLAN configurartion
#define VLAN_HAL_MAX_VLANGROUP_TEXT_LENGTH             32
#define VLAN_HAL_MAX_VLANID_TEXT_LENGTH                32
#define VLAN_HAL_MAX_INTERFACE_NAME_TEXT_LENGTH        32

#define VLAN_HAL_MAX_LINE_BUFFER_LENGTH                120

/**********************************************************************
                ENUMERATION DEFINITIONS
**********************************************************************/

/**********************************************************************
                STRUCTURE DEFINITIONS
**********************************************************************/

/**
 * @brief Represents VLANID maintainance.
 *
 * Holds information about the VLAN id configuration.
 * Provides the bridge name, vlan id and acts as a linked list.
 *
 * @note Ensure that the structure members does not exceed the buffer size
 * limit defined by the `VLAN_HAL_MAX_VLANGROUP_TEXT_LENGTH` macro.
 */

// Structure for VLANID Maintainance
/**
 * @brief VLAN Group Configuration Structure
 *
 * This structure stores the configuration for a VLAN group. 
 * It includes:
 *   - The name of the VLAN group (which is the bridge name).
 *   - The VLAN ID associated with the group.
 *
 * The structure is designed to be part of a linked list, allowing for the
 * management of multiple VLAN groups.
 *
 * @note Ensure the total size of this structure (including padding) does not
 *       exceed the `VLAN_HAL_MAX_VLANGROUP_TEXT_LENGTH` limit.
 */
typedef struct _vlan_vlanidconfiguration {
    char groupName[VLAN_HAL_MAX_VLANGROUP_TEXT_LENGTH]; // Bridge name for the VLAN group.
    char vlanID[VLAN_HAL_MAX_VLANID_TEXT_LENGTH];      // ID assigned to interfaces within the group.
    struct _vlan_vlanidconfiguration *nextlink;        // Pointer to the next configuration in the linked list.
} vlan_vlanidconfiguration_t;

/** @} */  //END OF GROUP VLAN_HAL_TYPES

/**********************************************************************
 *
 *  VLAN HAL function prototypes
 *
***********************************************************************/

/**
 * @todo Refactor Return Codes and Header File Organization
 *
 * This module uses `#define` macros for return codes. To improve error handling and 
 * provide better feedback, refactor as follows:
 *
 * 1. Create an `enum` for success and error conditions.
 * 2. Add specific error values to the enum (e.g., `VLAN_HAL_GROUP_EXISTS`, 
 *    `VLAN_HAL_INVALID_VLAN_ID`, `VLAN_HAL_INTERFACE_NOT_FOUND`, etc.).
 * 3. Update function return types to use the new enum.
 * 4. Ensure consistent error code usage throughout the VLAN HAL module.
 *
 * Additionally, split the header files to improve modularity and encapsulation:
 *
 * 5. `vlan_hal.h`: Contains the public interface (external functions).
 * 6. `vlan_hal_internal.h`: Contains private helper functions and internal 
 *                           data structures.
 * 7. `vlan_utils.h`: Contains utility functions that might be useful in other
 *                    parts of the codebase (if applicable).
 *
 * This refactoring improves usability, maintainability, and clarity of the 
 * VLAN HAL module, as suggested in the open-source review.
 */



/**
 * @addtogroup VLAN_HAL_APIS
 * @{
 */

/**
 * @brief Creates a new VLAN group and assigns the default VLAN ID.
 *
 * This function adds a new VLAN group to the system using the provided
 * bridge name (`groupName`) and sets its default VLAN ID (`default_vlanID`).
 * If a group with the same name already exists and has the same `default_vlanID`,
 * it is considered a success.
 *
 * @param[in] groupName - The name of the bridge for the VLAN group (e.g., "brlan0").
 *                        Valid values are: brlan0, brlan1, brlan2, brlan3, brlan4, brlan5,
 *                        brlan7, brlan10, brlan106, brlan403, brlan112, brlan113, brebhaul.
 * @param[in] default_vlanID - The VLAN ID assigned to the new VLAN group (1-4094).
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The operation succeeded, or the group already existed with the
 *                     expected VLAN ID.
 * @retval RETURN_ERR - An error occurred during group creation.
 *
 * @note Possible errors:
 *       - VLAN ID is outside the valid range (1-4094).
 *       - A group with the same name exists, but with a different VLAN ID.
 *       - System-level errors during bridge creation or VLAN configuration.
 *
 * @todo Refactor return codes to use a more specific and informative enum (see general TODO).
 */
int vlan_hal_addGroup(const char *groupName, const char *default_vlanID);

/**
 * @brief Deletes an existing VLAN group and its associated interface memberships.
 *
 * This function removes a VLAN group identified by its bridge name (`groupName`). 
 * If the group does not exist, the function considers this a success and returns 
 * `RETURN_OK`. Any interfaces associated with the deleted group are also removed 
 * from the VLAN configuration.
 *
 * @param[in] groupName - The name of the bridge associated with the VLAN group to 
 *                        be deleted (e.g., "brlan0").
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The operation was successful, or the group did not exist.
 * @retval RETURN_ERR - An error occurred during the deletion process.
 *
 * @note Possible error scenarios include:
 *       - System-level errors during bridge deletion or interface removal.
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int vlan_hal_delGroup(const char *groupName);

/**
 * @brief Adds an interface to an existing VLAN group and assigns a VLAN ID.
 *
 * This function adds a network interface (`ifName`) to a specified VLAN group 
 * (`groupName`). If the interface is already a member of the group with the
 * expected VLAN ID (`vlanID`), it is considered a success.
 *
 * @param[in] groupName - The name of the VLAN group (bridge name) to which the 
 *                        interface will be added (e.g., "brlan0").
 *                        Valid values are: brlan0, brlan1, brlan2, brlan3, brlan4, brlan5,
 *                        brlan7, brlan10, brlan106, brlan403, brlan112, brlan113, brebhaul.
 * @param[in] ifName - The name of the network interface to be added (e.g., "eth0").
 *                     This is vendor-specific.
 * @param[in] vlanID - The VLAN ID (1-4094) to assign to the interface within the
 *                     group.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The operation succeeded, or the interface was already a
 *                     member of the group with the expected VLAN ID.
 * @retval RETURN_ERR - An error occurred.
 *
 * @note Possible errors:
 *       - The VLAN group does not exist.
 *       - The VLAN ID is outside the valid range (1-4094).
 *       - The interface is already in the group with a different VLAN ID.
 *       - System-level errors during interface addition or VLAN configuration.
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int vlan_hal_addInterface(const char *groupName, const char *ifName, const char *vlanID);

/**
 * @brief Removes an interface from a VLAN group.
 *
 * This function deassociates a network interface (`ifName`) from the specified 
 * VLAN group (`groupName`). If the interface is not a member of the group, it 
 * is considered a success.
 *
 * @param[in] groupName - The name of the VLAN group (bridge name) from which the
 *                        interface will be removed (e.g., "brlan0").
 *                        Valid values are: brlan0, brlan1, brlan2, brlan3, brlan4, brlan5,
 *                        brlan7, brlan10, brlan106, brlan403, brlan112, brlan113, brebhaul.
 * @param[in] ifName - The name of the network interface to be removed (e.g., "eth0"). 
 *                     This is vendor-specific.
 * @param[in] vlanID - The VLAN ID (1-4094) associated with the interface within the
 *                     group.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The operation succeeded, or the interface was not a
 *                     member of the group.
 * @retval RETURN_ERR - An error occurred during the removal process.
 *
 * @note Possible errors:
 *       - The VLAN group does not exist.
 *       - The interface is not a member of the VLAN group.
 *       - System-level errors during interface removal or VLAN configuration.
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int vlan_hal_delInterface(const char *groupName, const char *ifName, const char *vlanID);

/**
 * @brief Outputs the settings of a specific VLAN group for debugging.
 *
 * This function retrieves and displays the configuration details of a 
 * VLAN group identified by its bridge name (`groupName`). The output is intended
 * for diagnostic purposes and may be printed to a console or log.
 *
 * @param[in] groupName - The name of the bridge representing the VLAN group to 
 *                        dump (e.g., "brlan0"). Valid values are: brlan0, brlan1, 
 *                        brlan2, brlan3, brlan4, brlan5, brlan7, brlan10, 
 *                        brlan106, brlan403, brlan112, brlan113, brebhaul.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The group settings were successfully dumped.
 * @retval RETURN_ERR - An error occurred (e.g., the group was not found, or 
 *                     there was a system error).
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int vlan_hal_printGroup(const char *groupName);


/**
 * @brief Outputs the settings of all VLAN groups for debugging.
 *
 * This function retrieves and displays the configuration details of all VLAN 
 * groups on the system. The output is typically directed to a console or log 
 * file and is intended for diagnostic purposes. The output method is implementation-specific,
 * but in this case, it uses `printf` for logging to the console.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - All group settings were successfully dumped.
 * @retval RETURN_ERR - An error occurred during the process of retrieving or
 *                     displaying the group settings.
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int vlan_hal_printAllGroup();

/**
 * @brief Removes all interfaces from a VLAN group.
 *
 * This function deassociates all network interfaces from the specified VLAN 
 * group (`groupName`).
 *
 * @param[in] groupName - The name of the bridge representing the VLAN group from
 *                        which to remove all interfaces (e.g., "brlan0").
 *                        Valid values are: brlan0, brlan1, brlan2, brlan3, brlan4, brlan5,
 *                        brlan7, brlan10, brlan106, brlan403, brlan112, brlan113, brebhaul.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - All interfaces were successfully removed from the group.
 * @retval RETURN_ERR - An error occurred during the removal process.
 *
 * @note Possible errors:
 *       - The VLAN group does not exist.
 *       - System-level errors during interface removal or VLAN configuration.
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int vlan_hal_delete_all_Interfaces(const char *groupName);

/**
 * @brief Checks if a given bridge exists in the Linux bridge system.
 *
 * This utility function determines whether a bridge identified by its name 
 * (`br_name`) is currently present in the Linux bridge configuration. It can be
 * used to verify the existence of a bridge before performing operations on it.
 *
 * @param[in] br_name - The name of the bridge to check (e.g., "brlan0"). Valid
 *                      values are: brlan0, brlan1, brlan2, brlan3, brlan4, brlan5,
 *                      brlan7, brlan10, brlan106, brlan403, brlan112, brlan113, brebhaul.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The bridge exists in the Linux bridge system.
 * @retval RETURN_ERR - The bridge was not found or an error occurred during the
 *                     check.
 */
int _is_this_group_available_in_linux_bridge(char * br_name);

/**
 * @brief Checks if a given interface exists in any Linux bridge.
 *
 * This utility function determines whether a network interface, identified by 
 * its name (`if_name`) and associated VLAN ID (`vlanID`), is currently a member of 
 * any bridge within the Linux bridge system. This can be useful for checking the 
 * VLAN configuration of an interface.
 *
 * @param[in] if_name - The name of the network interface to be checked 
 *                      (e.g., "eth0"). This name is vendor-specific.
 * @param[in] vlanID  - The VLAN ID (1-4094) associated with the interface.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The interface exists within a Linux bridge.
 * @retval RETURN_ERR - The interface was not found in any bridge or an error 
 *                     occurred during the check.
 */
int _is_this_interface_available_in_linux_bridge(char * if_name, char *vlanID);

/**
 * @brief Checks if a specific interface is a member of a given Linux bridge.
 *
 * This utility function determines whether a network interface (`if_name`) with
 * a specific VLAN ID (`vlanID`) is currently associated with a particular
 * Linux bridge (`br_name`).
 *
 * @param[in] if_name - The name of the network interface to be checked 
 *                      (e.g., "eth0"). This name is vendor-specific.
 * @param[in] br_name - The name of the bridge to check for the interface's
 *                      membership (e.g., "brlan0"). Valid values are: brlan0, brlan1, 
 *                      brlan2, brlan3, brlan4, brlan5, brlan7, brlan10, brlan106, 
 *                      brlan403, brlan112, brlan113, brebhaul.
 * @param[in] vlanID  - The VLAN ID (1-4094) associated with the interface.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The interface is a member of the specified bridge with the
 *                     given VLAN ID.
 * @retval RETURN_ERR - The interface is not a member of the bridge or an error 
 *                     occurred during the check.
 */
int _is_this_interface_available_in_given_linux_bridge(char * if_name, char * br_name,char *vlanID);

/**
* @brief This HAL utility is used get the buffer from shell output.
* @param[in] cmd A string representing the Linux shell command that will be executed to obtain the desired output. It will execute and get the buffer from shell output based on given command. It is vendor specific.
* @param[out] out A character array (string) where the output of the input cmd will be copied. It is vendor specific.
* @param[out] len length of the output string.
*                 \n The maximum output length is 512.
*
*
*/
void _get_shell_outputbuffer(char * cmd, char * out, int len);

/**
 * @brief Retrieves output from a file stream (likely from a shell command).
 *
 * This utility function reads data from a `FILE` stream (`fp`), which is
 * typically the output of a shell command, and copies it into a provided
 * buffer (`out`). 
 *
 * @param[in] fp  - Pointer to a `FILE` object representing the input stream.
 * @param[out] out - The buffer to store the data read from the stream.
 * @param[out] len - The length of the output buffer. The maximum output length is 512.
 */
void _get_shell_outputbuffer_res(FILE *fp, char * out, int len);

/**
 * @brief Stores the VLAN ID and group name configuration.
 *
 * This utility function inserts a new configuration entry for a VLAN group
 * into a persistent storage (implementation-specific). The entry associates 
 * the provided `groupName` with the `vlanID`.
 *
 * @param[in] groupName - The name of the VLAN group (bridge name) to store in
 *                        the configuration (e.g., "brlan0"). Valid values are:
 *                        brlan0, brlan1, brlan2, brlan3, brlan4, brlan5, brlan7,
 *                        brlan10, brlan106, brlan403, brlan112, brlan113, brebhaul.
 * @param[in] vlanID - The VLAN ID (1-4094) associated with the `groupName`
 *                     to store in the configuration.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The configuration entry was successfully inserted.
 * @retval RETURN_ERR - An error occurred during the insertion process (e.g.,
 *                     storage error, invalid parameters).
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int insert_VLAN_ConfigEntry(char *groupName, char *vlanID);

/**
 * @brief Deletes the VLAN configuration entry for a group.
 *
 * This utility function removes the configuration entry associated with the 
 * provided `groupName` from a persistent storage (implementation-specific). 
 *
 * @param[in] groupName - The name of the VLAN group (bridge name) whose 
 *                        configuration entry is to be deleted (e.g., "brlan0"). 
 *                        Valid values are: brlan0, brlan1, brlan2, brlan3, brlan4, brlan5,
 *                        brlan7, brlan10, brlan106, brlan403, brlan112, brlan113, brebhaul.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The configuration entry was successfully deleted.
 * @retval RETURN_ERR - An error occurred during the deletion process (e.g., entry 
 *                     not found, storage error).
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int delete_VLAN_ConfigEntry(char *groupName);


/**
 * @brief Retrieves the VLAN ID associated with a given group name.
 *
 * This utility function searches for the VLAN configuration entry corresponding
 * to the specified `groupName` and, if found, retrieves the associated `vlanID`.
 * The `vlanID` is stored in the provided output parameter.
 *
 * @param[in] groupName - The name of the VLAN group (bridge name) to look up 
 *                        (e.g., "brlan0"). Valid values are: brlan0, brlan1, brlan2,
 *                        brlan3, brlan4, brlan5, brlan7, brlan10, brlan106, brlan403, 
 *                        brlan112, brlan113, brebhaul.
 * @param[out] vlanID  - Pointer to a character array where the retrieved VLAN ID
 *                        will be stored.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - The VLAN ID was found and stored in `vlanID`.
 * @retval RETURN_ERR - The group name was not found, or an error occurred during 
 *                     the retrieval process.
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int get_vlanId_for_GroupName(const char *groupName, char *vlanID);

/**
 * @brief Prints all stored VLAN ID and group name configurations.
 *
 * This function retrieves and displays all VLAN configuration entries, typically 
 * from a persistent storage (implementation-specific). Each entry will include 
 * the VLAN group name and its associated VLAN ID. The output is likely directed 
 * to a console or log for debugging purposes.
 *
 * @returns The status of the operation.
 * @retval RETURN_OK - All configurations were successfully printed.
 * @retval RETURN_ERR - An error occurred during the retrieval or display process.
 *
 * @todo Refactor return codes to use a more specific and informative enum (see
 *       general TODO comment).
 */
int print_all_vlanId_Configuration(void);

/** @} */  //END OF GROUP VLAN_HAL_APIS

/*
Example usage on Puma6 platfom:
        Private network group (vlan100)
                brlan0      eth_1(external swith port 1), eth_2(port 2), eth_3(port 3),  MoCA.100 (MoCA vlan100),  ath0(2.4G vlan100), ath1 (5G vlan100)

vlan_hal_addGroup("brlan0", "100");                //!< brctl addbr brlan0
vlan_hal_addInterface("brlan0", "l2sd0", NULL);    //!< vconfig add l2sd0 100; brctl addif brlan0 l2sd0.100


        Xfinity Home VLAN (vlan101)
                brlan1      eth_4(port 4),  MoCA.101(MoCA vlan101), ath2(2.4G vlan101), ath3 (5G vlan101)

vlan_hal_addGroup("brlan1", "101");                //!< brctl addbr brlan1
vlan_hal_addInterface("brlan1", "l2sd0", NULL);    //!< vconfig add l2sd0 100; brctl addif brlan1 l2sd0.101


        Xfinity Wifi VLAN 2.4G (vlan102)
                brlan2     gretap0.102 (GRE vlan102),  MoCA.102(MoCA vlan102), ath4(2.4G vlan102)

vlan_hal_addGroup("brlan2", "102");                //!< brctl addbr brlan2
vlan_hal_addInterface("brlan2", "l2sd0", NULL);    //!< vconfig add l2sd0 102; brctl addif brlan1 l2sd0.102
vlan_hal_addInterface("brlan2", "gretap0", NULL);  //!< vconfig add gretap0 102; brctl addif brlan1 gretap0.102


        Xfinity Wifi VLAN 5G (vlan103)
                brlan3     gretap0.103 (GRE vlan103),  MoCA.103(MoCA vlan103), ath5(2.4G vlan103)

vlan_hal_addGroup("brlan3", "103");                //!< brctl addbr brlan3
vlan_hal_addInterface("brlan3", "l2sd0", NULL);    //!< vconfig add l2sd0 103; brctl addif brlan1 l2sd0.103
vlan_hal_addInterface("brlan3", "gretap0", NULL);  //!< vconfig add gretap0 103; brctl addif brlan1 gretap0.103

*/
#endif /*__VLAN_HAL_H__*/
