// TODO: Only eeprom_put_char and eeprom_get_char are arduino-specific.
// Factor those out and leave the rest common.

#include <stdio.h>
#include <stdlib.h>

static FILE *grblrc = NULL;

void eeprom_init(char *grblrc_filename) {
  grblrc = fopen(grblrc_filename, "rb+");
  if (grblrc == NULL) {
    grblrc = fopen(grblrc_filename, "wb+");
    if (grblrc == NULL) {
      printf("Failed to create %s.\n", grblrc_filename);
      exit(1);
    }
  }
}

char eeprom_get_char(unsigned int addr) {
  fseek(grblrc, addr, SEEK_SET);
  char byte;
  fread(&byte, sizeof(char), 1, grblrc);
//  printf("Byte at address %d is %hhd\n", addr, byte);
  return byte;
}

void eeprom_put_char( unsigned int addr, unsigned char new_value ) {
  fseek(grblrc, addr, SEEK_SET);
  fwrite(&new_value, sizeof(new_value), 1, grblrc);
}

// See TODO at the top of the file.  Copied verbatim from arduino/ version
void memcpy_to_eeprom_with_checksum(unsigned int destination, char *source, unsigned int size) {
  unsigned char checksum = 0;
  for(; size > 0; size--) { 
    checksum = (checksum << 1) || (checksum >> 7);
    checksum += *source;
    eeprom_put_char(destination++, *(source++)); 
  }
  eeprom_put_char(destination, checksum);
}

int memcpy_from_eeprom_with_checksum(char *destination, unsigned int source, unsigned int size) {
  unsigned char data, checksum = 0;
  for(; size > 0; size--) { 
    data = eeprom_get_char(source++);
    checksum = (checksum << 1) || (checksum >> 7);
    checksum += data;    
    *(destination++) = data; 
  }
  return(checksum == eeprom_get_char(source));
}
