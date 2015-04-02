#About how to configure the remote kernel debugging

# VMWare Machine Configuration #

We will debug a virtual machine directly over a named pipe from the host operating system. But we can connect two virtual machines to the same named pipe by creating a virtual null-modem cable.

We need to setup the guest virtual machine to debug as follows:
  1. Powered off the virtual machine.
  1. Open VM then select Settings in VMware Workstation menu.
  1. In the VMware Machine Settings dialog box, click Add.
  1. In the Add Hardware Wizard dialog box, select Serial Port and click Next.
  1. On next page, select Output to named pipe and click Next.
  1. Set Name pipe to \\.\pipe\MinispyDbgPipe. Make sure you select **This end is the server** and **The other end is virtual machine**. Check Connect at power on then click Finish.
  1. After clicking Finish, select the newly created serial port and check Yield CPU on poll.


# Virtual Machine Configuration #

To enable kernel debugging, we need to change Windows boot parameters. Our virtual machine will also be configured to use serial debugging on COM2 at 115200 bps.

Windows Server 2003 uses a configuration file called boot.ini on the root of the system partition (generally the C:\ drive) to control how the operating system is booted and any startup options.

We need to change that file following that steps:

  1. Go to Start-Control Panel-System
  1. Select The Advanced Tab
  1. In the Advanced Tab Select the Settings("Startup And Recovery")
  1. In the Startup and Recovery window go to Edit button and copy the follow into the boot.ini file.
  1. Save and Restart the VMachine.

```
[boot loader]
timeout=30
default=multi(0)disk(0)rdisk(0)partition(1)\WINDOWS
[operating systems]
multi(0)disk(0)rdisk(0)partition(1)\WINDOWS="Windows Server 2003, Enterprise [DEBUG]" /fastdetect /NoExecute=OptOut /Debug /debugport=com2 /baudrate=115200
multi(0)disk(0)rdisk(0)partition(1)\WINDOWS="Windows Server 2003, Enterprise" /fastdetect /NoExecute=OptOut
```

# Attaching WinDBG Debugger #

These are the main steps to follow:

  1. The WinDbg command line gives you the ability to use environment variables in order to create workspaces that contain your custom debugging settings.

Inside the development machine (host or virtual machine), open a command prompt and type:

```
set _NT_EXECUTABLE_IMAGE_PATH= C:\WinDDK\7600.16385.0\src\filesys\miniFilter\minispy\filter\objchk_wnet_x86\i386
set _NT_SOURCE_PATH=C:\WinDDK\7600.16385.0\src\filesys\miniFilter\minispy\filter
```

To debug a virtual machine from your host(physical computer),open a command prompt and type:

```
"C:\WinDDK\7600.16385.0\Debuggers\windbg.exe" -b -k com:pipe,port=\\.\pipe\MinispyDbgPipe,resets=0,reconnect
Otherwise:

"C:\WinDDK\7600.16385.0\Debuggers\windbg.exe" -b -k com:port=com2,resets=0,reconnect
```

WinDbg GUI will popup and display Waiting to reconnect message.

  1. Start the target virtual machine.

  1. While booting, WinDbg will halt the target system. In the WinDbg command pane, set a breakpoint in DriverEntry routine as follows:

```
bp filter!DriverEntry
```

Press g key to start debug.