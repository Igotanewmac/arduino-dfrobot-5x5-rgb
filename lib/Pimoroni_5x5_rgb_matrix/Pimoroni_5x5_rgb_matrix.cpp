
#include <Arduino.h>

#include <Wire.h>
#define wire Wire

#include <Pimoroni_5x5_rgb_matrix.h>










// define a mapping table

unsigned char mappingtable[ 3 ][ 5 ][ 5 ][ 2 ] = {
                                                    {
                                                        // red
                                                        {
                                                            // row 0
                                                            {  14 , 6 } , { 14 , 5 } , { 14 , 4 } , { 14 , 3 } , { 14 , 2 }
                                                        },
                                                        {
                                                            // row 1
                                                            {  16 , 4 } , { 16 , 5 } , { 16 , 6 } , { 14 , 0 } , { 14 , 1 }
                                                        },
                                                        {
                                                            // row 2
                                                            {  16, 3 } , { 16 , 2 } , { 16 , 1 } , { 16 , 0 } , { 15 , 7 }
                                                        },
                                                        {
                                                            // row 3
                                                            {  15 , 5 } , { 15 , 4 } , { 15 , 3 } , { 15 , 2 } , { 15 , 1 }
                                                        },
                                                        {
                                                            // row 4
                                                            {  15 , 6 } , { 1 , 7 } , { 1 , 0 } , { 1 , 1 } , { 1 , 2 }
                                                        }
                                                    } ,
                                                    {
                                                        // green
                                                        {
                                                            // row 0
                                                            {  8 , 5 } , { 8 , 4 } , { 10 , 4 } , { 10 , 3 } , { 10 , 2 }
                                                        },
                                                        {
                                                            // row 1
                                                            {  2 , 3 } , { 2 , 4 } , { 2 , 5 } , { 10 , 0 } , { 10 , 1 }
                                                        },
                                                        {
                                                            // row 2
                                                            {  2 , 2 } , { 2 , 1 } , { 4 , 1 } , { 4 , 0 } , { 5 , 7 }
                                                        },
                                                        {
                                                            // row 3
                                                            {  3 , 4 } , { 3 , 3 } , { 3 , 2 } , { 3 , 1 } , { 5 , 1 }
                                                        },
                                                        {
                                                            // row 4
                                                            {  3 , 5 } , { 11 , 7 } , { 11 , 1 } , { 11 , 2 } , { 11 , 3 }
                                                        }
                                                    } ,
                                                    {
                                                        // blue
                                                        {
                                                            // row 0
                                                            {  10 , 5 } , { 12 , 5 } , { 12 , 4 } , { 12 , 3 } , { 12 , 2 }
                                                        },
                                                        {
                                                            // row 1
                                                            {  4 , 3 } , { 4 , 4 } , { 4 , 5 } , { 12 , 0 } , { 12 , 1 }
                                                        },
                                                        {
                                                            // row 2
                                                            {  4 , 2 } , { 6 , 2 } , { 6 , 1 } , { 6 , 0 } , { 7 , 7 }
                                                        },
                                                        {
                                                            // row 3
                                                            {  5 , 4 } , { 5 , 3 } , { 5 , 2 } , { 7 , 2 } , { 7 , 1 }
                                                        },
                                                        {
                                                            // row 4
                                                            {  5 , 5 } , { 13 , 7 } , { 13 , 1 } , { 13 , 2 } , { 13 , 3 }
                                                        }
                                                    }
};





















Pimoroni_5x5_rgb_matrix::Pimoroni_5x5_rgb_matrix( uint8_t new_i2c_address ) {

    _i2c_address = new_i2c_address;

}

Pimoroni_5x5_rgb_matrix::~Pimoroni_5x5_rgb_matrix() {

}










