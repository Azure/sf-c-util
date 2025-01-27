# Copyright (C) Microsoft Corporation. All rights reserved.
#This script runs a command with retry logic, handling specific errors, ignoring others, and considering timeout settings to ensure robust execution.
<#
Define the conditions to handle
$conditions = @("FabricTransientException", "AnotherTransientException")

Define the conditions to ignore
$ignoreConditions = @("FabricElementNotFoundException", "AnotherElementNotFoundException")

Example: List of conditions/IgnoreConditions.
Execute-WithRetry -Command {
   # Removing application fabric:/AAA
   Remove-ServiceFabricApplication fabric:/AAA -ForceRemove -Force
} -Conditions $conditions -IgnoreConditions $ignoreConditions

Example: If no conditions are specified, it will handle FabricTransientException exceptions only.
Execute-WithRetry -Command { 
    # Removing application fabric:/BBB
    Remove-ServiceFabricApplication fabric:/BBB -ForceRemove -Force 
}

Example : If you want to handle all exceptions, then please override the -Conditions argument with an empty list.
Execute-WithRetry -Command { 
    # Removing application fabric:/CCC
    Remove-ServiceFabricApplication fabric:/CCC -ForceRemove -Force 
}  -Conditions @()

#>

# Define the maximum retry duration and the initial delay between retries
$maxRetryDuration = 60 # in seconds
$initialRetryDelay = 5 # in seconds 

# Function to execute a command with retry logic based on time and multiple conditions
function Execute-WithRetry {
    param (
        [scriptblock]$Command,       # The command to execute
        [string[]]$Conditions = @("FabricTransientException"), # The conditions to match exceptions against (optional)
        [string[]]$IgnoreConditions = @() # The conditions to ignore and return immediately (optional)
    )

    # Record the start time
    $startTime = Get-Date
    # Set the initial retry delay
    $retryDelay = $initialRetryDelay
    # Initialize the retry count
    $retryCount = 0

    # Loop until the maximum retry duration is exceeded
    while (((Get-Date) - $startTime).TotalSeconds -lt $maxRetryDuration) {
        try {
            # Log the command being executed
            Write-Host "Executing command: $Command"

            # Execute the command
            & $Command
            Write-Host "Command executed successfully."
            return
        } catch {
            $exceptionHandled = $false
            $retryCount++

            if ($IgnoreConditions.Count -gt 0) {
                # Check if the exception matches any of the ignore conditions
                foreach ($ignoreCondition in $IgnoreConditions) {
                    if ($_.Exception -match $ignoreCondition) {
                        Write-Host "Ignored exception: $($_.Exception.GetType().Name), returning immediately."
                        Write-Host "Exception message: $($_.Exception.Message)"
                        return
                    }
                }
            }

            if ($Conditions.Count -eq 0) {
                # If no conditions are specified, handle all exceptions
                Write-Host "Handled exception: $($_.Exception.GetType().Name), retrying... (Attempt: $retryCount)"
                Write-Host "Exception message: $($_.Exception.Message)"
                Start-Sleep -Seconds $retryDelay

                # Exponential backoff for retry delay
                $retryDelay *= 2
                $exceptionHandled = $true
            } else {
                # Check if the exception matches any of the specified conditions
                foreach ($condition in $Conditions) {
                    if ($_.Exception -match $condition) {
                        Write-Host "Handled exception: $($_.Exception.GetType().Name), retrying... (Attempt: $retryCount)"
                        Write-Host "Exception message: $($_.Exception.Message)"
                        Start-Sleep -Seconds $retryDelay

                        # Exponential backoff for retry delay
                        $retryDelay *= 2
                        $exceptionHandled = $true
                        break
                    }
                }
            }

            # If the exception was not handled, rethrow it and log it
            if (-not $exceptionHandled) {
                Write-Host "Unhandled exception: $($_.Exception.GetType().Name)"
                Write-Host "Exception message: $($_.Exception.Message)"
                throw $_
            }
        }
    }

    # If the maximum retry duration is exceeded, throw a timeout exception and log it
    Write-Host "Retry duration exceeded the maximum limit of $maxRetryDuration seconds."
    throw "Operation timed out."
}
