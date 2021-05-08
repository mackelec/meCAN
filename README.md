# meCAN
CAN (Dual)  bus Arduino library for stm32F103/105/107, stm32L4, stm32F4


### Acknoledgements

This library is very much based on EXOCAN.  The author of EXOCAN has indicated he has no interest in furthering his library and I needed both Dual CAN and able to run on the extra microcontrollers as above.

###  Status

This is a work in progress at the moment.  

The library is working on the F103 and dual CAN on the F105/107. Single CAN on the stm32L4.
I don't expect it will take much (except making the time) to have dual CAN working on the stm32F4.

Regarding Dual CAN, I have the interupt working on CAN1 but not on CAN2.  This not a problem for me.  In time I see no reason not to get CAN2 interupt working.
