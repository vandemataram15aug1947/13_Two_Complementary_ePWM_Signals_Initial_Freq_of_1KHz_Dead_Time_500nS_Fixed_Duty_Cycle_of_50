
/////////////////////////////// Task-13  ////////////////////////////////////////////////////////////

Generate two PWM signals with initial frequency of 1KHz having dead time of 500nSec and fixed duty cycle of 50%, such that
1. When switch 1 is pressed the frequency should increase by 10khz
2. When switch 2 is pressed then frequency should increase by 1khz
3. When switch 3 is pressed then frequency should decrease by 10khz
3. When switch 4 is pressed then frequency should decrease by 1khz
Two PWM Signals should be complementary
 */


#include "F28x_Project.h"
#include "F2837xD_device.h"
#include "F2837xD_Examples.h"
#include "driverlib.h"
#include "device.h"

//
// Function Prototypes
//
void initEPWM1(void);
void initEPWM2(void);
void PinMux_init();
__interrupt void xINT1_isr(void);
__interrupt void xINT2_isr(void);
__interrupt void xINT3_isr(void);
__interrupt void xINT4_isr(void);
volatile float PWM_Freq= 1000;
int period =0;


void main(void)
{
    // Initialize device clock and peripherals

       Device_init();


       // Disable pin locks and enable internal pull-ups.

       Device_initGPIO();


       // Initialize PIE and clear PIE registers. Disables CPU interrupts.

       Interrupt_initModule();


       // Initialize the PIE vector table with pointers to the shell Interrupt
       // Service Routines (ISR).

       Interrupt_initVectorTable();


       //////////////  Vande  ///////////////////

       // This is needed to write to EALLOW protected registers
       EALLOW;
       // Configure XINT1 in pie Vector Table
       PieVectTable.XINT1_INT = &xINT1_isr;
       // Configure XINT2 in pie Vector Table
       PieVectTable.XINT2_INT = &xINT2_isr;
       // Configure XINT3 in pie Vector Table
       PieVectTable.XINT3_INT = &xINT3_isr;
       // Configure XINT4 in pie Vector Table
       PieVectTable.XINT4_INT = &xINT4_isr;
       EDIS;




       // Enable CPU INT1 which is connected to CPU-Timer 0
       IER |= M_INT1; // M- means to Enable
       IER |= M_INT12; // M- means to Enable

       //////////////  Vande  ///////////////////

       // Enable  ,XINT1,XINT2,XINT3 ,XINT4    in the PIE: Group 1 & 12 __interrupt 4,5,1,2

       // Enable the PIE block
       PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
       // Enable PIE Group 1 INT4
       PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
       // Enable PIE Group 1 INT5
       PieCtrlRegs.PIEIER1.bit.INTx5 = 1;
       // Enable PIE Group 12 INT1
       PieCtrlRegs.PIEIER12.bit.INTx1 = 1;
       // Enable PIE Group 12 INT2
       PieCtrlRegs.PIEIER12.bit.INTx2 = 1;

       //////////////  Vande  ///////////////////

       //Configure GPIO67,GPIO22,GPIO11,GPIO14 FOR xINT1,xINT2xINT3,xINT4
       GPIO_SetupXINT1Gpio(67);
       GPIO_SetupXINT2Gpio(22);
       GPIO_SetupXINT3Gpio(11);
       GPIO_SetupXINT4Gpio(14);

       //////////////  Vande  ///////////////////

       // Check switch state for falling edge
       XintRegs.XINT1CR.bit.POLARITY = 1;          // Rising edge interrupt
       XintRegs.XINT2CR.bit.POLARITY = 1;          // Rising edge interrupt
       XintRegs.XINT3CR.bit.POLARITY = 1;          // Rising edge interrupt
       XintRegs.XINT4CR.bit.POLARITY = 1;          // Rising edge interrupt

       // Calling Function of Pin Mux
       PinMux_init();


       // Initialize ePWM1 and ePWM2
       //
       initEPWM1();
       initEPWM2();

       //////////////  Vande  ///////////////////

       //Disable xINT1,xINT2,xINT3,xINT4
       XintRegs.XINT1CR.bit.ENABLE = 0;            // Disable XINT1
       XintRegs.XINT2CR.bit.ENABLE = 0;            // Disable XINT2
       XintRegs.XINT3CR.bit.ENABLE = 0;            // Disable XINT3
       XintRegs.XINT4CR.bit.ENABLE = 0;            // Disable XINT4

       // Enable Global Interrupt and Real time Interrupt
       EINT;
       ERTM;



       while(1){

           //////////////  Vande  ///////////////////

           // Enable XINT1
           XintRegs.XINT1CR.bit.ENABLE = 1;
           // Enable XINT2
           XintRegs.XINT2CR.bit.ENABLE = 1;
           // Enable XINT1
           XintRegs.XINT3CR.bit.ENABLE = 1;
           // Enable XINT2
           XintRegs.XINT4CR.bit.ENABLE = 1;
           // Wait for CPU to wake up when Interrupt is Trigger
           asm("IDLE:");
       }
   }


