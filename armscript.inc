ERROR=ABORT // Abort if error occurs when processing the script
WAIT=ON // Wait for each command to finish

GO

STATS

  //STDIOLOG OFF // Close the log file

  //UNLOAD 1 // Unload the image
  //DELFILE 1 // Remove the symbol definitions
    //DISCONNECT  // Disconnect from the target
  //WAIT=OFF
QUIT Y
