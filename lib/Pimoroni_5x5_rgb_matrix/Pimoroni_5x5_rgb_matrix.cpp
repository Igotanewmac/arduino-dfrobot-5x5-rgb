
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
    // softwareShutdownSet( 0 );
    
    // set to picture display mode
    // displayModeSet( 0x00 );

    // set the frame pointer to zero
    // frameDisplayPointerSet( 0x00 );

    // clear the buffers
    //pixelStateBufferClear();
    //pixelpwmStateBufferClear();
    //pixelBufferClearAll();

    // now write them out
    //pixelBufferWriteAllToFrame( 0x00 );

    // now turn the chip back on.
    // softwareShutdownSet( 1 );
    
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





























