/// @brief Set the i2c address and perform any setup required.
/// @param new_i2c_address The i2c address of the chip.
void Pimoroni_5x5_rgb_matrix::begin( uint8_t new_i2c_address = 0x75 ) {

    // make sure the wire library is started.
    wire.begin();

    // store my i2c address for later.
    _i2c_address = new_i2c_address;

    // turn off the chip
    softwareShutdownSet( 0 );
    
    // set to picture display mode
    displayModeSet( 0x00 );

    // set the frame pointer to zero
    frameDisplayPointerSet( 0x00 );

    // clear the buffers
    //pixelStateBufferClear();
    //pixelpwmStateBufferClear();
    //pixelBufferClearAll();

    // now write them out
    //pixelBufferWriteAllToFrame( 0x00 );

    // now turn the chip back on.
    softwareShutdownSet( 1 );
    
    // all done, return to caller.
    return;

}




















/// @brief Switch to a different frame, if necessary.
/// @param framenumber The frame number to switch to.
void Pimoroni_5x5_rgb_matrix::_switchFrame( uint8_t framenumber ) {

    // check if we need to switch at all?
    if ( framenumber == _currentframe ) { return; }

    // ok, we need to switch now, perform an i2c transaction.
    wire.beginTransmission( _i2c_address );
    wire.write( 0xFD );
    wire.write( framenumber );
    wire.endTransmission();

    // now update our current frame number
    _currentframe = framenumber;

    // all done, return to caller.
    return;
    
}








/// @brief Write a single byte of data to the chip.
/// @param framenumber The number of the frame to write to. 0x00-0x07 Animation. 0x0B Control.
/// @param address The address within the frame to write to.
/// @param data The data byte to write to the chip.
void Pimoroni_5x5_rgb_matrix::_chipwritebyte( uint8_t framenumber , uint8_t address , uint8_t data ) {


    _switchFrame( framenumber );

    // say hello to the chip again...
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


/// @brief Read a single byte of data from the chip.
/// @param framenumber The number of the frame to read from. 0x00-0x07 Animation. 0x0B Control.
/// @param address The address within the frame to read from.
/// @return The data byte rturned from the chip as a uint8_t.
uint8_t Pimoroni_5x5_rgb_matrix::_chipreadbyte( uint8_t framenumber , uint8_t address ) {

    _switchFrame( framenumber );

    // say hello to the chip again...
    wire.beginTransmission( _i2c_address );
    
    // send the address
    wire.write( address );

    // say goodye to the chip
    wire.endTransmission();

    // request one byte back from the chip
    wire.requestFrom( _i2c_address , (uint8_t)(0x01) );

    // wait until that byte is available.
    while( !wire.available() ) { delay(1); };

    // receive one byte back from the chip and return it as a uint8_t.
    return (uint8_t)( wire.read() );

}









































/// @brief Set a pixels power state to on or off.
/// @param xpos The x position, with 0 at the left.
/// @param ypos The y position, with 0 at the top.
/// @param colour The colour bank to set. 0 = red, 1 = blue , 2 = green.
/// @param state  The on-off state of the pixel. 0 = off, 1 = on.
void Pimoroni_5x5_rgb_matrix::pixelSet( uint8_t xpos , uint8_t ypos , uint8_t colour , uint8_t state ) {

    // set the pixel state
    if ( state ) {
        // if we are turning on...
        _ledstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ] |= ( 0b1 << mappingtable[ colour ][ xpos ][ ypos][ 1 ] );
    }
    else {
        // if we are turning off...
        _ledstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ] &= ~( 0b1 << mappingtable[ colour ][ xpos ][ ypos][ 1 ] );
    }

    // all done, return to caller.
    return;

}







/// @brief Get a pixels power state.
/// @param xpos The x position, with 0 on the left.
/// @param ypos The y position, with 0 at the top.
/// @param colour The colour bank to check. 0 = red, 1 = blue , 2 = green.
/// @return The on-off state of the pixel, as a uint8_t. 0 = off, 1 = on.
uint8_t Pimoroni_5x5_rgb_matrix::pixelGet( uint8_t xpos , uint8_t ypos , uint8_t colour ) {

    // just return the right bit.
    return ( ( _ledstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ] & ( 0b1 << mappingtable[ colour ][ xpos ][ ypos][ 1 ] ) ) >> mappingtable[ colour ][ xpos ][ ypos][ 1 ] );

}













