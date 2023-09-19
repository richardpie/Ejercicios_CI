

#include <xc.h>
#define _XTAL_FREQ 8000000  

#include <string.h>
#include "config.h"
#include "GLCD.h"
#define PINA0 PORTAbits.RA0





unsigned char pulse = 0;

unsigned char status = 0;
void writeScreen(unsigned char x, unsigned char y, char* str){
      unsigned char i, size = strlen(str);
      for(i = 0; i < size; ++i){
	 char aux = str[i];
	 putchGLCD(x,y,aux);
	 y += 0x01;
      }
}


void high_priority interrupt high_prio(void)
{
   if (TMR0IF && TMR0ON && status == 1){
      INTCONbits.TMR0IF = 0;
      ++status;
      
   }
   else if(TMR0IF && TMR0ON && status == 2){
      INTCONbits.TMR0IF = 0;
      status = 4;
      
   }
   TMR0 = 59911;
}


	    




void configStatus(){
   
   
      if (status == 0){
	 writeScreen(0x04,0x08,"START      ");
	 TMR0 = 49911;
      }
      else if(status == 1){
	 writeScreen(0x04,0x08,"READY!     ");
	 
	 TMR0ON = 1;
	 
      }
      else if (status == 2){
	 writeScreen(0x04,0x08,"SHOOT!     ");

	
      }
       else if (status == 3){
	 TMR0ON = 0;
	 writeScreen(0x04,0x08,"GOOD!      ");
	 status = 0;
	 
	 
	  __delay_ms(1000);
	  writeScreen(0x04,0x08,"YOU WIN!");
	  __delay_ms(1000);
      }
       else if (status == 4){
	 TMR0ON = 0;
	 writeScreen(0x04,0x08,"LATE       ");
	 status = 0;
	
	 __delay_ms(1000);
	  
	  writeScreen(0x04,0x08,"YOU LOSE!");
	  __delay_ms(1000);
      }
      else if (status == 5){
	 TMR0ON = 0;
	 writeScreen(0x04,0x08,"EARLY!     ");
	 __delay_ms(1000);
	 status = 0;
	 writeScreen(0x04,0x08,"YOU LOSE!");
	  __delay_ms(1000);
	
      }
      
}


void InitPIC() {
   // Port config.
   
   ANSELD=0;      //PORTD configurat com Digital
   ANSELB=0;	//PORTB configurat com Digital
    
   ANSELA=0;
   
   TRISD=0x00;    //Configurem D i B de sortida
   TRISB=0x00;
   TRISA=0xFF;
   GLCDinit();		   //Inicialitzem la pantalla
   clearGLCD(0,7,0,127);      //Esborrem pantalla
   setStartLine(0);           //Definim linia d'inici

   // Configuracio del TMR 0
   T0CONbits.T0CS = 0;    // clock select intern
   T0CONbits.T08BIT = 0;  // 16 bits counter
   INTCONbits.TMR0IF = 0; // clear Interrupt flag 
   INTCONbits.TMR0IE = 1; // int enabled
   TMR0H = 0;             // clear TMR 0 (first, write to High Byte => read section 11.3 on datasheet)
   TMR0L = 0;             // clear TMR 0 (then, write to Low Byte => read section 11.3 on datasheet)
   T0CONbits.PSA = 0;
  
   T0CONbits.T0PS = 0b111;//*
   
   T0CONbits.TMR0ON = 0;  // timer on
   TMR0 = 49911;
   // General int. configuration
   INTCONbits.GIEH = 1;  //Enabling High priority interrupts
   
}


void main(void)
{
   InitPIC();
   
   while(1){
      
      if (PINA0 == 1 && pulse == 0 && status == 0) {
	    pulse = 1;
	    ++status;
	    
	 } 
	 
      
      
      else if (pulse == 1 && PINA0 == 0) {
	    pulse = 0;
      }
	 
      else if(PINA0 == 1 && pulse == 0 && status == 1){
	 
	 pulse = 1;
	 status = 5;
      }
      else if(PINA0 == 1 && pulse == 0 && status == 2){
	   
	 ++status;
	 pulse = 1;
      }
      configStatus();
      
       
      
    }
   
}