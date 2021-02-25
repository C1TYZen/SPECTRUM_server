/***********************
 * USART
 ***********************/
void USART_init();

void USART_flush();
uint8_t USART_read();
uint16_t USART_get();
uint8_t USART_readnow();
uint16_t USART_getmessage();
void USART_scanln(char*, size_t);

void USART_write(uint8_t);
void USART_send(uint16_t);
void USART_println(char*);
void USART_print(char*);
void USART_putbyteview(uint8_t);