/// @brief Clear the pixel state buffer to all zero.
void Pimoroni_5x5_rgb_matrix::pixelStateBufferClear() {
    // hard clear the pixel buffer
    for ( uint8_t i = 0 ; i < 18 ; i++ ) {
        _ledstate[ i ] = 0;
    }

    // all done, return to caller.
    return;
}

/// @brief Fills the pixel buffer with a given byte.
/// @param statebyte The byte to use for fill.  LSB is leftmost pixel. order XXX43210.
void Pimoroni_5x5_rgb_matrix::pixelStateBufferFill( uint8_t statebyte ) {
    // hard clear the pixel buffer

    // this is the big slow loop part!
    for ( uint8_t colour = 0; colour < 3 ; colour++ ) {
        for ( uint8_t xpos = 0 ; xpos < 5 ; xpos++ ) {
            for ( uint8_t ypos = 0 ; ypos < 5 ; ypos++ ) {
                pixelSet( xpos , ypos , colour , ( ( statebyte & ( 0b1 << ypos ) ) >> ypos ) );
            }
        }
    }

    // all done, return to caller.
    return;
}








/// @brief Set a pixels blink state to on or off.
/// @param xpos The x position, with 0 at the left.
/// @param ypos The y position, with 0 at the top.
/// @param colour The colour bank to set. 0 = red, 1 = blue , 2 = green.
/// @param state  The on-off blink state of the pixel. 0 = off, 1 = on.
void Pimoroni_5x5_rgb_matrix::pixelBlinkSet( uint8_t xpos , uint8_t ypos , uint8_t colour , uint8_t state ) {
    // set the pixel blink state
    if ( state ) {
        // if we are turning on...
        _ledblinkstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ] |= ( 0b1 << mappingtable[ colour ][ xpos ][ ypos][ 1 ] );
    }
    else {
        // if we are turning off...
        _ledblinkstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ] &= ~( 0b1 << mappingtable[ colour ][ xpos ][ ypos][ 1 ] );
    }

    // all done, return to caller.
    return;
}

/// @brief Get a pixels blink state.
/// @param xpos The x position, with 0 on the left.
/// @param ypos The y position, with 0 at the top.
/// @param colour The colour bank to check. 0 = red, 1 = blue , 2 = green.
/// @return The on-off blink state of the pixel, as a uint8_t. 0 = off, 1 = on.
uint8_t Pimoroni_5x5_rgb_matrix::pixelBlinkGet( uint8_t xpos , uint8_t ypos , uint8_t colour ) {

    // just return the right bit.
    return ( ( _ledblinkstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ] & ( 0b1 << mappingtable[ colour ][ xpos ][ ypos][ 1 ] ) ) >> mappingtable[ colour ][ xpos ][ ypos][ 1 ] );

}

/// @brief Clear the pixel blink state buffer to all zero.
void Pimoroni_5x5_rgb_matrix::pixelBlinkStateBufferClear() {
    // hard clear the pixel buffer
    for ( uint8_t i = 0 ; i < 18 ; i++ ) {
        _ledblinkstate[ i ] = 0x00;
    }

    // all done, return to caller.
    return;
}

/// @brief Fills the pixel blink state buffer with a given byte.
/// @param statebyte The byte to use for fill.  LSB is leftmost pixel. order XXX43210.
void Pimoroni_5x5_rgb_matrix::pixelBlinkStateBufferFill( uint8_t statebyte ) {
    // hard clear the pixel buffer

    // this is the big slow loop part!
    for ( uint8_t colour = 0; colour < 3 ; colour++ ) {
        for ( uint8_t xpos = 0 ; xpos < 5 ; xpos++ ) {
            for ( uint8_t ypos = 0 ; ypos < 5 ; ypos++ ) {
                pixelBlinkSet( xpos , ypos , colour , ( ( statebyte & ( 0b1 << ypos ) ) >> ypos ) );
            }
        }
    }

    // all done, return to caller.
    return;
}




















