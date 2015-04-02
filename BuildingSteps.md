Open Specific platform building console
> Home->Windows Driver Kits->Building->2003->x86 fre

for building minifilter samples go to:
> wdk\_install\_dir\src\filesys\minifilter\

type bcz -W to build

you could run the building command in the specific sample directory or in the parent dir, in that case it will compile all directories specified in **dirs** file


Output driver (**.sys) file will be located under the platform directory
> obj\_free\_winnt\_x86....  =>**

put the **.sys and**.inf in the same folder in your **test machine**

right click on **.inf and select install option//**should be done as administrator

in order to check if the minifilter driver was installed open a command terminal (cmd.exe) run driverquery tool, it should display your filter name. at the end if was recently installed, in its lexicographical order if you update the driver






