/*
 * Copyright (c) 2013 Fundacion Jala
 * Module Name: sauronFilter.c
 * Environment: Kernel mode
 */

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>

#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")

//Global variables

#define SAURON_FILTER_NAME     L"SauronFilter"

typedef struct _SAURON_FILTER_DATA {
    // The filter handle that results from a call to FltRegisterFilter.
    PFLT_FILTER FilterHandle;

} SAURON_FILTER_DATA, *PSAURON_FILTER_DATA;


/*************************************************************************
    Prototypes for the startup and unload routines used for
    this Filter.
*************************************************************************/

DRIVER_INITIALIZE DriverEntry;
NTSTATUS
DriverEntry (
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
    );

NTSTATUS
SauronUnload (
    __in FLT_FILTER_UNLOAD_FLAGS Flags
    );

NTSTATUS
SauronQueryTeardown (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
    );

//  Structure that contains all the global data structures used throughout SauronFilter.

SAURON_FILTER_DATA SauronFilterData;

//  Assign text sections for each routine.

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, SauronUnload)
#pragma alloc_text(PAGE, SauronQueryTeardown)
#endif

//  This defines what we want to filter with FltMgr
CONST FLT_OPERATION_REGISTRATION Callbacks[] = { 
      {IRP_MJ_CREATE},
      {IRP_MJ_CLOSE},
      {IRP_MJ_READ},
      {IRP_MJ_WRITE},
      {IRP_MJ_OPERATION_END}

};


CONST FLT_REGISTRATION FilterRegistration = {

    sizeof( FLT_REGISTRATION ),         //  Size
    FLT_REGISTRATION_VERSION,           //  Version
    0,                                  //  Flags

    NULL,                               //  Context
    Callbacks,                          //  Operation callbacks

    SauronUnload,                         //  FilterUnload

    NULL,                               //  InstanceSetup
    SauronQueryTeardown,                  //  InstanceQueryTeardown
    NULL,                               //  InstanceTeardownStart
    NULL,                               //  InstanceTeardownComplete

    NULL,                               //  GenerateFileName
    NULL,                               //  GenerateDestinationFileName
    NULL                                //  NormalizeNameComponent

};


/*************************************************************************
    Filter initialization and unload routines.
*************************************************************************/

NTSTATUS
DriverEntry (
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:

    This is the initialization routine for this miniFilter driver. This
    registers the miniFilter with FltMgr and initializes all
    its global data structures.

Arguments:

    DriverObject - Pointer to driver object created by the system to
        represent this driver.
    RegistryPath - Unicode string identifying where the parameters for this
        driver are located in the registry.

Return Value:

    Returns STATUS_SUCCESS.

--*/
{
    NTSTATUS status;

    UNREFERENCED_PARAMETER( RegistryPath );

    //
    //  Register with FltMgr
    //

    status = FltRegisterFilter( DriverObject,
                                &FilterRegistration,
                                &SauronFilterData.FilterHandle );

    ASSERT( NT_SUCCESS( status ) );

    if (NT_SUCCESS( status )) {

        //
        //  Start filtering i/o
        //

        status = FltStartFiltering( SauronFilterData.FilterHandle );

        if (!NT_SUCCESS( status )) {
            FltUnregisterFilter( SauronFilterData.FilterHandle );
        }
    }
    return status;
}

NTSTATUS
SauronUnload (
    __in FLT_FILTER_UNLOAD_FLAGS Flags
    )
/*++

Routine Description:

    This is the unload routine for this miniFilter driver. This is called
    when the minifilter is about to be unloaded. We can fail this unload
    request if this is not a mandatory unloaded indicated by the Flags
    parameter.

Arguments:

    Flags - Indicating if this is a mandatory unload.

Return Value:

    Returns the final status of this operation.

--*/
{
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    FltUnregisterFilter( SauronFilterData.FilterHandle );

    return STATUS_SUCCESS;
}


NTSTATUS
SauronQueryTeardown (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
    )
/*++

Routine Description:

    This is the instance detach routine for this miniFilter driver.
    This is called when an instance is being manually deleted by a
    call to FltDetachVolume or FilterDetach thereby giving us a
    chance to fail that detach request.

Arguments:

    FltObjects - Pointer to the FLT_RELATED_OBJECTS data structure containing
        opaque handles to this filter, instance and its associated volume.

    Flags - Indicating where this detach request came from.

Return Value:

    Returns the status of this operation.

--*/
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );
    PAGED_CODE();
    return STATUS_SUCCESS;
}