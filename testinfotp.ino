/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Transmition d'un code ASCII à l'aide d'Arduino
 *

 *
 * \par Note :  Fichier à partir de la Datasheet
 *
 * \par Documentation : https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
 * 
 * \author
 *      Github: https://github.com/Totom08 \n
 *      email: thomas.davenne@etudiant.univ-reims.fr
 *
 * $Name: LiaisonSerie_RELEASE_1_0 $
 * $Revision: 1.8 $
 * $RCSfile: testinfotp.c,v $
 * $Date: 2023/11/22 11:02:21 $  \n
 * 
 */

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
int flag=0;

/*! \brief Fonction Initialisation
 */
void USART_Init(unsigned int ubrr)
{
/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/*Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

/*! \brief Fonction Transmition
 */
void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;
/* Put data into buffer, sends the data */
UDR0 = data;
}

/*! \brief Fonction Recevoir
 */
unsigned char USART_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)))
;
/* Get and return received data from buffer */
return UDR0;
}

/*! \brief Interruption
 */
ISR(USART_RX_vect) //https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
{
  flag=1;
}

/*! \brief Fonction Main
 */
int main(void)
{

USART_Init(MYUBRR);
sei();
while(1){
/*USART_Transmit('D');
USART_Transmit('o');
USART_Transmit('r');
USART_Transmit('i');
USART_Transmit('a');
USART_Transmit('n');
USART_Transmit(' ');
USART_Transmit('C');
USART_Transmit('H');
USART_Transmit('A');
USART_Transmit('U');
USART_Transmit('V');
USART_Transmit('E');
USART_Transmit('T');
USART_Transmit('\n');
_delay_ms(1000);*/
  if (flag==1){
    USART_Transmit(UDR0);
    flag=0;
  }
  _delay_ms(1);
}


}