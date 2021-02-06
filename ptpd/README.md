# My PTPD 
My program helps to use ptpd easyly. The objective is to syncronize clocks of two computers in the same local network.

## Dependencies

* First of all, install `ptpd`
    ```
    $ sudo apt update
    $ sudo apt install ptpd -y
    ```
* Secondly, mmake connection between both computers

## Executing
* Compile the .ccp file
    ```
    $ g++ -o MyPtpd MyPtpd.cpp
    ```
* Run MyPtpd (First in Master and then in a Slave)
    ```
    $ ./MyPtpd
    ```
> I recommend `Masterblackconfig.ini` as config file for Master and `Slavegreenconfig.ini` as config file for Slave.