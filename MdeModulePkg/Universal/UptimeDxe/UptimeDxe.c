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
STATIC UINTN mStartingTimestamp = 0;

/**
  Retrieves the uptime in seconds since the driver is loaded.

  @retval The timestamp in seconds

**/
EFI_STATUS
EFIAPI
UptimeDriverGetUptime (
  OUT UINTN  *Elapsed
  )
{
  EFI_TIME    ET;
  UINTN       CurrentTimestamp;

  gRT->GetTime (&ET, NULL);
  DEBUG ((DEBUG_INFO, "Current time: %04u-%02u-%02u %02u:%02u:%02u\n", ET.Year, ET.Month, ET.Day, ET.Hour, ET.Minute, ET.Second));
  CurrentTimestamp = EfiTimeToEpoch (&ET);
  *Elapsed = CurrentTimestamp - mStartingTimestamp;
  DEBUG ((DEBUG_INFO, "Current timestamp: %llu; Starting timestamp: %llu; Diff timestamp: %llu\n",
          CurrentTimestamp, mStartingTimestamp, *Elapsed));

  return EFI_SUCCESS;
}

/**
  Retrieves the timestamp in seconds when the driver is loaded.

  @retval The timestamp in seconds

**/
EFI_STATUS
EFIAPI
UptimeDriverGetStartingTimestamp (
  OUT UINTN  *Starting
  )
{
  *Starting = mStartingTimestamp;
  DEBUG ((DEBUG_INFO, "Starting timestamp: %llu\n", mStartingTimestamp));

  return EFI_SUCCESS;
}

/**
  Retrieves the timestamp in seconds when this function is invoked.

  @retval The timestamp in seconds

**/
EFI_STATUS
EFIAPI
UptimeDriverGetCurrentTimestamp (
  OUT UINTN  *Current
  )
{
  EFI_TIME  ET;

  gRT->GetTime (&ET, NULL);
  DEBUG ((DEBUG_INFO, "Current time: %04u-%02u-%02u %02u:%02u:%02u\n", ET.Year, ET.Month, ET.Day, ET.Hour, ET.Minute, ET.Second));
  *Current = EfiTimeToEpoch (&ET);
  DEBUG ((DEBUG_INFO, "Current timestamp: %llu\n", *Current));

  return EFI_SUCCESS;
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
  DEBUG ((DEBUG_INFO, "Current time: %04u-%02u-%02u %02u:%02u:%02u\n", ET.Year, ET.Month, ET.Day, ET.Hour, ET.Minute, ET.Second));
  mStartingTimestamp = EfiTimeToEpoch (&ET);
  DEBUG ((DEBUG_INFO, "Current timestamp: %llu\n", mStartingTimestamp));

  // Install the Uptime Protocol onto a new handle
  Status = gBS->InstallMultipleProtocolInterfaces (
    &ImageHandle,
    &gEfiUptimeProtocolGuid,
    &mUptime,
    NULL
  );
  ASSERT_EFI_ERROR (Status);

  // Register the Uptime Protocol instance as a variable
  Status = gRT->SetVariable(
    L"UptimeDriverRuntimeService",
    &gEfiUptimeVendorGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    sizeof(mUptime),
    &mUptime
  );
  ASSERT_EFI_ERROR (Status);

  // Register the Uptime Protocol instance to the UEFI Runtime Services Table
  gRT->GetUptime = UptimeDriverGetUptime;
  gRT->GetStartingTimestamp = UptimeDriverGetStartingTimestamp;
  gRT->GetCurrentTimestamp = UptimeDriverGetCurrentTimestamp;

  return EFI_SUCCESS;
}
