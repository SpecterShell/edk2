/** @file
  Implementation of Flash Protocol using UEFI APIs.
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/Flash.h>

#define WRITE_BYTE_CMD  0x10
#define READ_ARRAY_CMD  0xff
#define CODE_BASE       0x0
#define BLOCK_BASE      0x3C0000

STATIC UINT8      *mFlashBase = NULL;
STATIC UINTN      mFlashSize = 0;

/**
  Retrieves the flash size in bytes.

  @retval EFI_SUCCESS   The flash size is returned successfully.

**/
EFI_STATUS
EFIAPI
FlashDriverGetFlashProperty (
  OUT UINT8  **FlashBase,
  OUT UINTN  *FlashSize
  )
{
  *FlashBase = mFlashBase;
  *FlashSize = mFlashSize;

  return EFI_SUCCESS;
}

/**
  Retrieves the flash data.

  @param  Offset        The offset in the flash to read.
  @param  BufferSize    The size of the data to read.
  @param  Buffer        The buffer to store the data.

  @retval EFI_SUCCESS   The flash data is read successfully.
**/
EFI_STATUS
EFIAPI
FlashDriverReadFlash (
  IN     UINTN  Offset,
  IN OUT UINTN  BufferSize,
  OUT    UINT8  *Buffer
  )
{
  // Check if the flash base is valid
  if (mFlashBase == NULL) {
    DEBUG ((DEBUG_INFO, "The flash base is NULL\n"));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "Reading data from %p\n", mFlashBase + BLOCK_BASE + Offset));

  // Check if the offset and buffer size are valid
  if (BLOCK_BASE + Offset >= mFlashSize) {
    DEBUG ((DEBUG_INFO, "The offset exceeds the capacity\n"));
    BufferSize = 0;
  } else if (BLOCK_BASE + Offset + BufferSize > mFlashSize) {
    DEBUG ((DEBUG_INFO, "The offset plus buffer size exceeds the capacity\n"));
    BufferSize = mFlashSize - Offset - BLOCK_BASE;
  } else {
    DEBUG ((DEBUG_INFO, "The offset and buffer size are all good\n"));
  }

  DEBUG ((DEBUG_INFO, "The first byte is %c\n", *(mFlashBase + BLOCK_BASE + Offset)));
  CopyMem (Buffer, mFlashBase + BLOCK_BASE + Offset, BufferSize);

  return EFI_SUCCESS;
}

/**
  Writes the flash data.

  @param  Offset        The offset in the flash to write.
  @param  BufferSize    The size of the data to write.
  @param  Buffer        The buffer to store the data.

  @retval EFI_SUCCESS   The flash data is written successfully.
**/
EFI_STATUS
EFIAPI
FlashDriverWriteFlash (
  IN     UINTN  Offset,
  IN OUT UINTN  BufferSize,
  OUT    UINT8  *Buffer
  )
{
  UINT8       *Ptr;
  UINTN       i;

  // Check if the flash base is valid
  if (mFlashBase == NULL) {
    DEBUG ((DEBUG_INFO, "The flash base is NULL\n"));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "Writing data to %llu\n", Offset));
  // Check if the offset and buffer size are valid
  if (BLOCK_BASE + Offset >= mFlashSize) {
    DEBUG ((DEBUG_INFO, "The offset exceeds the capacity\n", Offset));
    BufferSize = 0;
  } else if (BLOCK_BASE + Offset + BufferSize > mFlashSize) {
    DEBUG ((DEBUG_INFO, "The offset plus buffer size exceeds the capacity\n", Offset));
    BufferSize = mFlashSize - Offset - BLOCK_BASE;
  } else {
    DEBUG ((DEBUG_INFO, "The offset and buffer size are all good\n", Offset));
  }

  Ptr = mFlashBase + BLOCK_BASE + Offset;
  for (i = 0; i < BufferSize; ++i, ++Ptr) {
    // Set the flash to write mode
    *Ptr = WRITE_BYTE_CMD;
    DEBUG ((DEBUG_INFO, "Writing byte %c to %p\n", Buffer[i], Ptr));
    *Ptr = Buffer[i];
    DEBUG ((DEBUG_INFO, "Wrote byte %c to %p\n", *Ptr, Ptr));
  }
  if (BufferSize > 0) {
    // Restore the flash to read mode
    *(Ptr - 1) = READ_ARRAY_CMD;
  }

  return EFI_SUCCESS;
}

// The Flash Protocol instance produced by this driver
EFI_FLASH_PROTOCOL  mFlash = {
  FlashDriverGetFlashProperty,
  FlashDriverReadFlash,
  FlashDriverWriteFlash
};

/**
  Entry point of the Flash Protocol driver.

  @param  ImageHandle   The image handle of this driver.
  @param  SystemTable   The pointer of EFI_SYSTEM_TABLE.

  @retval EFI_SUCCESS   Watchdog Timer Architectural Protocol successfully installed.

**/
EFI_STATUS
EFIAPI
FlashDriverInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  // Get the flash base address from PCD
  mFlashBase = (VOID *)(UINTN)PcdGet32 (PcdOvmfFdBaseAddress);
  DEBUG ((DEBUG_INFO, "Flash base address: %p\n", mFlashBase));

  // Get the flash size (n_blocks * block size) from PCD
  mFlashSize = PcdGet32 (PcdOvmfFirmwareFdSize);
  DEBUG ((DEBUG_INFO, "Flash size: %llu\n", mFlashSize));

  // Install the Flash Protocol onto a new handle
  Status = gBS->InstallMultipleProtocolInterfaces (
    &ImageHandle,
    &gEfiFlashProtocolGuid,
    &mFlash,
    NULL
  );
  ASSERT_EFI_ERROR (Status);

  // Register the Flash Protocol instance as a variable
  Status = gRT->SetVariable(
    L"FlashDriverRuntimeService",
    &gEfiFlashVendorGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    sizeof(mFlash),
    &mFlash
  );
  ASSERT_EFI_ERROR (Status);

  // Register the Flash Protocol instance to the UEFI Runtime Services Table
  gRT->GetFlashProperty = FlashDriverGetFlashProperty;
  gRT->ReadFlash = FlashDriverReadFlash;
  gRT->WriteFlash = FlashDriverWriteFlash;

  return EFI_SUCCESS;
}
