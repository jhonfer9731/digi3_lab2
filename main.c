#include <hidef.h>
#include "derivative.h"



#define MODULO_TOPE_TIMER 30000 // it defines the ceiling of the timer, then the count is reset

volatile unsigned char flag_inputCapture = 0; //variable for indicating the main program that the Input Capture interrupt rutine was attended

unsigned int time_edge_1, time_edge_2; //variables for indicating the current timer count when falling and rising edge occurs

unsigned long time_pulse_width; // time of bottle going across the sensor (this time measures indirectly the bottle diameter)


unsigned int overflow_Count = 0; // indicates the number of timer overflows when the bottle goes across the sensor



void Timer1_init(void){
    /* initialisation of timer status and control register 
       and SC_channel register
    */

   // clock initialisation

	TPM1SC_CLKSB=0;
	TPM1SC_CLKSA=0;
   TPM1SC_TOIE = 1; //timer overflow input enable (enables the output interrupt request)
   TPM1SC_CPWMS = 0; // disabled the edge-aligned PWM

   TPM1SC_PS = 5; // configure the pre-scale

   TPM1MOD = MODULO_TOPE_TIMER; // configure the module register of timer (16bits)


    /* **** Configuration of timer channels **** 0 */
   
    TPM1C0SC_CH0IE = 1; // interrupt enable

    // select (Input Capture) Timer Mode
    TPM1C0SC_MS0A = 0;
    TPM1C0SC_MS0B = 0;

    // capture on rising and falling edge
    TPM1C0SC_ELS0A = 1;
    TPM1C0SC_ELS0B = 1; // edge level selector
    
    
    /* Configuration of timer channel 1 */
    /* manages the on/off button */
    
    TPM1C1SC_CH1IE = 1; // start/stop pin
    
	// select (Input Capture) Timer Mode
	TPM1C1SC_MS1A = 0;
	TPM1C1SC_MS1B = 0;
	// capture on rising and falling edge
	TPM1C1SC_ELS1A = 1;
	TPM1C1SC_ELS1B = 1;
	
	
	/* Configuration of timer channel 2 */
	    /* manages the Borra_cuentas button */
	    
	    TPM1C2SC_CH2IE = 1; // start/pin
	    
		// select (Input Capture) Timer Mode
		TPM1C2SC_MS2A = 0;
		TPM1C2SC_MS2B = 0;
		// capture on rising edge only
		TPM1C2SC_ELS2A = 1;
		TPM1C2SC_ELS2B = 0;
	
	/* Configuration of timer channel 3 */
		/* manages the Cuenta_Standard button */
		
		TPM1C3SC_CH3IE = 1; // Cuenta_Standard
		
		// select (Input Capture) Timer Mode
		TPM1C3SC_MS3A = 0;
		TPM1C3SC_MS3B = 0;
		// capture on rising edge only
		TPM1C3SC_ELS3A = 1;
		TPM1C3SC_ELS3B = 0;
		
	/* Configuration of timer channel 4 */
		/* manages the Cuenta_stand_mas button */
		
		TPM1C4SC_CH4IE = 1; // Cuenta_stand_mas
		
		// select (Input Capture) Timer Mode
		TPM1C4SC_MS4A = 0;
		TPM1C4SC_MS4B = 0;
		// capture on rising edge only
		TPM1C4SC_ELS4A = 1;
		TPM1C4SC_ELS4B = 0;
	/* Configuration of timer channel 5 */
		/* manages the Cuenta_stand_menos button */
		
		TPM1C5SC_CH5IE = 1; // Cuenta_stand_menos
		
		// select (Input Capture) Timer Mode
		TPM1C5SC_MS5A = 0;
		TPM1C5SC_MS5B = 0;
		// capture on rising edge only
		TPM1C5SC_ELS5A = 1;
		TPM1C5SC_ELS5B = 0;
	/* Configuration of timer channel 6 */
		/* manages the Average_Defectuosas button */
		
		TPM2C0SC_CH0IE = 1; // Average_Defectuosas
		
		// select (Input Capture) Timer Mode
		TPM2C0SC_MS0A = 0;
		TPM2C0SC_MS0B = 0;
		// capture on rising edge only
		TPM2C0SC_ELS0A = 1;
		TPM2C0SC_ELS0B = 0;

    

    //activate the bus clock A
    TPM1SC_CLKSA = 1;
    TPM1SC_CLKSB=0;// BUS RATE CLOCK
}


void Port_Init( void ){
    /* Ports configuration */
	
	
	
    PTADD = 0xFF; // output port
    PTAD = 0x00; // clear port
    
    /*  PTAD[2] boton start/stop */

}


interrupt 9 void TPM1InputCapture_ISR(void){

    /* attends the interrupt request of timer when 
    there is a falling or rising edge

    It happens when the bottle pass through the sensor
    */
   // clear the interrupt request of CHOF (channel 0 flag)
   
   TPM1C0SC;//1 step, read the ch0 SC register
   TPM1C0SC_CH0F = 0; // 2 step, clear CH0F
   flag_inputCapture = 1; // indicates the main program the input capture

}

