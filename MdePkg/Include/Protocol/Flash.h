/** @file
  EFI Flash Protocol
  Used to provide a platform independent interface for retrieving the flash in seconds since the driver is loaded.
**/

#ifndef __EFI_FLASH_PROTOCOL_H__
#define __EFI_FLASH_PROTOCOL_H__

#define EFI_FLASH_PROTOCOL_GUID \
  { 0xafbfdea2, 0x2e6e, 0x4262, {0xba, 0x65, 0x62, 0xb9, 0x23, 0x6e, 0x54, 0x95 } }
#define EFI_FLASH_VENDOR_GUID \
  { 0xafbfdea2, 0x2e6e, 0x4362, {0xba, 0x65, 0x62, 0xb9, 0x23, 0x6e, 0x54, 0x95 } }

/// Declare forward reference for the Time Stamp Protocol
typedef struct _EFI_FLASH_PROTOCOL EFI_FLASH_PROTOCOL;

/**
  Retrieves the flash in seconds since the driver is loaded.

  @retval The timestamp in seconds

**/
typedef
EFI_STATUS
(EFIAPI *EFI_FLASH_GET_PROPERTY)(
  OUT UINT8  **FlashBase,
  OUT UINTN  *FlashSize
  );

/**
  Retrieves the timestamp in seconds when the driver is loaded.

  @retval The timestamp in seconds

**/
typedef
EFI_STATUS
(EFIAPI *EFI_FLASH_READ)(
  IN     UINTN  Offset,
  IN OUT UINTN  DataSize,
  OUT    UINT8  *Data
  );

/**
  Retrieves the timestamp in seconds when this function is invoked.

  @retval The timestamp in seconds

**/
typedef
EFI_STATUS
(EFIAPI *EFI_FLASH_WRITE)(
  IN     UINTN  Offset,
  IN OUT UINTN  DataSize,
  OUT    UINT8  *Data
  );

/// EFI_FLASH_PROTOCOL
/// The protocol provides a platform independent interface for retrieving a high resolution
/// flash operator.
struct _EFI_FLASH_PROTOCOL {
  EFI_FLASH_GET_PROPERTY  GetFlashProperty;
  EFI_FLASH_READ          ReadFlash;
  EFI_FLASH_WRITE         WriteFlash;
};

extern EFI_GUID  gEfiFlashProtocolGuid;
extern EFI_GUID  gEfiFlashVendorGuid;

#endif
