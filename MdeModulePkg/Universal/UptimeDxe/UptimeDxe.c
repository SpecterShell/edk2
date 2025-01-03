/** @file
  Implementation of Uptime Protocol using UEFI APIs.
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Library/TimeBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/Uptime.h>


// The starting timestamp
UINTN  StartingTimestamp = 0;


/**
  Retrieves the uptime in seconds since the driver is loaded.

  @retval The timestamp in seconds

**/
UINTN
EFIAPI
UptimeDriverGetUptime (
  VOID
  )
{
  EFI_TIME    ET;
  UINTN       CurrentTimestamp;
  UINTN       DiffTimestamp;

  gRT->GetTime (&ET, NULL);
  CurrentTimestamp = EfiTimeToEpoch (&ET);
  DiffTimestamp = CurrentTimestamp - StartingTimestamp;
  DEBUG ((DEBUG_INFO, "Current timestamp: %llu; Starting timestamp: %llu; Diff timestamp: %llu\n",
          CurrentTimestamp, StartingTimestamp, DiffTimestamp));

  return DiffTimestamp;
}

/**
  Retrieves the timestamp in seconds when the driver is loaded.

  @retval The timestamp in seconds

**/
UINTN
EFIAPI
UptimeDriverGetStartingTimestamp (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "Starting timestamp: %llu\n", StartingTimestamp));

  return StartingTimestamp;
}

/**
  Retrieves the timestamp in seconds when this function is invoked.

  @retval The timestamp in seconds

**/
UINTN
EFIAPI
UptimeDriverGetCurrentTimestamp (
  VOID
  )
{
  EFI_TIME    ET;
  UINTN       CurrentTimestamp;

  gRT->GetTime (&ET, NULL);
  CurrentTimestamp = EfiTimeToEpoch (&ET);
  DEBUG ((DEBUG_INFO, "Current timestamp: %llu\n", CurrentTimestamp));

  return CurrentTimestamp;
}

// The Uptime Protocol instance produced by this driver
EFI_UPTIME_PROTOCOL  mUptime = {
  UptimeDriverGetUptime,
  UptimeDriverGetStartingTimestamp,
  UptimeDriverGetCurrentTimestamp
};

/**
  Entry point of the Uptime Protocol driver.

  @param  ImageHandle   The image handle of this driver.
  @param  SystemTable   The pointer of EFI_SYSTEM_TABLE.

  @retval EFI_SUCCESS   Watchdog Timer Architectural Protocol successfully installed.

**/
EFI_STATUS
EFIAPI
UptimeDriverInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_TIME    ET;

  gRT->GetTime (&ET, NULL);
  StartingTimestamp = EfiTimeToEpoch (&ET);
  DEBUG ((DEBUG_INFO, "Current timestamp: %llu\n", StartingTimestamp));

  // Install the Uptime Protocol onto a new handle
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiUptimeProtocolGuid,
                  &mUptime,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
