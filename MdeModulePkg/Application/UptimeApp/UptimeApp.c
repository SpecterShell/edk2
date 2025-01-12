/** @file
  This sample application retrieves and prints the uptime in seconds since the driver is loaded to the UEFI Console.
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/Uptime.h>

EFI_GUID gEfiUptimeProtocolGuid = EFI_UPTIME_PROTOCOL_GUID;
EFI_GUID gEfiUptimeVendorGuid = EFI_UPTIME_VENDOR_GUID;

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS           Status;
  EFI_UPTIME_PROTOCOL  *Uptime;
  UINTN                Timestamp;

  Status = gBS->LocateProtocol (&gEfiUptimeProtocolGuid, NULL, (VOID **)&Uptime);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to load the protocol\n");
    return Status;
  }

  Status = Uptime->GetUptime (&Timestamp);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to get the uptime\n");
    return Status;
  }
  Print(L"The uptime is %llu seconds\n", Timestamp);

  Status = Uptime->GetStartingTimestamp (&Timestamp);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to get the starting timestamp\n");
    return Status;
  }
  Print(L"The starting timestamp is %llu seconds\n", Timestamp);

  Status = Uptime->GetCurrentTimestamp (&Timestamp);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to get the current timestamp\n");
    return Status;
  }
  Print(L"The current timestamp is %llu seconds\n", Timestamp);

  EFI_UPTIME_PROTOCOL mUptime;
  UINTN mUptimeSize = sizeof (mUptime);
  Status = gRT->GetVariable(
    L"UptimeDriverRuntimeService",
    &gEfiUptimeVendorGuid,
    NULL,
    &mUptimeSize,
    &mUptime
  );
  if (EFI_ERROR(Status)) {
    Print (L"Failed to load the variable\n");
    return Status;
  }

  Status = mUptime.GetUptime(&Timestamp);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to get the uptime from variable\n");
    return Status;
  }
  Print(L"The uptime is %llu seconds\n", Timestamp);

  Status = mUptime.GetStartingTimestamp(&Timestamp);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to get the starting timestamp from variable\n");
    return Status;
  }
  Print(L"The starting timestamp is %llu seconds\n", Timestamp);

  Status = mUptime.GetCurrentTimestamp(&Timestamp);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to get the current timestamp from variable\n");
    return Status;
  }
  Print(L"The current timestamp is %llu seconds\n", Timestamp);

  return EFI_SUCCESS;
}
