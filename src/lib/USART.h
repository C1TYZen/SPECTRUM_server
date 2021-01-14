/***********************
 * USART
 ***********************/
void USART_init();

void USART_flush();
uint8_t USART_read();
uint16_t USART_get();
uint8_t USART_readnow();
uint16_t USART_getmessage();
void USART_readln(char* str);

void USART_write(uint8_t data);
void USART_send(uint16_t data);
void USART_println(char* string);
void USART_print(char* string);
void USART_putbyteview(uint8_t data);