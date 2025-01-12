/** @file
  EFI Uptime Protocol
  Used to provide a platform independent interface for retrieving the uptime in seconds since the driver is loaded.
**/

#ifndef __EFI_UPTIME_PROTOCOL_H__
#define __EFI_UPTIME_PROTOCOL_H__

#define EFI_UPTIME_PROTOCOL_GUID \
  { 0xafbfde42, 0x2e6e, 0x4262, {0xba, 0x65, 0x62, 0xb9, 0x23, 0x6e, 0x54, 0x95 } }
#define EFI_UPTIME_VENDOR_GUID \
  { 0xafbfde42, 0x2e6e, 0x4362, {0xba, 0x65, 0x62, 0xb9, 0x23, 0x6e, 0x54, 0x95 } }

/// Declare forward reference for the Time Stamp Protocol
typedef struct _EFI_UPTIME_PROTOCOL EFI_UPTIME_PROTOCOL;

/**
  Retrieves the uptime in seconds since the driver is loaded.

  @retval The timestamp in seconds

**/
typedef
EFI_STATUS
(EFIAPI *EFI_UPTIME_GET_ELAPSED)(
  OUT UINTN  *Elapsed
  );

/**
  Retrieves the timestamp in seconds when the driver is loaded.

  @retval The timestamp in seconds

**/
typedef
EFI_STATUS
(EFIAPI *EFI_UPTIME_GET_STARTING)(
  OUT UINTN  *Starting
  );

/**
  Retrieves the timestamp in seconds when this function is invoked.

  @retval The timestamp in seconds

**/
typedef
EFI_STATUS
(EFIAPI *EFI_UPTIME_GET_CURRENT)(
  OUT UINTN  *Current
  );

/// EFI_UPTIME_PROTOCOL
/// The protocol provides a platform independent interface for retrieving a high resolution
/// uptime counter.
struct _EFI_UPTIME_PROTOCOL {
  EFI_UPTIME_GET_ELAPSED       GetUptime;
  EFI_UPTIME_GET_STARTING      GetStartingTimestamp;
  EFI_UPTIME_GET_CURRENT       GetCurrentTimestamp;
};

extern EFI_GUID  gEfiUptimeProtocolGuid;
extern EFI_GUID  gEfiUptimeVendorGuid;

#endif
