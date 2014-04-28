#ifndef eeprom_h
#define eeprom_h

void eeprom_init(const char *grblrc_filename);
char eeprom_get_char(unsigned int addr);
void eeprom_put_char( unsigned int addr, unsigned char new_value );
void memcpy_to_eeprom_with_checksum(unsigned int destination, char *source, unsigned int size);
int memcpy_from_eeprom_with_checksum(char *destination, unsigned int source, unsigned int size);

#endif
