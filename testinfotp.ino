/*! \file *********************************************************************
 *
 * \brief
 *      Transmition de données entre USART0 et USART3 sur Arduino
 *
 * \par Documentation :
 *      https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
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

#define F_CPU 16000000UL // Fréquence du CPU
#define BAUD 9600 // Débit en bauds
#define MYUBRR F_CPU/16/BAUD-1 // Calcul de la valeur d'UBRR

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int flag3 = 0, flag0 = 0; // Variable pour indiquer la réception de données
volatile char donnee; // Variable pour stocker les données reçues

/*! \brief Initialisation de l'USART0
 *
 *  Configure l'USART0 avec la vitesse de transmission, le mode asynchrone,
 *  8 bits de données, 1 bit de stop.
 *
 *  \param ubrr Valeur du registre de baud rate pour la vitesse souhaitée.
 */
void USART0_Init(unsigned int ubrr) {
  /* Configuration du taux de bauds */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  // Activation du récepteur et du transmetteur, Activation de l'interruption de réception
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  /* Configuration du format de trame : 8 bits de données, 1 bit de stop */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/*! \brief Initialisation de l'USART3
 *
 *  Configure l'USART3 avec la vitesse de transmission, le mode asynchrone,
 *  8 bits de données, 1 bit de stop.
 *
 *  \param ubrr Valeur du registre de baud rate pour la vitesse souhaitée.
 */
void USART3_Init(unsigned int ubrr) {
  /* Configuration du taux de bauds */
  UBRR3H = (unsigned char)(ubrr >> 8);
  UBRR3L = (unsigned char)ubrr;
  // Activation du récepteur et du transmetteur, Activation de l'interruption de réception
  UCSR3B = (1 << RXEN3) | (1 << TXEN3) | (1 << RXCIE3);
  /* Configuration du format de trame : 8 bits de données, 1 bit de stop */
  UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);
}

/*! \brief Transmission via USART0
 *
 *  \param data Caractère à transmettre.
 */
void USART0_Transmit(unsigned char data) {
  /* Attente du buffer de transmission vide */
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /* Envoi des données dans le buffer */
  UDR0 = data;
}

/*! \brief Transmission via USART3
 *
 *  \param data Caractère à transmettre.
 */
void USART3_Transmit(unsigned char data) {
  /* Attente du buffer de transmission vide */
  while (!(UCSR3A & (1 << UDRE3)))
    ;
  /* Envoi des données dans le buffer */
  UDR3 = data;
}

/*! \brief Réception via USART0
 *
 *  \return Caractère reçu.
 */
unsigned char USART0_Receive(void) {
  /* Attente des données à recevoir */
  while (!(UCSR0A & (1 << RXC0)))
    ;
  /* Récupération et renvoi des données reçues */
  return UDR0;
}

/*! \brief Réception via USART3
 *
 *  \return Caractère reçu.
 */
unsigned char USART3_Receive(void) {
  /* Attente des données à recevoir */
  while (!(UCSR3A & (1 << RXC3)))
    ;
  /* Récupération et renvoi des données reçues */
  return UDR3;
}

/*! \brief Interruption de réception sur USART0
 *
 *  Cette interruption est déclenchée lorsqu'un caractère est reçu sur l'USART0.
 *  Elle indique la réception de données.
 */
ISR(USART0_RX_vect) {
  flag0 = 1;
}

/*! \brief Interruption de réception sur USART3
 *
 *  Cette interruption est déclenchée lorsqu'un caractère est reçu sur l'USART3.
 *  Elle indique la réception de données.
 */
ISR(USART3_RX_vect) {
  flag3 = 1;
}

/*! \brief Fonction principale
 *
 *  Fonction principale du programme.
 */
int main(void) {
  DDRB = 0b10000000; // Configuration de la broche PB7 en sortie
  USART0_Init(MYUBRR); // Initialisation de l'USART0
  USART3_Init(MYUBRR); // Initialisation de l'USART3
  sei(); // Activation des interruptions globales

  while (1) {
    if (flag3) {
      USART0_Transmit(UDR3); // Transmission des données reçues par USART3 via USART0
      flag3 = 0;
    }
    if (flag0) {
      USART3_Transmit(UDR0); // Transmission des données reçues par USART0 via USART3
      flag0 = 0;
    }
    _delay_ms(1); // Délai d'attente
  }
}