/// @brief Set a pixels pwm value.
/// @param xpos The x position, with 0 at the left.
/// @param ypos The y position, with 0 at the top.
/// @param colour The colour bank to set. 0 = red, 1 = blue , 2 = green.
/// @param state  The pwm value of the pixel. 0x00-0xFF.
void Pimoroni_5x5_rgb_matrix::pixelpwmStateSet( uint8_t xpos , uint8_t ypos , uint8_t colour , uint8_t state ) {
    _ledpwmstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ][ mappingtable[ colour ][ xpos ][ ypos ][ 1 ] ] = state;
    return;
}

/// @brief Get a pixels pwm state.
/// @param xpos The x position, with 0 on the left.
/// @param ypos The y position, with 0 at the top.
/// @param colour The colour bank to check. 0 = red, 1 = blue , 2 = green.
/// @return The pwm value of the pixel, as a uint8_t. 0-255.
uint8_t Pimoroni_5x5_rgb_matrix::pixelpwmStateGet( uint8_t xpos , uint8_t ypos , uint8_t colour ) {
    return _ledpwmstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ][ mappingtable[ colour ][ xpos ][ ypos ][ 1 ] ];
}



/// @brief Clear the pixel pwm buffer to all zero.
void Pimoroni_5x5_rgb_matrix::pixelpwmStateBufferClear() {
    for ( uint8_t colour = 0 ; colour < 3 ; colour++ ) {
        for ( uint8_t xpos = 0 ; xpos < 5 ; xpos++ ) {
            for ( uint8_t ypos = 0 ; ypos < 5 ; ypos++ ) {
                _ledpwmstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ][ mappingtable[ colour ][ xpos ][ ypos ][ 1 ] ] = 0;
            }
        }
    }
    return;
}


/// @brief Fills the pixel pwm buffer with a given byte.
/// @param statebyte The byte to use for fill.  0x00-0xFF.
void Pimoroni_5x5_rgb_matrix::pixelpwmStateBufferFill( uint8_t statebyte ) {
    for ( uint8_t colour = 0 ; colour < 3 ; colour++ ) {
        for ( uint8_t xpos = 0 ; xpos < 5 ; xpos++ ) {
            for ( uint8_t ypos = 0 ; ypos < 5 ; ypos++ ) {
                _ledpwmstate[ mappingtable[ colour ][ xpos ][ ypos ][ 0 ] ][ mappingtable[ colour ][ xpos ][ ypos ][ 1 ] ] = statebyte;
            }
        }
    }
    return;
}
































/// @brief Writes the in-memory pixel buffer to an on-chip frame buffer.
/// @param framenumber The frame number to write to.  0-7.
void Pimoroni_5x5_rgb_matrix::frameWrite( uint8_t framenumber ) {

    // now write to chip.
    _switchFrame( framenumber );

    // say hello to the chip again...
    wire.beginTransmission( _i2c_address );

    // send the address
    wire.write( 0x00 );

    for ( uint8_t i = 0 ; i < 18 ; i++ ) {
        // send the data
        wire.write( _ledstate[ i ] );

    }

    // say goodbye
    wire.endTransmission();




    // write out the blink values....

    // say hello to the chip again...
    wire.beginTransmission( _i2c_address );

    // send the address
    wire.write( 0x12 );

    for ( uint8_t i = 0 ; i < 18 ; i++ ) {
        // send the data
        wire.write( _ledblinkstate[ i ] );

    }

    // say goodbye
    wire.endTransmission();






    // write out the pwm values.
    // need to break it up into rows as the chip cant handle a full teble dump.
    // for each row...
    for ( uint8_t xpos = 0 ; xpos < 18 ; xpos++ ) {
        // say hello to the chip.
        wire.beginTransmission( _i2c_address );
        // send the row address.
        wire.write( ( 0x24 + ( xpos * 8 ) ) );
        // for each byte in the row.
        for ( uint8_t ypos = 0 ; ypos < 8 ; ypos++ ) {
            // send the byte.
            wire.write( _ledpwmstate[ xpos ][ ypos ] );
        }
        // say goodbye to the chip.
        wire.endTransmission();
    // all done for this row, loop around again for the next row.
    }
    


    // all done, return to caller.
    return;

}
