//
// initEPWM1 - Configure ePWM1
//
void initEPWM1()
{

    //////////////  Vande  ///////////////////

    // 1k frequency
        EPwm1Regs.TBPRD = 500000;
        // Phase is 0
        EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;
        EPwm1Regs.TBCTR = 0x0000;
       // Set Compare values   : 50% OF DUTY CYCLE
        EPwm1Regs.CMPA.bit.CMPA = 25000 ;

        //////////////  END  ///////////////////

      // Setup counter mode   :  Count up and down
       EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
       // Clock ratio to SYSCLKOUT   :  High speed pre-scalar clock division
       EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
       EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

      // Acction qualifier Register
       EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;  // Compare UP
       EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;   // Compare  DOWN
}
void initEPWM2()
{
    //////////////  Vande  ///////////////////

    // 1k frequency
        EPwm2Regs.TBPRD = 50000;
        // Phase is 0
        EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;
        EPwm2Regs.TBCTR = 0x0000;
       // Set Compare values   : 50% duty cycle
        EPwm2Regs.CMPA.bit.CMPA = 25000 ;

        ////////////// END  ///////////////////

      // Setup counter mode  :  Count up and down
       EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
       // Clock ratio to SYSCLKOUT   : Highspeed pre-scalar clock division
       EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
       EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

   //   Action Qualifier register
       EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;
       EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;

       //////////////  Vande  ///////////////////

       // Dead band Control registers
       EPwm2Regs.DBCTL.bit.OUT_MODE = 3; // out mode
       EPwm2Regs.DBCTL.bit.IN_MODE = 0;  // IN mode
       EPwm2Regs.DBCTL.bit.POLSEL = 2;

       EPwm2Regs.DBRED.all = 50; // Rising Edge
       EPwm2Regs.DBFED.all = 50; // Falling Edge

       ////////////// END  ///////////////////
           EALLOW;
           CpuSysRegs.PCLKCR0.bit.TBCLKSYNC=1;
           EDIS;
}

void PinMux_init()
{
    //////////////  Vande  ///////////////////
    EALLOW;
    //EPWM1 -> myEPWM1 Pinmux
    // gpio 15 to gpio 0  general purpouse I/O
    GpioCtrlRegs.GPAMUX1.all=0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0=1;// epwm active
    GpioCtrlRegs.GPAMUX1.bit.GPIO2=1;// epwm active
    EDIS;

}
//////////////  Vande  ///////////////////

