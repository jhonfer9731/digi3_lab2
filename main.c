#include <hidef.h>
#include "derivative.h"


#define MODULO_TOPE_TIMER 30000 // it defines the ceiling of the timer, then the count is reset

volatile unsigned char flag_inputCapture = 0; //variable for indicating the main program that the Input Capture interrupt rutine was attended

unsigned int time_edge_1, time_edge_2; //variables for indicating the current timer count when falling and rising edge occurs

unsigned long time_pulse_width; // time of bottle going across the sensor (this time measures indirectly the bottle diameter)


volatile unsigned int overflow_Count = 0; // indicates the number of timer overflows when the bottle goes across the sensor



void Timer1_init(void){
    /* initialisation of timer status and control register 
       and SC_channel register
    */

   // clock initialisation ( bus rate 8MHz )

	TPM1SC_CLKSB=0;
	TPM1SC_CLKSA=0;
	TPM1SC_TOIE = 1; //timer overflow input enable (enables the output interrupt request)
	TPM1SC_CPWMS = 0; // disabled the edge-aligned PWM
	
	TPM1SC_PS = 6; // configure the pre-scale
	
	TPM1MOD = MODULO_TOPE_TIMER; // configure the module register of timer (16bits)
	
	
	TPM2SC_TOIE =1; //timer overflow input enable (enables the output interrupt request)
	TPM2SC_PS = 7; // configure the pre-scale
	TPM2SC_CPWMS = 0;
	TPM2MOD = 0;
   
   
   
   
   
   
   
   
    /* **** Configuration of timer channels ****  */
   
   /* Configuration of timer channel 0 */
   /* in charge of the bottle sensor */
   
    TPM1C0SC_CH0IE = 1; // interrupt enable

    // select (Input Capture) Timer Mode
    TPM1C0SC_MS0A = 0;
    TPM1C0SC_MS0B = 0;

    // capture on rising and falling edge
    TPM1C0SC_ELS0A = 1;
    TPM1C0SC_ELS0B = 1; // edge level selector
    
    
    /* Configuration of timer channel 1 */
    /* manages the on/off button  (switch) */
    
    TPM1C1SC_CH1IE = 1; // start/stop pin
    
	// select (Input Capture) Timer Mode
	TPM1C1SC_MS1A = 0;
	TPM1C1SC_MS1B = 0;
	// capture on falling edge
	TPM1C1SC_ELS1A = 0;
	TPM1C1SC_ELS1B = 1;
	
	
	/* Configuration of timer channel 2 */
	/* manages the Borra_cuentas button */
	
	TPM1C2SC_CH2IE = 1; // start/pin
	
	// select (Input Capture) Timer Mode
	TPM1C2SC_MS2A = 0;
	TPM1C2SC_MS2B = 0;
	// capture on falling edge only
	TPM1C2SC_ELS2A = 0;
	TPM1C2SC_ELS2B = 1;
	
	/* Configuration of timer channel 3 */
	/* manages the Cuenta_Standard button */
	
	TPM1C3SC_CH3IE = 1; // Cuenta_Standard
	
	// select (Input Capture) Timer Mode
	TPM1C3SC_MS3A = 0;
	TPM1C3SC_MS3B = 0;
	// capture on falling edge only
	TPM1C3SC_ELS3A = 0;
	TPM1C3SC_ELS3B = 1;
		
	/* Configuration of timer channel 4 */
	/* manages the Cuenta_stand_mas button */
	
	TPM1C4SC_CH4IE = 1; // Cuenta_stand_mas
	
	// select (Input Capture) Timer Mode
	TPM1C4SC_MS4A = 0;
	TPM1C4SC_MS4B = 0;
	// capture on rising edge only
	TPM1C4SC_ELS4A = 0;
	TPM1C4SC_ELS4B = 1;
	
	
	/* Configuration of timer channel 5 */
	/* manages the Cuenta_stand_menos button */
	
	TPM1C5SC_CH5IE = 1; // Cuenta_stand_menos
	
	// select (Input Capture) Timer Mode
	TPM1C5SC_MS5A = 0;
	TPM1C5SC_MS5B = 0;
	// capture on falling edge only
	TPM1C5SC_ELS5A = 0;
	TPM1C5SC_ELS5B = 1;
	
	
	/* Configuration of timer channel 6 */
	/* manages the Average_Defectuosas button */
	
	TPM2C0SC_CH0IE = 1; // Average_Defectuosas
	
	// select (Input Capture) Timer Mode
	TPM2C0SC_MS0A = 0;
	TPM2C0SC_MS0B = 0;
	// capture on falling edge only
	TPM2C0SC_ELS0A = 0;
	TPM2C0SC_ELS0B = 1;
	
	

    /*** activate the bus clocks ****/
	
    TPM1SC_CLKSA = 1;
    TPM1SC_CLKSB = 0;// BUS RATE CLOCK
    
    
    TPM2SC_CLKSA = 1;
    TPM2SC_CLKSB = 0;
}


