/*
 * MountMgr Unix interface definitions
 *
 * Copyright 2021 Alexandre Julliard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "mountmgr.h"
#include "wine/unixlib.h"

enum device_op
{
    ADD_DOS_DEVICE,
    ADD_VOLUME,
    REMOVE_DEVICE
};

struct device_info
{
    enum device_op    op;
    enum device_type  type;
    const char       *udi;
    const char       *device;
    const char       *mount_point;
    const char       *serial;
    GUID             *guid;
    struct scsi_info *scsi_info;

    /* buffer space for pointers */
    GUID              guid_buffer;
    struct scsi_info  scsi_buffer;
    char              str_buffer[1024];
};

struct run_loop_params
{
    HANDLE     op_thread;
    PNTAPCFUNC op_apc;
};

struct dequeue_device_op_params
{
    ULONG_PTR arg;
    struct device_info *info;
};

struct add_drive_params
{
    const char *device;
    enum device_type type;
    int *letter;
};

struct get_dosdev_symlink_params
{
    const char *dev;
    char *dest;
    ULONG size;
};

struct set_dosdev_symlink_params
{
    const char *dev;
    const char *dest;
};

struct get_volume_dos_devices_params
{
    const char *mount_point;
    unsigned int *dosdev;
};

struct read_volume_file_params
{
    const char *volume;
    const char *file;
    void *buffer;
    ULONG *size;
};

struct match_unixdev_params
{
    const char *device;
    ULONGLONG unix_dev;
};

struct detect_ports_params
{
    char *names;
    ULONG size;
};

struct set_shell_folder_params
{
    const char *folder;
    const char *backup;
    const char *link;
};

struct get_shell_folder_params
{
    const char *folder;
    char *buffer;
    ULONG size;
};

struct dhcp_request_params
{
    const char *unix_name;
    struct mountmgr_dhcp_request_param *req;
    char  *buffer;
    ULONG  offset;
    ULONG  size;
    ULONG *ret_size;
};

enum mountmgr_funcs
{
    unix_run_loop,
    unix_dequeue_device_op,
    unix_add_drive,
    unix_get_dosdev_symlink,
    unix_set_dosdev_symlink,
    unix_get_volume_dos_devices,
    unix_read_volume_file,
    unix_match_unixdev,
    unix_detect_serial_ports,
    unix_detect_parallel_ports,
    unix_set_shell_folder,
    unix_get_shell_folder,
    unix_dhcp_request,
};

extern unixlib_handle_t mountmgr_handle;

#define MOUNTMGR_CALL( func, params ) __wine_unix_call( mountmgr_handle, unix_ ## func, params )

extern void queue_device_op( enum device_op op, const char *udi, const char *device,
                             const char *mount_point, enum device_type type, const GUID *guid,
                             const char *disk_serial, const struct scsi_info *info ) DECLSPEC_HIDDEN;
extern void run_dbus_loop(void) DECLSPEC_HIDDEN;
extern void run_diskarbitration_loop(void) DECLSPEC_HIDDEN;

extern NTSTATUS dhcp_request( void *args ) DECLSPEC_HIDDEN;
extern NTSTATUS query_symbol_file( void *buff, ULONG insize, ULONG outsize, ULONG *info ) DECLSPEC_HIDDEN;
extern NTSTATUS read_credential( void *buff, ULONG insize, ULONG outsize, ULONG *info ) DECLSPEC_HIDDEN;
extern NTSTATUS write_credential( void *buff, ULONG insize, ULONG outsize, ULONG *info ) DECLSPEC_HIDDEN;
extern NTSTATUS delete_credential( void *buff, ULONG insize, ULONG outsize, ULONG *info ) DECLSPEC_HIDDEN;
extern NTSTATUS enumerate_credentials( void *buff, ULONG insize, ULONG outsize, ULONG *info ) DECLSPEC_HIDDEN;
