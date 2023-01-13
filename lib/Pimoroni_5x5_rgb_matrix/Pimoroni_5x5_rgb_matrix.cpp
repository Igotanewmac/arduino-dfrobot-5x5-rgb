
#include <Arduino.h>

#include <Wire.h>
#define wire Wire

#include <Pimoroni_5x5_rgb_matrix.h>





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
    pixelBufferClearAll();

    // now write them out
    pixelBufferWriteAllToFrame( 0x00 );

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



















/// @brief Optimised write to chip.
/// @param framenumber The frame number to write to.
void Pimoroni_5x5_rgb_matrix::_pixelBufferStateFastWrite( uint8_t framenumber ) {

    _switchFrame( framenumber );

    // say hello to the chip again...
    wire.beginTransmission( _i2c_address );

    // send the base address 0x00 for pixel state data
    wire.write( 0x00 );

    // now send the pixel array in the right sequence

    // say goodbye
    wire.endTransmission();

    // all done, return to caller
    return;


}


/// @brief Optimised write to chip.
/// @param framenumber The frame number to write to.
void Pimoroni_5x5_rgb_matrix::_pixelBufferBlinkStateFastWrite( uint8_t framenumber ) {
    
    _switchFrame( framenumber );

    // say hello to the chip again...
    wire.beginTransmission( _i2c_address );


    // send the base address 0x00 for pixel state data
    wire.write( 0x12 );

    // say goodbye
    wire.endTransmission();

    // all done, return to caller
    return;
}


/// @brief Optimised write to chip.
/// @param  framenumber The frame number to write to.
void Pimoroni_5x5_rgb_matrix::_pixelBufferpwmStateFastWrite( uint8_t framenumber ) {

    _switchFrame( framenumber );

    
   
    // now send the pixel array in the right sequence
   
    




    // all done, return to caller
    return;


}




































/// @brief Write the pixel buffer to a frame on the chip.
/// @param framenumber The number of the frame to write to. 0-7.
void Pimoroni_5x5_rgb_matrix::pixelBufferWriteAllToFrame( uint8_t framenumber ) {

    _pixelBufferStateFastWrite( framenumber );

    _pixelBufferBlinkStateFastWrite( framenumber );

    _pixelBufferpwmStateFastWrite( framenumber );
    
    // now all done, return to caller.
    return;

}







/// @brief Write the pixel state buffer to a frame on the chip.
/// @param framenubmer The number of the frame to write. 0-7.
void Pimoroni_5x5_rgb_matrix::pixelBufferStateWriteToFrame( uint8_t framenumber ) {
    
    _pixelBufferStateFastWrite( framenumber );
    
}

/// @brief Write the pixel blink state buffer to a frame on the chip.
/// @param framenumber The number of the frame to write to. 0-7.
void Pimoroni_5x5_rgb_matrix::pixelBufferBlinkStateWriteToFrame( uint8_t framenumber ) {
    
    _pixelBufferBlinkStateFastWrite( framenumber );

}

/// @brief Write the pixel pwm state buffer to a frame on the chip.
/// @param framenumber The number of the frame to write to. 0-7.
void Pimoroni_5x5_rgb_matrix::pixelBufferpwmStateWriteToFrame( uint8_t framenumber ) {

    _pixelBufferpwmStateFastWrite( framenumber );

}



















/// @brief Sets the pixel buffers for state, blink and pwm to all zero.
void Pimoroni_5x5_rgb_matrix::pixelBufferClearAll() {

    // for each column of pixel buffers
    for ( uint8_t x = 0 ; x < 11 ; x++ ) {

        // set the whole row of _ledstate to zero.
        _ledstate[ x ] = 0x00;

        // set the whole row of _ledblinkstate to zero.
        _ledblinkstate[ x ] = 0x00;

        // for each pwm value in the row
        for ( uint8_t y = 0 ; y < 7 ; y++ ) {

            // set it to zero.
            _ledpwmstate[ x ][ y ] = 0x00;

        }

    }

    // all done, return to caller.
    return;

}




/// @brief Sets the pixel buffer for state to all zero.
void Pimoroni_5x5_rgb_matrix::pixelBufferStateClear() {

    // for each element in the _ledstate array...
    for ( uint8_t i = 0 ; i < 11 ; i++ ) {

        // set it to zero.
        _ledstate[ i ] = 0x00;

    }

    // all done, return to caller.
    return;

}



/// @brief Sets the pixel buffer for blink state to all zero.
void Pimoroni_5x5_rgb_matrix::pixelBufferBlinkStateClear() {

    // for each element in the _ledblinkstate array...
    for ( uint8_t i = 0 ; i < 11 ; i++ ) {

        // set it to zero
        _ledblinkstate[ i ] = 0x00;

    }

    // all done, return to caller.
    return;

}



/// @brief Sets the pixel buffer for pwm value to all zero.
void Pimoroni_5x5_rgb_matrix::pixelBufferpwmStateClear() {

    // for each column in the _ledpwmstate array...
    for ( uint8_t x = 0 ; x < 11 ; x++ ) {

        // for each row in the _ledpwmstate array...
        for ( uint8_t y = 0 ; y < 7 ; y++ ) {

            // set it to zero.
            _ledpwmstate[ x ][ y ] = 0x00;

        }

    }

    // all done, return to caller.
    return;
}







