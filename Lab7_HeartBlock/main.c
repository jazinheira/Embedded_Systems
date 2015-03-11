// 0.Documentation Section 
// Lab7_HeartBlock, main.c

// Runs on LM4F120 or TM4C123 LaunchPad
// Input from PF4(SW1) is AS (atrial sensor), 
// Output to PF3, Green LED, is Ready,
// Output to PF1, Red LED, is VT (ventricular trigger) 
// Make PF4 input, PF3,PF1 output
// Initialize Ready to high and VT to low
// Repeat this sequence of operation over and over
// 1) Wait for AS to fall (touch SW1 switch)
// 2) Clear Ready low
// 3) Wait 10ms (debounces the switch)
// 4) Wait for AS to rise (release SW1)
// 5) Wait 250ms (simulates the time between atrial and ventricular contraction)
// 6) set VT high, which will pulse the ventricles 
// 7) Wait 250ms
// 8) clear VT low 
// 9) set Ready high

// Date: January 7, 2015

// 1. Pre-processor Directives Section
#include "TExaS.h"

// Constant declarations to access port registers using 
// symbolic names instead of addresses
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
	// Port F Address: 0x40025000
// Set bit-specific addresses for PF0-PF4
#define PF0											(*((volatile unsigned long *)0x40025004))
#define PF1											(*((volatile unsigned long *)0x40025008))
#define PF2											(*((volatile unsigned long *)0x40025010))
#define PF3											(*((volatile unsigned long *)0x40025020))
#define PF4											(*((volatile unsigned long *)0x40025040))
	
// 2. Declarations Section
//   Global Variables

//   Function Prototypes
void PortF_Init(void);
void Delay1ms(unsigned long msec);
void EnableInterrupts(void);  // Enable interrupts
void WaitForASLow(void);
void WaitForASHigh(void);
void SetVT(void);
void ClearVT(void);
void SetReady(void);
void ClearReady(void);



// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  TExaS_Init(SW_PIN_PF40, LED_PIN_PF31,ScopeOn);  // activate grader and set system clock to 80 MHz
  PortF_Init();                            // Init port PF4 PF3 PF1    
  EnableInterrupts();                      // enable interrupts for the grader  
  while(1){          // Follows the nine steps list above
    // a) Ready signal goes high
    // b) wait for switch to be pressed
    // c) Ready signal goes low
    // d) wait 10ms
    // e) wait for switch to be released
    // f) wait 250ms
    // g) VT signal goes high
    // h) wait 250ms
    // i) VT signal goes low

			//a)
		SetReady();
		
		//b)
		WaitForASLow();
		
		//c)
		ClearReady();
		
		//d)
		Delay1ms(10);
		
		//e)
		WaitForASHigh();
		
		//f
		Delay1ms(250);
		
		//g
		SetVT();
		
		//h
		Delay1ms(250);
		
		//i
		ClearVT();
		
  }
}
// Subroutine to initialize port F pins for input and output
// PF4 is input SW1 and PF3-1 is output LEDs
// Inputs: None
// Outputs: None
// Notes: ...
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;      // 1) F clock
  delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize     
  GPIO_PORTF_AMSEL_R &= 0x00;        // 2) disable analog function
  GPIO_PORTF_PCTL_R &= 0x00000000;   // 3) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R &= ~0x10;         // 4.1) PF4 input,
  GPIO_PORTF_DIR_R |= 0x0E;          // 4.2) PF3,2,1 output  
  GPIO_PORTF_AFSEL_R &= 0x00;        // 5) no alternate function
  GPIO_PORTF_PUR_R |= 0x10;          // 6) enable pullup resistor on PF4       
  GPIO_PORTF_DEN_R |= 0x1E;          // 7) enable digital pins PF4-PF1
	
	//GPIO_PORTF_DATA_R |= 0x04; //Have blue LED start
	GPIO_PORTF_DATA_R &= 0x00; //Start with LED dark.
}
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E


// Subroutine reads AS input and waits for signal to be low
// If AS is already low, it returns right away
// If AS is currently high, it will wait until it to go low
// Inputs:  None
// Outputs: None
void WaitForASLow(void){
// Wait for AS (PF4 - SW1) to be 0 [Switch pressed]
	int j = 0;
	while (j < 1) {
		if (PF4 == 0x00)
		{
			j = 1;
		}
	}
}

// Subroutine reads AS input and waits for signal to be low
// If AS is already low, it returns right away
// If AS is currently high, it will wait until it to go low
// Inputs:  None
// Outputs: None
void WaitForASHigh(void){
// Wait for AS (PF4 - SW1) to be 1 [Switch released]
	int j = 0;
	while (j < 1) {
		if (PF4 == 0x10)
		{
			j = 1;
		}
	}
}

// Subroutine sets VT high
// Inputs:  None
// Outputs: None
// Notes:   friendly means it does not affect other bits in the port
void SetVT(void){
// Set VT - PF1 (Red LED) On (1)
	GPIO_PORTF_DATA_R |= 0x02;
}

// Subroutine clears VT low
// Inputs:  None
// Outputs: None
// Notes:   friendly means it does not affect other bits in the port
void ClearVT(void){
// Clear VT - PF1 (Red LED) Off (0)
	GPIO_PORTF_DATA_R &= ~0x02;
}

// Subroutine sets Ready high
// Inputs:  None
// Outputs: None
// Notes:   friendly means it does not affect other bits in the port
void SetReady(void){
// Set Ready - PF3 (Green LED) On (1)
	GPIO_PORTF_DATA_R |= 0x08;
}


// Subroutine clears Ready low
// Inputs:  None
// Outputs: None
// Notes:   friendly means it does not affect other bits in the port
void ClearReady(void){
// Cears Ready - PF3 (Green LED) Off (0)
	GPIO_PORTF_DATA_R &= ~0x08;
}

// Subroutine to delay in units of milliseconds
// Inputs:  Number of milliseconds to delay
// Outputs: None
// Notes:   assumes 80 MHz clock
void Delay1ms(unsigned long msec){
  while(msec > 0){
    unsigned long i = 13333.33;  // should finish in 1ms. 1333333 takes 100ms to complete.
    while(i > 0){
      i = i - 1;
    }
    msec = msec - 1; // decrements every 1 ms
  }
}