/// @brief Writes the in-memory pixel buffer to an on-chip frame buffer.
/// @param framenumber The frame number to write to.  0-7.
void Pimoroni_5x5_rgb_matrix::frameWritePixelState( uint8_t framenumber ) {

    // now write to chip.
    _switchFrame( framenumber );

    // say hello to the chip again...
    wire.beginTransmission( _i2c_address );

    // send the address
    wire.write( 0x00 );

    for ( uint8_t i = 0 ; i < 18 ; i++ ) {
        // send the data
        wire.write( _ledstate[ i ] );

    }

    // say goodbye
    wire.endTransmission();






    // all done, return to caller.
    return;

}







































// register config functions

// 0x00 configuration register



/// @brief Sets the display mode on the chip.
/// @param mode The mode number to set. 0b00 = picture mode, 0b01 = auto frame play, 0b1x = audio frame play.
void Pimoroni_5x5_rgb_matrix::displayModeSet( uint8_t mode ) {


    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REG );

    // add in my data
    tempbyte &= 0b11100111;
    tempbyte |= ( mode << 3 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REG , tempbyte );

    // all done, return to caller.
    return;

}







/// @brief Gets the display mode from the chip.
/// @return The current display mode number as a uint8_t. 0b00 = picture mode, 0b01 = auto frame play, 0b1x = audio frame play.
uint8_t Pimoroni_5x5_rgb_matrix::displayModeGet() {

    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REG );

    // get our data out
    tempbyte &= 0b00011000;
    
    return (uint8_t)( tempbyte >> 3 );

}




/// @brief Sets the start frame for autoplay
/// @param startframe The number of the frame to syart autoplay on. 0-7.
void Pimoroni_5x5_rgb_matrix::autoplayFrameStartSet( uint8_t startframe ) {

    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REG );

    // add in my data
    tempbyte &= 0b11111000;
    tempbyte |= ( startframe & 0b00000111 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REG , tempbyte );

    // all done, return to caller.
    return;

}

/// @brief Gets the start frame for autoplay
/// @return The number of the frame to start autoplay on as a uint8_t. 0-7.
uint8_t Pimoroni_5x5_rgb_matrix::autoplayFrameStartGet() {

    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;

}





// 0x01 Picture Display Register


/// @brief Set the chips frame display pointer
/// @param framenumber The number of the frame to display. 0-7.
void Pimoroni_5x5_rgb_matrix::frameDisplayPointerSet( uint8_t framenumber ) {
    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_PICTURE_DISPLAY_REG , framenumber );
}


/// @brief Fetches the current frame display pointer from the chip.
/// @return The current frame display pointer as a uint8_t. 0-7.
uint8_t Pimoroni_5x5_rgb_matrix::frameDisplayPointerGet() {
    return _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_PICTURE_DISPLAY_REG );
}







// 0x02 Autoplay Control Register 1