/// @brief Set all pixels state to the given value.
/// @param data 0 = off, 1 = on.
void Pimoroni_5x5_rgb_matrix::pixelBufferStateFill( uint8_t data ) {

    // for each element in the _ledstate array...
    for ( uint8_t i = 0 ; i < 11 ; i++ ) {

        // set it to zero.
        _ledstate[ i ] = data;

    }

    // all done, return to caller.
    return;

}

/// @brief Set all pixels blink state to the given value
/// @param data 0 = off, 1 = on.
void Pimoroni_5x5_rgb_matrix::pixelBufferBlinkStateFill( uint8_t data ) {
    
    // for each element in the _ledstate array...
    for ( uint8_t i = 0 ; i < 11 ; i++ ) {

        // set it to zero.
        _ledblinkstate[ i ] = data;

    }

    // all done, return to caller.
    return;

}

/// @brief Set all pixels pwm value to the given value.
/// @param data 0-255. 0 is fully off, 255 is fully on.
void Pimoroni_5x5_rgb_matrix::pixelBufferpwmStateFill( uint8_t data ) {
    // for each column in the _ledpwmstate array...
    for ( uint8_t x = 0 ; x < 11 ; x++ ) {

        // for each row in the _ledpwmstate array...
        for ( uint8_t y = 0 ; y < 7 ; y++ ) {

            // set it to zero.
            _ledpwmstate[ x ][ y ] = data;

        }

    }

    // all done, return to caller.
    return;
}




















/// @brief Sets a pixel to on or off in the pixel buffer.
/// @param xpos The x position, with zero at the bottom left.
/// @param ypos The y position, with the zero at the bottom left.
/// @param state The state, 1 for on, 0 for off.
void Pimoroni_5x5_rgb_matrix::pixelSet( uint8_t xpos , uint8_t ypos , uint8_t state ) {

    // check if we are turning the bit on, or off.
    if ( state ) {

        // set bit to 1
        _ledstate[ xpos ] |= ( 0b00000001 << ypos );

    }
    else {

        // set bit to 0
        _ledstate[ xpos ] &= ~( 0b00000001 << ypos );
    }

    // all done, return to caller.
    return;
}



/// @brief Gets the value of a pixel from the pixel buffer.
/// @param xpos The x position, with zero at the bottom left.
/// @param ypos The y position, with zero at the bottom left.
/// @return The state of the pixel as a uint8_t.  0 for off, 1 for on.
uint8_t Pimoroni_5x5_rgb_matrix::pixelGet( uint8_t xpos , uint8_t ypos ) {

    // return the bit for this pixel as a uint8_t.
    return (uint8_t)( ( _ledstate[ xpos ] >> ypos ) & 0b00000001 );

}





/// @brief Set the blink flag for a pixel on or off in the pixel buffer.
/// @param xpos The x position, with zero at the bottom left.
/// @param ypos The y position, with zero at the bottom left.
/// @param state The state of the blink flag as a uint8_t.  0 for off, 1 for on.
void Pimoroni_5x5_rgb_matrix::pixelBlinkSet( uint8_t xpos , uint8_t ypos , uint8_t state ){

    // check if we are turning the bit on, or off.
    if ( state ) {

        // set bit to 1
        _ledblinkstate[ xpos ] |= ( 0b00000001 << ypos );

    }
    else {

        // set bit to 0
        _ledblinkstate[ xpos ] &= ~( 0b00000001 << ypos );
    }

    // all done, return to caller.
    return;

}

/// @brief Get the state of a pixels blink flag from the pixel buffer.
/// @param xpos The x position, with zero at the bottom left.
/// @param ypos The y position, with zero at the bottom left.
/// @return The state of the pixel as a uint8_t.  0 for off, 1 for on.
uint8_t Pimoroni_5x5_rgb_matrix::pixelBlinkGet( uint8_t xpos , uint8_t ypos ) {
    
    // return the bit for this pixel as a uint8_t.
    return (uint8_t)( ( _ledblinkstate[ xpos ] >> ypos ) & 0b00000001 );

}





/// @brief Set the pwm value for a pixel in the pixel buffer
/// @param xpos The x position of the pixel, with zero at the bottom left.
/// @param ypos The y position of the pixel, with zero at the bottom left.
/// @param state The pwm value to set, as a uint8_t.  0 is full off, 255 is full on.
void Pimoroni_5x5_rgb_matrix::pixelpwmSet( uint8_t xpos , uint8_t ypos , uint8_t state ) {

    // set the pixel pwm value in the array
    _ledpwmstate[ xpos ][ ypos ] = state;

    // all done, return to caller.
    return;

}

/// @brief Get the pwm value for a pixel from the pixel buffer.
/// @param xpos The x position of the pixel, with zero at the bottom left.
/// @param ypos The y position of the pixel, with zero at the bottom left.
/// @return The pwm value of the pixel as a uint8_t.  0 is fully off, 255 is fully on.
uint8_t Pimoroni_5x5_rgb_matrix::pixelpwmGet( uint8_t xpos , uint8_t ypos ) {

    // return the byte for this pixel as a uint8_t.
    return _ledpwmstate[ xpos ][ ypos ];

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






















