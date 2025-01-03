/** @file
  This sample application retrieves and prints the uptime in seconds since the driver is loaded to the UEFI Console.
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Uptime.h>

EFI_GUID gEfiUptimeProtocolGuid = EFI_UPTIME_PROTOCOL_GUID;

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
  EFI_UPTIME_PROTOCOL *Uptime;
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gEfiUptimeProtocolGuid, NULL, (VOID **)&Uptime);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to load the protocol\n");
    return Status;
  }

  Print(L"The uptime is %llu seconds\n", Uptime->GetUptime ());
  Print(L"The current timestamp is %llu seconds\n", Uptime->GetCurrentTimestamp ());
  Print(L"The starting timestamp is %llu seconds\n", Uptime->GetStartingTimestamp ());

  return EFI_SUCCESS;
}