/// @brief Sets the number of loops to play in Auto frame Play mode.
/// @param numberofloops The number of loops to play. 0 = infinite, 1-7 plays that many loops.
void Pimoroni_5x5_rgb_matrix::autoplayNumberOfLoopsSet( uint8_t numberofloops ) {
    
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_ONE_REG );

    // add in my data
    tempbyte &= 0b10001111;
    tempbyte |= ( ( numberofloops & 0b00000111 ) << 4);

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_ONE_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Gets the number of loops to play in Auto Frame Play mode.
/// @return The number of loops to play.  0 = infinite, 1-7 plays that many loops.
uint8_t Pimoroni_5x5_rgb_matrix::autoplayNumberOfLoopsGet() {

    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_ONE_REG );

    // get our data out
    tempbyte &= 0b01110000;
    
    return ( tempbyte >> 4 );
}




/// @brief Sets the number of frames to play in Auto Frame Play mode.
/// @param  numberofframes The number of frames to play. 0 = all frames, 1-7 = that many frames.
void Pimoroni_5x5_rgb_matrix::autoplayNumberOfFramesPlayingSet( uint8_t numberofframes ) {
    
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_ONE_REG );

    // add in my data
    tempbyte &= 0b11111000;
    tempbyte |= ( numberofframes & 0b00000111 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_ONE_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Gets the number of frames to play in an Auto Frame Play mode.
/// @return The number of frames to play as a uint8_t. 0 = all framed, 1-7 = that many frames.
uint8_t Pimoroni_5x5_rgb_matrix::autoplayNumberOfFramesPlayingGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_ONE_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}











// 0x03 Autoplay Control Register 2

/// @brief Sets the frame delay time for Auto Frame Play mode.
/// @param framedelaytime The time each frame should be shown.
void Pimoroni_5x5_rgb_matrix::autoplayFrameDelayTimeSet( uint8_t framedelaytime ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_TWO_REG );

    // add in my data
    tempbyte &= 0b11000000;
    tempbyte |= ( framedelaytime & 0b00111111 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_TWO_REG , tempbyte );

    // all done, return to caller.
    return;
}


/// @brief Gets the frame delay time for Auto Frame Play mode.
/// @return The frame delay time as a uint8_t.
uint8_t Pimoroni_5x5_rgb_matrix::autoplayFrameDelayTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_TWO_REG );

    // get our data out
    tempbyte &= 0b00111111;
    
    return tempbyte;
}












// 0x05 Display Option Register

/// @brief Sets the intensity control bit.
/// @param intensitystate 0 = set the intensity of each frame independently.  1 = use frame 0 for all settings.
void Pimoroni_5x5_rgb_matrix::intensityControlSet( uint8_t intensitystate ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG );

    // add in my data
    tempbyte &= 0b11011111;
    tempbyte |= ( intensitystate & 0b00000001 ) << 5;

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Gets the intensity control bit.
/// @return The intensity control bit, as a uint8_1. 0 = set the intensity of each frame independently.  1 = use frame 0 for all settings.
uint8_t Pimoroni_5x5_rgb_matrix::intensityControlGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG );

    // get our data out
    tempbyte &= 0b00100000;
    
    return ( tempbyte >> 5 );
}


/// @brief Enable blinking!
/// @param blinkstate The blink state. 0 for disabled, 1 for enabled.
void Pimoroni_5x5_rgb_matrix::blinkEnableSet( uint8_t blinkstate ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG );

    // add in my data
    tempbyte &= 0b11110111;
    tempbyte |= ( ( blinkstate & 0b00000001 ) << 3 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Get the current blink state.
/// @return The current blink enable state as a uint8_t. 0 for disabled, 1 for enabled.
uint8_t Pimoroni_5x5_rgb_matrix::blinkEnableGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG );

    // get our data out
    tempbyte &= 0b00001000;
    
    return ( tempbyte >> 3 );
}

/// @brief Sets the blink period time.
/// @param  blinkperiodtime The amount of time to spend on each blink. 0-7 = bpt * 0.27s
void Pimoroni_5x5_rgb_matrix::blinkPeriodTimeSet( uint8_t blinkperiodtime ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG );

    // add in my data
    tempbyte &= 0b11111000;
    tempbyte |= ( blinkperiodtime & 0b00000111 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG , tempbyte );

    // all done, return to caller.
    return;

}

