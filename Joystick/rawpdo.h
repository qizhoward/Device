//
#pragma warning(default:4201)

#include <initguid.h>
#include <devguid.h>
#include <wdf.h>
#include <public.h>


#ifndef _RAWPDO_H
#define _RAWPDO_H

#ifndef RTL_BITS_OF
// This macro is not defined in Win2k ntdef.h
#define RTL_BITS_OF(sizeOfArg) (sizeof(sizeOfArg) * 8)
#endif

//
// Used to identify Joystick bus. This guid is used as the enumeration string
// for the device id.
// {D6E55CA0-1A2E-4234-AAF3-3852170B492F}
DEFINE_GUID(GUID_BUS_Joystick_RAWPDO,
0xd6e55ca0, 0x1a2e, 0x4234, 0xaa, 0xf3, 0x38, 0x52, 0x17, 0xb, 0x49, 0x2f);

//// {A4221D72-A5E8-493C-96D6-E18CD359106D}
//DEFINE_GUID(GUID_DEVINTERFACE_Joystick,
//0xa4221d72, 0xa5e8, 0x493c, 0x96, 0xd6, 0xe1, 0x8c, 0xd3, 0x59, 0x10, 0x6d);


// \0 in the end is for double termination - required for MULTI_SZ string
#define  Joystick_RAW_DEVICE_ID		L"{D6E55CA0-1A2E-4234-AAF3-3852170B492F}\\JoystickRawPdo\0"
#define  Joystick_HARDWARE_ID		L"{D6E55CA0-1A2E-4234-AAF3-3852170B492F}\\JoystickRawPdoHw\0"
#define  Joystick_DEVICE_TEXT_409	L"Joystick Driver"
//#define  Joystick_DEVICE_TEXT_40D	L"התקן גולמי Joystick %02d"
#define  Joystick_DEVICE_INSTANCE	L"JoystickInstance%02d"

typedef struct _RPDO_DEVICE_DATA
{

    ULONG InstanceNo;

	WDFDEVICE hParentDevice;

	WDFIOTARGET	IoTargetToParent;

	USHORT UsedInterfacesMask;

} RPDO_DEVICE_DATA, *PRPDO_DEVICE_DATA;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(RPDO_DEVICE_DATA, PdoGetData)

#if (KMDF_MINOR_VERSION==005)
typedef struct _WDF_REQUEST_FORWARD_OPTIONS {
  ULONG Size;
  ULONG Flags;
} WDF_REQUEST_FORWARD_OPTIONS, *PWDF_REQUEST_FORWARD_OPTIONS;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////

// File Object context area
typedef struct _FILEOBJECT_EXTENSION{

	// Every file object represent one of the top-level devices
	// Each top-level device is marked is an id (1 to 16)
	int id;

	// Every valid file object is associated with one device ID (id>0)
	// (id < 1 is saved for other uses)
	// Every device can be owned by at most one process	- CallingProcessId holds the ID of the calling process.
	// Valid CallingProcessId>4
	DWORD CallingProcessId;

	// pointer to the parent Raw PDO Device context area
	PRPDO_DEVICE_DATA pParentRawDeviceContext;

} FILEOBJECT_EXTENSION, * PFILEOBJECT_EXTENSION;

// Declare Accessor function GetFileObjectContext()
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(FILEOBJECT_EXTENSION, GetFileObjectContext)

////////////////////////////////////////////////////////////////////////////////////////////////

NTSTATUS
Joystick_CreateRawPdo(
    WDFDEVICE       Device,
    ULONG           InstanceNo
);

VOID
Joystick_EvtIoDeviceControlForRawPdo(
    IN WDFQUEUE      Queue,
    IN WDFREQUEST    Request,
    IN size_t        OutputBufferLength,
    IN size_t        InputBufferLength,
    IN ULONG         IoControlCode
    );

VOID
rawEvtCleanupCallback(
    IN WDFDEVICE Device
    );

VOID Joystick_EvtDeviceFileCreate(
  __in  WDFDEVICE Device,
  __in  WDFREQUEST Request,
  __in  WDFFILEOBJECT FileObject
);

VOID Joystick_EvtFileCleanup(
  __in  WDFFILEOBJECT FileObject
);

#endif