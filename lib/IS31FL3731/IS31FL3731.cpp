
// IS31FL3731 library code
#include <IS31FL3731.h>


/// @brief Constructor for IS31FL3731 LED matrix driver.
/// @param address I2C address of the chip.
IS31FL3731::IS31FL3731() {

    // _i2c_address = ( address & 0b01111111 );

}





/// @brief Set the i2c address of the chip.
/// @param _i2c_address The i2c address of the chip.
void IS31FL3731::i2caddressset( uint8_t new_i2c_address ) {

    _i2c_address = ( new_i2c_address & 0b01111111 );
    
}

/// @brief  Returns the current i2c address as a uint8_t
uint8_t IS31FL3731::i2caddressget() {

    return _i2c_address;

}




/// @brief Write a byte to the chip
/// @param page The page to write to, 0x0-7 animation, 0xB control.
/// @param address The address within the page to write to.
/// @param data The data to write
void IS31FL3731::write( uint8_t page , uint8_t address , uint8_t data ) {

    // say hello to the chip
    wire.beginTransmission( _i2c_address );

    // send the page
    wire.write( 0xFD );
    wire.write( page );

    wire.endTransmission();

    wire.beginTransmission( _i2c_address );

    // send the address
    wire.write( address );

    // send the data
    wire.write( data );

    // say goodbye
    wire.endTransmission();

    // all done, return to caller
    return;

}






/// @brief Read a byte from the chip
/// @param page The page to read from.  0x0-7 animation, 0xB control.
uint8_t IS31FL3731::read( uint8_t page , uint8_t address ) { 
    
    // say hello to the chip
    wire.beginTransmission( _i2c_address );

    // send the page
    wire.write( 0xFD );
    wire.write( page );

    wire.endTransmission();

    wire.beginTransmission( _i2c_address );
    
    // send the address
    wire.write( address );

    // say goodye to the chip
    wire.endTransmission();

    // request one byte back from the chip
    wire.requestFrom( _i2c_address , (uint8_t)(1) );

    while( !wire.available() ) { delay(1); };
    // receive one byte back from the chip
    return wire.read();

    // return said byte.
    //return 0;
    
}






/// @brief Sets the software shutdown state to 0 ( shutdown ) or 1 ( normal operation ). 
/// @param state 0 or 1. 0 is shutdown, 1 is normal operation.
void IS31FL3731::shutdownset( uint8_t state ) {

    // write the data to the chip
    write( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_SOFTWARESHUTDOWN , state );

}







/// @brief Retreives the current software shutdown state as a uint8_t.
uint8_t IS31FL3731::shutdownget() {

    return read( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_SOFTWARESHUTDOWN );

}





/// @brief Set the mode in the configuration register.
/// @param mode 0b00 = picture mode, 0b01 = auto frame play, 0b1x = audio frame play.
void IS31FL3731::modeset( uint8_t mode ) {

    // read out the current config register byte
    uint8_t tempbyte = read( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REGISTER );

    // add in my data
    tempbyte &= 0b11100111;
    tempbyte |= ( mode << 3 );
    
    // write back the completed register byte
    write( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REGISTER , tempbyte );

    // and return to caller!
    return;

}

/// @brief Gets the current mode from the chip.
uint8_t IS31FL3731::modeget() {

    uint8_t tempbyte = read( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REGISTER );

    tempbyte &= 0b00011000;

    return ( tempbyte >> 3 );

}








/// @brief Set the current frame number to display.
/// @param frame the number of the frame to display.  0-7.
void IS31FL3731::frameset( uint8_t frame ) {
    write( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_PICTURE_DISPLAY_REG , frame );
}

/// @brief returns the current frame number from the chip.
uint8_t IS31FL3731::frameget() {
    return read( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_PICTURE_DISPLAY_REG );
}




