void Port_Init( void ){
    /* Ports configuration */
	
	
	
    PTADD = 0xFF; // output port
    PTAD = 0x00; // clear port (actuators)
    //PTEDD = 0x00;
    //PTED = 0x00;
    //PTFDD = 0x00;
    //PTFD = 0x00;
    
    /*  PTAD[2] boton start/stop */

}


interrupt 9 void TPM1InputCapture_ISR(void){

    /* attends the interrupt request of timer when 
    there is a falling or rising edge

    It happens when the bottle pass through the sensor
    */
   // clear the interrupt request of CHOF (channel 0 flag)
   
   TPM1C0SC; //1 step, read the ch0 SC register
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


void asignarMensaje(char *mensaje,char *nuevoMensaje){
	
	unsigned char contador = 0;
	while(nuevoMensaje[contador] != '\0'){
		mensaje[contador] = nuevoMensaje[contador];
		contador++;
	}
	
}

void asignarNumero(unsigned int A, char* mensaje){
	
	mensaje[2] = (A/10)+48;
	mensaje[3] = (A%10)+48;
		
}




void main(void){

    char firstEdge_flag = 0; // indicates the fist and second edge of the IC interrupt
    unsigned char movimiento = 0; // initial state of  conveyor belt
    unsigned int numeroBotellas = 0;
    unsigned char defecto = 0; // flag for wrong bottles
	unsigned int numeroBotellasStd = 0;
    unsigned int cuentaStandardMas = 0;
    unsigned int cuentaStandardMenos = 0;
    unsigned int averageDefectuosas  = 0;
    unsigned int botellasDefectuosas  = 0;
    unsigned int variableSeleccion = 0;  // it has the number of one bottle category
    const unsigned int min = 30000; // Min & Max to define the size range for Standard bottles.
	const unsigned int max = 50000; 
	unsigned int tmax = 0; //Counter for the 10 seconds
	
	char mensaje[65]= "  00   Numero de Botellas Std"; 
	
	
    // system initialisation
    SOPT1_COPT  = 0; // disable the WatchDog
    Timer1_init();
    Port_Init(); 

    //Enable interrupts
    EnableInterrupts;
   
    
    /**** Assigned input ports **/
    /*
     * 
     * 
    TImer 1: 
    
    PTED[2] = ch0 -> t1,t2 rising/fallin edge
    PTED[3] = ch1 -> on/off btn
    PTFD[0] = ch2 -> erase counts
    PTFD[1] = ch3 -> # standard bottles
    PTFD[2] = ch4 -> # stardard +
    PTFD[3] = ch5 -> # stardard -
    
    Timer 2:
    PTFD[4] = ch0 -> #deficient bottles average
    
     */


    for(;;){

        __asm WAIT;// an interruption wake the MCU up
        

        //check if the interrupt is an Input Capture
        if(flag_inputCapture == 1){
            flag_inputCapture = 0;
            // verify if it is first or second edge
            if(firstEdge_flag == 0 && movimiento){
                overflow_Count = 0; // reset the overflow counter
                firstEdge_flag = 1; 
                time_edge_1 = TPM1C0V; // save the current count of the timer
                PTAD_PTAD0 = 1; // the bottle just got captured by sensor (LED)
                PTAD_PTAD1 = 0;
            }
            else if(firstEdge_flag == 1 && movimiento){

                firstEdge_flag = 0; 
                time_edge_2 = TPM1C0V; // save the current count of the timer
                PTAD_PTAD0 = 0;
                PTAD_PTAD1 = 1; // the bottle just left the sensor (LED)
                time_pulse_width = (long) (time_edge_2 + overflow_Count*MODULO_TOPE_TIMER - time_edge_1); // calculates the time the bottle takes to go across the sensor
                if (time_pulse_width>=min && time_pulse_width<=max){
					numeroBotellasStd++;
					defecto=0;
					asignarMensaje(mensaje,"      Numero de botellas estandar");
					asignarNumero(numeroBotellasStd,mensaje);
					
				}
				else if(max<time_pulse_width){
					cuentaStandardMas++;
					botellasDefectuosas++;
					defecto=1;
				}
				else if(time_pulse_width<min){
					cuentaStandardMenos++;
					botellasDefectuosas++;
					defecto=1;
				}
				else{
					/* it never goes into */
					botellasDefectuosas++;
					defecto=1;
				}
				PTAD_PTAD4 = defecto; /* LED is active */ //PTAD[4] indicates the non-standard status
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
				PTAD_PTAD0 = 0;
				PTAD_PTAD1 = 0; // the bottle just left the sensor (LED)
				defecto = 0;
				PTAD_PTAD4 = 0;
        	}
        	PTAD_PTAD3 = movimiento; //PTAD[3] indicates the movement status
        	
        	
        }
        // it indicates that Borrar_cuentas button was pressed
        else if(flag_inputCapture == 3){
        	flag_inputCapture = 0;
        	if(!movimiento){ 
        		numeroBotellas = 0; // the number of bottles is reset
        	}
        }
        //the cuentaEstandar button is pressed
        else if(flag_inputCapture == 4){
        	flag_inputCapture = 0;
        	variableSeleccion = numeroBotellasStd;
        	asignarMensaje(mensaje,"      Numero de botellas estandar");
			asignarNumero(numeroBotellasStd,mensaje);
        }
        //It happens when the CuentaEstandarMas button is pressed
        else if(flag_inputCapture == 5){
			flag_inputCapture = 0;
			variableSeleccion = cuentaStandardMas;
			asignarMensaje(mensaje,"      Numero de botellas estandar mas");
			asignarNumero(variableSeleccion,mensaje);
        }
        //It happens when the CuentaEstandarMenos button is pressed
        else if(flag_inputCapture == 6){
			flag_inputCapture = 0;
			variableSeleccion = cuentaStandardMenos;
			asignarMensaje(mensaje,"      Numero de botellas estandar menos");
			asignarNumero(variableSeleccion,mensaje);
        }
        //It happens when the Average_Defectuosas button is pressed
        else if(flag_inputCapture == 7){
			flag_inputCapture = 0;
			if(numeroBotellas){
				variableSeleccion = (botellasDefectuosas*100)/numeroBotellas;
			}else{
				variableSeleccion = 0;
			}
			asignarMensaje(mensaje,"    %  Porcentaje botellas defectuosas");
			asignarNumero(variableSeleccion,mensaje);
        }
        else if(overflow_Count > 2){ // Enter when 
        	overflow_Count = 0;
        	PTAD_PTAD2 = 0; // PTAD[2] stops the conveyor belt
        	movimiento = 0;
        	PTAD_PTAD3 = movimiento; //PTAD[3] indicates the movement status
        	PTAD_PTAD0 = 0;
			PTAD_PTAD1 = 0;
			defecto = 0;
			PTAD_PTAD4 = 0;
			asignarMensaje(mensaje,"Tiempo de espera maximo alcanzado, el sistema se apagara ....");
        }
    }



}