/// @brief Gets the blink period time
/// @return The blink period time multiplier, as a uint8_t.  0-7 = bpt * 0.27s
uint8_t Pimoroni_5x5_rgb_matrix::blinkPeriodTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}



// 0x06 Audio Synchronisation Register.

/// @brief Set the Audio Synchronisaton state.
/// @param state The desired state as a uint8_t. 0 = disable, 1 = enable.
void Pimoroni_5x5_rgb_matrix::audioSynchEnableSet( uint8_t state ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUDIO_SYNCH_REG );

    // add in my data
    tempbyte &= 0b11111110;
    tempbyte |= ( state & 0b00000001 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUDIO_SYNCH_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Get the Audio Synchronisation state.
/// @return The desired state as a uint8_t.  0 = disabled, 1 = enabled.
uint8_t Pimoroni_5x5_rgb_matrix::audioSynchEnableGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUDIO_SYNCH_REG );

    // get our data out
    tempbyte &= 0b00000001;
    
    return tempbyte;
}



// 0x07 Frame Display State Register. ( read only )

/// @brief Returns true when the Auto Frame Play process has finished.  Automatically cleared on read.
/// @return 0 if not finished.  1 when finished.  Automatically cleared on read.
uint8_t Pimoroni_5x5_rgb_matrix::frameDisplayInterruptGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_FRAME_STATE_REG );

    // get our data out
    tempbyte &= 0b00010000;
    
    return ( tempbyte >> 4 );
}

/// @brief Gets the number of the frame currently displayed in Auto Frame Play mode.
/// @return The frame number. 0-7.
uint8_t Pimoroni_5x5_rgb_matrix::currentFrameDisplayGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_FRAME_STATE_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}





// 0x08 Breath Control Register 1

/// @brief Sets the fade out time for breath control
/// @param fadetime 0-7. interval 26ms.
void Pimoroni_5x5_rgb_matrix::breathControlFadeOutTimeSet( uint8_t fadetime ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_ONE_REG );

    // add in my data
    tempbyte &= 0b10001111;
    tempbyte |= ( ( fadetime & 0b00000111 ) << 4 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_ONE_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Gets the fade out time for breath control.
/// @return 0-7. interval 26ms.
uint8_t Pimoroni_5x5_rgb_matrix::breathControlFadeOutTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_ONE_REG );

    // get our data out
    tempbyte &= 0b01110000;
    
    return ( tempbyte >> 4 );
}


/// @brief Sets the fade in time for breath control.
/// @param fadetime 0-7. interval 26ms.
void Pimoroni_5x5_rgb_matrix::breathControlFadeInTimeSet( uint8_t fadetime ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_ONE_REG );

    // add in my data
    tempbyte &= 0b11111000;
    tempbyte |= ( fadetime & 0b00000111 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_ONE_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Gets the fade in time for breath control.
/// @return 0-7. interval 26ms.
uint8_t Pimoroni_5x5_rgb_matrix::breathControlFadeInTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_ONE_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}





// 0x09 Breath Control Register 2

/// @brief Sets the enable flaf for the Breath Control system.
/// @param state 0 = disable , 1 = enable.
void Pimoroni_5x5_rgb_matrix::breathControlEnableSet( uint8_t state ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_TWO_REG );

    // add in my data
    tempbyte &= 0b11101111;
    tempbyte |= ( ( state & 0b00000001 ) << 4 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_TWO_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Gets the enable flag for the Breath Control system.
/// @return 0 = disable , 1 = enable.
uint8_t Pimoroni_5x5_rgb_matrix::breathControlEnableGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_TWO_REG );

    // get our data out
    tempbyte &= 0b00010000;
    
    return ( tempbyte >> 4 );
}


