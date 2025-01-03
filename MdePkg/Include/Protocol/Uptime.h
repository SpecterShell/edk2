/** @file
  EFI Uptime Protocol
  Used to provide a platform independent interface for retrieving the uptime in seconds since the driver is loaded.
**/

#ifndef __EFI_UPTIME_PROTOCOL_H__
#define __EFI_UPTIME_PROTOCOL_H__

#define EFI_UPTIME_PROTOCOL_GUID \
  { 0xafbfde42, 0x2e6e, 0x4262, {0xba, 0x65, 0x62, 0xb9, 0x23, 0x6e, 0x54, 0x95 } }

/// Declare forward reference for the Time Stamp Protocol
typedef struct _EFI_UPTIME_PROTOCOL EFI_UPTIME_PROTOCOL;

/**
  Retrieves the uptime in seconds since the driver is loaded.

  @retval The timestamp in seconds

**/
typedef
UINTN
(EFIAPI *UPTIME_GET_ELAPSED)(
  VOID
  );

/**
  Retrieves the timestamp in seconds when the driver is loaded.

  @retval The timestamp in seconds

**/
typedef
UINTN
(EFIAPI *UPTIME_GET_STARTING)(
  VOID
  );

/**
  Retrieves the timestamp in seconds when this function is invoked.

  @retval The timestamp in seconds

**/
typedef
UINTN
(EFIAPI *UPTIME_GET_CURRENT)(
  VOID
  );

/// EFI_UPTIME_PROTOCOL
/// The protocol provides a platform independent interface for retrieving a high resolution
/// uptime counter.
struct _EFI_UPTIME_PROTOCOL {
  UPTIME_GET_ELAPSED       GetUptime;
  UPTIME_GET_STARTING      GetStartingTimestamp;
  UPTIME_GET_CURRENT       GetCurrentTimestamp;
};

extern EFI_GUID  gEfiUptimeProtocolGuid;

#endif
