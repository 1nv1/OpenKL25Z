Introduction
============

!!! Warning !!!
This project is under hard construction. If well is possible to use it, I do not recommend for any without advance experience with Linux/ARM/GCC.

The goal of this project is make a clean and single framework to learn about microcontroller Cortex-M0. 
The project is based on [bare-metal ARM](https://github.com/payne92/bare-metal-arm). In this moment I work actively in add the funcionality that I want. This is a "bare metal" runtime for the 
[Freescale Freedom FRDM-KL25Z](http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-KL25Z) 
ARM development board ($13). It builds with the GCC ARM toolchain, with no other external dependencies.

All about project can be found on [OpenKL25Z](http://1nv1.github.io/OpenKL25Z/).

Quick start on Linux or Mac OS X:
* Clone the repo: `git clone https://github.com/1nv1/OpenKL25Z`
* Grab and unpack GCC ARM toolchain:
  * On Ubuntu: `sudo apt-get install gcc-arm-none-eabi`
  * On Mac & Linux: `cd bare-metal-arm; make gcc-arm`
* `make`

This will create a `main.srec` image file to flash onto the development board.  (If you're using
the standard bootloader, plug the SDA USB port to a host computer.  On Linux, type `make deploy`.  On other systems,
copy the .SREC file to the FRDM-KL25Z volume.)  

If everything is working, the RGB LEB will flash a few times and then be steady green.  You can access the USB 
SDA serial port (at 115,200 baud) and see the conversion of ADC of input 8 in single mode.

References:
* [Freescale Freedom FRDM-KL25Z](http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-KL25Z)
  * Where to buy: [Newark](http://www.newark.com/jsp/search/productdetail.jsp?SKU=28W5033&CMP=KNC-GPLA&mckv=|pcrid|27090073701|plid|),
[Digi-Key](http://www.digikey.com/product-detail/en/FRDM-KL25Z/FRDM-KL25Z-ND/3529594?WT.mc_id=PLA_3529594)
* [KL25 Sub-Family Reference Manual](http://cache.freescale.com/files/32bit/doc/ref_manual/KL25P80M48SF0RM.pdf)
* [GCC ARM toolchain](https://launchpad.net/gcc-arm-embedded)
* [Newlib C library](http://sourceware.org/newlib/)

Why do this?
------------

I think this is the better way to learn about a some subject is when apply the
knowledge in some practical case.

Walkthrough
-----------

The interrupt vectors and reset code are in `_startup.c`.  The CPU comes out of reset in `_reset_init()` which:
* Copies initialized constant values from flash ROM to RAM
* Configures the main clock (48Mhz)
* Jumps to `_start()` in the Newlib C library

After the C library is done initializing, it invokes `main()` (implemented in `main.c`).
Also I promote the use of protothreads with this «framework». 

Status
------
Warning! This project is in alpha stage.

ToDo
----

In this case:
* Better documentation about `Makefile`.
* Better documentation about linker script.
* Clean file for create new modules support.

History
-------

*(02/12/2015)
    *A new repository was created and it is called OpenKL25Z.
    *Now the main.c has a experimental example using state-machines and 
    protothreads.

*(01/05/2015)
    *The structure has been improved on experimental branch (at old repo, inspired for CIAA's firmware).

*0.0.1 (08/26/2014)
    *ADC full support.
    *It adds big quantity of documentation with doxygen.
    *Better performance of size using newlib-nano in compilation.


* 0.0.0 (07/26/2014)
    * Warning of readme about the goals and status of project.
    * For the "ADC" support  I use CooCox development files as base. When I get the
idea of how it works, is more "cheaper" if you already get the APIs constants. Also 
the license of them are "open/free" style.
    
Contact
-------
For "bare metal" framework:

[andy@payne.org](mailto:andy@payne.org)

[blog.payne.org](http://blog.payne.org)

For this project:

[nelson.lombardo@gmail.com](mailto:nelson.lombardo@gmail.com)

[NelsonLombardo.tk](http://nelsonlombardo.tk/)
