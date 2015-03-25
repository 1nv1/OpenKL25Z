Introduction
============

!!! Warning !!!
This project is under hard construction. If well is possible to use it, I do not recommend for any without advance experience with Linux/ARM/GCC.

The goal of this project is make a single framework to learn about microcontroller Cortex-M0+. 
The project is based on [bare-metal ARM](https://github.com/payne92/bare-metal-arm). In this moment I work actively in add the funcionality that I want. This is a "bare metal" runtime for the 
[Freescale Freedom FRDM-KL25Z](http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-KL25Z) 
ARM development board ($12.95 at ). It builds with the GCC ARM toolchain, with no other external dependencies.

Quick start on Linux:

+ Clone the repo: `git clone https://github.com/1nv1/OpenKL25Z`
+ Grab and unpack GCC ARM toolchain:
    + On Ubuntu/Debian/Fedora: `sudo apt-get install gcc-arm-none-eabi` also you can use `$ make gcc-arm` to download it 
+ Important! For use the board under Linux do you need update the OpenSDA [firmware](http://www.pemicro.com/blog/index.cfm?post_id=11)
+ Mount the `FRDM-KL25Z` in your system 
+ For compile the example and burn it into the board, execute `$ make burn` (On other systems copy the .SREC file to the FRDM-KL25Z volume.)
+ If everything is working, the RGB LEB will flash very quickly
+ Now you can interact with the program using `screen` command: `$ screen /dev/ttyACM0 115200`
+ Using the capacitive touch you can navigate for the menu and select items. The part of capacitive sensor near of QR code in the board is for navigate, the rest is for select action

Status
------
Warning! This project is in alpha stage.

    
Contact
-------

For more information write me [nelson.lombardo@gmail.com](mailto:nelson.lombardo@gmail.com) or visit the [website](http://1nv1.github.io/OpenKL25Z/).
