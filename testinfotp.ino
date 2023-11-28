/*! \file *********************************************************************
 *
 * \brief
 *      Transmition d'un code ASCII à l'aide d'Arduino
 *
 * \par Note :  Fichier à partir de la Datasheet
 *
 * \par Documentation : https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
 *
 * \author
 *      Github: https://github.com/Totom08 \n
 *      email: thomas.davenne@etudiant.univ-reims.fr
 *
 * $Name: LiaisonSerie_RELEASE_2_0 $
 * $Revision: 1.8 $
 * $RCSfile: testinfotp.c,v $
 * $Date: 2023/11/28 12:10:21 $  \n
 * 
 */

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define FRAMING_ERROR (1<<FE0)
#define PARITY_ERROR (1<<UPE0)
#define DATA_OVERRUN (1<<DOR0)

int flag=0;

/*! \brief Fonction d'initialisation de l'USART
 *
 *  Configure l'USART avec la vitesse de transmission, le mode asynchrone,
 *  8 bits de données, 2 bits d'arrêt, pas de parité.
 *
 *  \param ubrr Valeur du registre de baud rate pour la vitesse souhaitée.
 */
void USART_Init(unsigned int ubrr)
{
    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /*Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
    /* Set frame format: 8 data bits, 2 stop bits */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

/*! \brief Fonction de transmission de données via l'USART
 *
 *  \param data Caractère à transmettre.
 */
void USART_Transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)))
        ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

/*! \brief Interruption de réception de l'USART
 *
 *  Cette interruption est déclenchée lorsqu'un caractère est reçu sur l'USART.
 *  Elle détecte les erreurs de communication (framing, parity, overrun) et
 *  transmet le caractère ou un message d'erreur en conséquence.
 */
ISR(USART_RX_vect)
{
    char status=UCSR0A;
    char byteReceive = UDR0;

    // Vérifier s'il n'y a pas d'erreur de communication
    if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0){
        // Transmettre le caractère reçu
        USART_Transmit(byteReceive);
    }
    else{
        // S'il y a une erreur, transmettre le message d'erreur
        USART_putsln("!error");
    }
}

/*! \brief Fonction pour l'envoi d'une chaîne de caractères via l'USART
 *
 *  \param str Pointeur vers la chaîne de caractères à transmettre.
 */
void USART_puts(unsigned char *str) {
    // Envoyer chaque caractère de la chaîne jusqu'à la fin
    while (*str != 0) {
        USART_Transmit(*str);
        str++;
    }
}

/*! \brief Fonction pour l'envoi d'une chaîne de caractères suivie d'un retour à la ligne via l'USART
 *
 *  \param str Pointeur vers la chaîne de caractères à transmettre.
 */
void USART_putsln(unsigned char *str) {
    // Utiliser la fonction USART_puts pour envoyer la chaîne
    USART_puts(str);

    // Ajouter un retour à la ligne ('\n') et un retour chariot ('\r') à la fin
    USART_Transmit('\n');
    USART_Transmit('\r');
}

/*! \brief Fonction principale
 *
 *  Fonction principale du programme.
 */
int main(void)
{
    USART_Init(MYUBRR);
    _delay_ms(1000); // Attendre un peu après l'initialisation
    sei();
    while(1){
        _delay_ms(1);
    }
}