interrupt 10 void TPM1OnOFFButton_ISR(void){
	/* attends the interrupt request of timer when 
	there is a falling or rising edge

	It happens when the on/off button is pressed
	*/
	// clear the interrupt request of CHOF (channel 1 flag)
	TPM1C1SC;//1 step, read the ch1 SC register
	TPM1C1SC_CH1F = 0; // 2 step, clear CH1F
	flag_inputCapture = 2;// indicates the main program the input capture with On/off button
	
}

interrupt 11 void borrarCuentas_ISR(void){
	/* attends the interrupt request of timer when 
		there is a  rising edge

		It happens when the Borra_cuentas button is pressed
		*/
		// clear the interrupt request of CHOF (channel 1 flag)
		TPM1C2SC;//1 step, read the ch1 SC register
		TPM1C2SC_CH2F = 0; // 2 step, clear CH1F
		flag_inputCapture = 3;// indicates the main program the input capture with Borra_Cuentas
		
}


interrupt 12 void cuentaEstandar_ISR(void){
	/* attends the interrupt request of timer when 
		there is a  rising edge

		It happens when the cuentaEstandar button is pressed
		*/
		// clear the interrupt request of CHOF (channel 1 flag)
		TPM1C3SC;//1 step, read the ch1 SC register
		TPM1C3SC_CH3F = 0; // 2 step, clear CH1F
		flag_inputCapture = 4;// indicates the main program the input capture with Borra_Cuentas
		
}
interrupt 13 void CuentaEstandarMas_ISR(void){
	/* attends the interrupt request of timer when 
		there is a  rising edge

		It happens when the CuentaEstandarMas button is pressed
		*/
		// clear the interrupt request of CHOF (channel 1 flag)
		TPM1C4SC;//1 step, read the ch1 SC register
		TPM1C4SC_CH4F = 0; // 2 step, clear CH1F
		flag_inputCapture = 5;// indicates the main program the input capture with Borra_Cuentas	
}

interrupt 14 void CuentaEstandarMenos_ISR(void){
	/* attends the interrupt request of timer when 
		there is a  rising edge

		It happens when the CuentaEstandarMenos button is pressed
		*/
		// clear the interrupt request of CHOF (channel 1 flag)
		TPM1C5SC;//1 step, read the ch1 SC register
		TPM1C5SC_CH5F = 0; // 2 step, clear CH1F
		flag_inputCapture = 6;// indicates the main program the input capture with Borra_Cuentas	
}

interrupt 16 void averageDefectuosas_ISR(void){
	/* attends the interrupt request of timer when 
		there is a  rising edge

		It happens when the Average_Defectuosas button is pressed
		*/
		// clear the interrupt request of CHOF (channel 1 flag)
		TPM2C0SC;//1 step, read the ch1 SC register
		TPM2C0SC_CH0F = 0; // 2 step, clear CH1F
		flag_inputCapture = 7;// indicates the main program the input capture with Borra_Cuentas	
}






interrupt 15 void TPM1_Overflow_ISR(void){

    /* attends the interrupt request of timer when 
    there is an overflow in the counter
    
    it occurs when the count reach the module value
    
     */
    //Clear TOF in two steps
    TPM1SC;
    TPM1SC_TOF = 0;
    overflow_Count++; // increase overflows counter

}

void main(void){

    char firstEdge_flag = 0; // indicates the fist and second edge of the IC interrupt
    unsigned char movimiento = 0; // initial state of  conveyor belt
    unsigned int numeroBotellas = 0;
    // system initialisation
    SOPT1_COPT  = 0; // disable the WatchDog
    Port_Init(); 
    Timer1_init();

    //Enable interrupts
    EnableInterrupts;


    for(;;){

        __asm WAIT;// an interruption wake the MCU up
        

        //check if the interrupt is an Input Capture
        if(flag_inputCapture == 1 || movimiento){
            flag_inputCapture = 0;
            // verify if it is first or second edge
            if(firstEdge_flag == 0){
                overflow_Count = 0; // reset the overflow counter
                firstEdge_flag = 1;
                time_edge_1 = TPM1C0V; // save the current count of the timer
                PTAD_PTAD0 = 1; // the bottle just got captured by sensor
                PTAD_PTAD1 = 0;
            }
            else if(firstEdge_flag == 1){

                firstEdge_flag = 0; 
                time_edge_2 = TPM1C0V; // save the current count of the timer
                PTAD_PTAD0 = 0;
                PTAD_PTAD1 = 1; // the bottle just left the sensor
                time_pulse_width = (long) (time_edge_2 + overflow_Count*MODULO_TOPE_TIMER - time_edge_1); // calculates the time the bottle takes to go across the sensor
                overflow_Count = 0; // reset the overflow counter
                numeroBotellas++; // increases the bottle counter

            }
        }
        else if(flag_inputCapture == 2){
        	// on/off button was pressed
        	flag_inputCapture = 0;
        	if(!movimiento){
        		// on state
				PTAD_PTAD2 = 1; // PTAD[2] activates the conveyor belt
				movimiento = 1;
        	}else{
        		// off state
				PTAD_PTAD2 = 0; // PTAD[2] stops the conveyor belt
				movimiento = 0;
        	}
        	
        	
        }
        
    }



}