__interrupt void xINT1_isr(void)
{
    XintRegs.XINT1CR.bit.ENABLE = 0; // Disable XINT3
    int debounce_count = 10;
    while(debounce_count > 0)
    {
        // Wait for a short delay
        DELAY_US(10000);
        // Check the state of GPIO 11
        if(GpioDataRegs.GPCDAT.bit.GPIO67 == 1)
        {
            // Ignore the edge if the input is still high
            debounce_count = 10;
        }
        else
        {
            // Decrement the debounce counter
            debounce_count--;
        }
    }
    // Increase PWM Frequency by 10KHz
    PWM_Freq +=10000;
    if(PWM_Freq >=200000)
    {
        PWM_Freq=200000;
    }
    // Update Period
    period=(100000000)/(2*(PWM_Freq));
    EPwm1Regs.TBPRD = period ;
    // Update Duty Cycle for EPwm1
    EPwm1Regs.CMPA.bit.CMPA = (period/2) ;
    EPwm2Regs.TBPRD = period ;
    // Update Duty Cycle for EPwm2
    EPwm2Regs.CMPA.bit.CMPA = (period/2) ;
    // clear interrupt flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

//////////////  Vande  ///////////////////

__interrupt void xINT2_isr(void)
{
    // Disable XINT2
    XintRegs.XINT2CR.bit.ENABLE = 0;
    int debounce_count = 10;
    while(debounce_count > 0)
    {
        // Wait for a short delay
        DELAY_US(10000);
        // Check the state of GPIO 22
        if(GpioDataRegs.GPADAT.bit.GPIO22 == 1)
        {
            // Ignore the edge if the input is still high
            debounce_count = 10;
        }
        else
        {
            // Decrement the debounce counter
            debounce_count--;
        }
    }
    // Increase PWM frequency b  1KHz
    PWM_Freq +=1000;
    if(PWM_Freq >=200000)
    {
        PWM_Freq=200000;
    }
    period=(100000000)/(2*(PWM_Freq));
    EPwm1Regs.TBPRD = period ;
    // Update Duty Cycle for EPwm1
    EPwm1Regs.CMPA.bit.CMPA = (period/2) ;
    EPwm2Regs.TBPRD = period ;
    // Update Duty Cycle for EPwm2
    EPwm2Regs.CMPA.bit.CMPA = (period/2) ;
    // clear interrupt flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

//////////////  Vande  ///////////////////

__interrupt void xINT3_isr(void)
{
    // Disable XINT3
    XintRegs.XINT3CR.bit.ENABLE = 0;
    int debounce_count = 10;
    while(debounce_count > 0)
    {
        // Wait for a short delay
        DELAY_US(10000);
        // Check the state of GPIO 11
        if(GpioDataRegs.GPADAT.bit.GPIO11 == 1)
        {
            // Ignore the edge if the input is still high
            debounce_count = 10;
        }
        else
        {
            // Decrement the debounce counter
            debounce_count--;
        }
    }

    PWM_Freq -=10000;
    if(PWM_Freq <=1000)
    {
        PWM_Freq=1000;
    }
    period=(100000000)/(2*(PWM_Freq));
    EPwm1Regs.TBPRD = period ;
    // Update Duty Cycle for EPwm1
    EPwm1Regs.CMPA.bit.CMPA = (period/2) ;
    EPwm2Regs.TBPRD = period ;
    // Update Duty Cycle for EPwm2
    EPwm2Regs.CMPA.bit.CMPA = (period/2) ;
    // clear interrupt flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

//////////////  Vande  ///////////////////

__interrupt void xINT4_isr(void)
{
    // Disable XINT4
    XintRegs.XINT4CR.bit.ENABLE = 0;
    int debounce_count = 10;
    while(debounce_count > 0)
    {
        // Wait for a short delay
        DELAY_US(10000);
        // Check the state of GPIO 14
        if(GpioDataRegs.GPADAT.bit.GPIO14 == 1)
        {
            // Ignore the edge if the input is still high
            debounce_count = 10;
        }
        else
        {
            // Decrement the debounce counter
            debounce_count--;
        }
    }

    PWM_Freq -=1000;
    if(PWM_Freq <=1000)
    {
        PWM_Freq=1000;
    }
    period=(100000000)/(2*(PWM_Freq));
    EPwm1Regs.TBPRD = period ;
    // Update Duty Cycle for EPwm1
    EPwm1Regs.CMPA.bit.CMPA = (period/2) ;
    EPwm2Regs.TBPRD = period ;
    // Update Duty Cycle for EPwm2
    EPwm2Regs.CMPA.bit.CMPA = (period/2) ;
    // clear interrupt flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}