/// @brief Sets the time off, between fade out and fade in, for the Breath Control system.
/// @param fadetime 0-7. interval 3.5ms.
void Pimoroni_5x5_rgb_matrix::breathControlExtinguishTimeSet( uint8_t fadetime ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_TWO_REG );

    // add in my data
    tempbyte &= 0b11111000;
    tempbyte |= ( fadetime & 0b00000111 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_TWO_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Gets the time off, between fade out and fade on, from the Breath Control system.
/// @return 0-7. interval 3.5ms
uint8_t Pimoroni_5x5_rgb_matrix::breathControlExtinguishTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_TWO_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}







// 0x0A Shutdown Register.


/// @brief Sets the software shutdown flag on the chip.
/// @param state The state to set as a uint8_t. 0 = shutdown, 1 = normal operation.
void Pimoroni_5x5_rgb_matrix::softwareShutdownSet( uint8_t state ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_SOFTWARESHUTDOWN_REG );

    // add in my data
    tempbyte &= 0b11111110;
    tempbyte |= ( state & 0b00000001 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_SOFTWARESHUTDOWN_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Gets the software shutdown flag from the chip.
/// @return the flag as a uint8_t. 0 = shutdown, 1 = normal operation.
uint8_t Pimoroni_5x5_rgb_matrix::softwareShutdownGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_SOFTWARESHUTDOWN_REG );

    // get our data out
    tempbyte &= 0b00000001;
    
    return tempbyte;
}




// 0x0B AGC Control Register.

/// @brief Set the AGC mode.
/// @param state 0 = slow mode, 1 = fast mode.
void Pimoroni_5x5_rgb_matrix::audioagcModeSet( uint8_t state ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG );

    // add in my data
    tempbyte &= 0b11101111;
    tempbyte |= ( ( state & 0b00000001 ) << 4 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Get the AGC mode.
/// @return 0 = slow mode, 1 = fast mode.
uint8_t Pimoroni_5x5_rgb_matrix::audioagcModeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG );

    // get our data out
    tempbyte &= 0b00010000;
    
    return ( tempbyte >> 4 );
}

/// @brief Set the enable flag for AGC.
/// @param state 0 = disable, 1 = enable.
void Pimoroni_5x5_rgb_matrix::audioagcEnableSet( uint8_t state ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG );

    // add in my data
    tempbyte &= 0b11110111;
    tempbyte |= ( ( state & 0b00000001 ) << 3 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Get the enable flag for AGC.
/// @return 0 = disable, 1 = enable.
uint8_t Pimoroni_5x5_rgb_matrix::audioagcEnableGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG );

    // get our data out
    tempbyte &= 0b00001000;
    
    return ( tempbyte >> 3 );
}

/// @brief Sets the gain for the AGC
/// @param gain 0-7, interval 3dB
void Pimoroni_5x5_rgb_matrix::audioagcGainSet( uint8_t gain ) {
    // read out the current byte
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG );

    // add in my data
    tempbyte &= 0b11111000;
    tempbyte |= ( gain & 0b00000111 );

    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG , tempbyte );

    // all done, return to caller.
    return;
}

/// @brief Gets the gain for AGC.
/// @return 0-7, interval 3dB.
uint8_t Pimoroni_5x5_rgb_matrix::audioagcGainGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}





// 0x019 Audio ADC Rate Register

/// @brief Sets the ADC sample rate.
/// @param samplerate 0-255, interval 46us
void Pimoroni_5x5_rgb_matrix::audioadcSampleRateSet( uint8_t samplerate ) {
    
    // an actual 8 bit number?!
    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUDIO_ADC_RATE_REG , samplerate );

    // all done, return to caller.
    return;
}

/// @brief Gets the ADC sample rate.
/// @return 0-255, interval 46us
uint8_t Pimoroni_5x5_rgb_matrix::audioadcSampleRateGet() {
    return _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUDIO_ADC_RATE_REG );

}






















