/** @file
  This sample application reads and writes to the flash device using the Flash protocol.
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/Flash.h>

EFI_GUID gEfiFlashProtocolGuid = EFI_FLASH_PROTOCOL_GUID;
EFI_GUID gEfiFlashVendorGuid = EFI_FLASH_VENDOR_GUID;

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
  EFI_FLASH_PROTOCOL   *Flash;

  Status = gBS->LocateProtocol (&gEfiFlashProtocolGuid, NULL, (VOID **)&Flash);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to load the protocol\n");
    return Status;
  }

  UINT8 *FlashBase;
  UINTN FlashSize;
  Status = Flash->GetFlashProperty(&FlashBase, &FlashSize);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to get the flash property\n");
    return Status;
  }
  Print(L"The flash base is %p\n", FlashBase);
  Print(L"The flash size is %llu bytes\n", FlashSize);

  UINT8 Data[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
  Status = Flash->WriteFlash(10, 16, Data);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to write the flash\n");
    return Status;
  }

  UINT8 ReadData[16] = {0};
  Status = Flash->ReadFlash(10, 16, ReadData);
  if (EFI_ERROR(Status)) {
    Print (L"Failed to read the flash\n");
    return Status;
  }
  Print(L"The first 16 bytes of the flash are:\n");
  for (UINTN i = 0; i < 16; i++) {
    Print(L"%c,", ReadData[i]);
  }
  Print(L"\n");

  return EFI_SUCCESS;
}
