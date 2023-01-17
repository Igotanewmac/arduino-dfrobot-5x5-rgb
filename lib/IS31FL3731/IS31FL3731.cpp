
// IS31FL3731 library code
#include <IS31FL3731.h>






/* ************************************* Private Methods ****************************************** */




/// @brief Switch to a different frame, if necessary.
/// @param framenumber The frame number to switch to.
void IS31FL3731::_switchFrame( uint8_t framenumber ) {

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
void IS31FL3731::_chipwritebyte( uint8_t framenumber , uint8_t address , uint8_t data ) {


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
uint8_t IS31FL3731::_chipreadbyte( uint8_t framenumber , uint8_t address ) {

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
















/* **************************   Public Methods *************************** */










IS31FL3731::IS31FL3731(/* args */)
{
}

IS31FL3731::~IS31FL3731()
{
}




/// @brief Set the i2c address and perform any setup required.
/// @param i2c_address The i2c address of the chip.
void IS31FL3731::begin( uint8_t i2c_address ) {

        // make sure the wire library is started.
    wire.begin();

    // store my i2c address for later.
    _i2c_address = i2c_address;

    // all done, return to caller
    return;

}

















/// @brief Write the pixel state to the chip.
/// @param framenumber The number of the frame to write to. 0-7.
/// @param framedata The frame data as an array of 18 uint8_t's.
void IS31FL3731::frameWriteState( uint8_t framenumber , uint8_t *framedata ) {

    // switch to the right frame
    _switchFrame( framenumber );

    // now send the data
    wire.beginTransmission( _i2c_address );
    wire.write( IS31FL3731_ADDRESS_FRAME_BUFFER_STATE );
    for ( uint8_t i = 0 ; i < 18 ; i++ ) {
        wire.write( framedata[i] );
    }
    wire.endTransmission();

    // all done, return to caller.
    return;

}






/// @brief Write the pixel blink state to the chip.
/// @param framenumber The number of the frame to write to. 0-7.
/// @param framedata The frame data as an array of 18 uint8_t's.
void IS31FL3731::frameWriteBlink( uint8_t framenumber , uint8_t *framedata ) {

    // switch to the right frame
    _switchFrame( framenumber );

    // now send the data
    wire.beginTransmission( _i2c_address );
    wire.write( IS31FL3731_ADDRESS_FRAME_BUFFER_BLINK );
    for ( uint8_t i = 0 ; i < 18 ; i++ ) {
        wire.write( framedata[i] );
    }
    wire.endTransmission();

    // all done, return to caller.
    return;

}










/// @brief Write the pixel pwm state to the chip.
/// @param framenumber The number of the frame to write to. 0-7.
/// @param framedata The frame data as an array of 18*8 uint8_t's.
void IS31FL3731::frameWritepwm( uint8_t framenumber , uint8_t *framedata ) {

    // switch to the right frame
    _switchFrame( framenumber );

    // for each row...
    for ( uint8_t row = 0 ; row < 18 ; row++ ) {

        // say hello to the chip
        wire.beginTransmission( _i2c_address );

        // send the address
        wire.write( IS31FL3731_ADDRESS_FRAME_BUFFER_PWM + ( 8 * row ) );

        // send the row data
        for ( uint8_t column = 0 ; column < 8 ; column++ ) {
            wire.write( framedata[ ( 8 * row ) + column ] );
        }

        // now say goodbye to the chip.
        wire.endTransmission();
        
        // now go around again for the next row...
    }

    // all done, return to caller.
    return;
}






































// register config functions

// 0x00 configuration register



/// @brief Sets the display mode on the chip.
/// @param mode The mode number to set. 0b00 = picture mode, 0b01 = auto frame play, 0b1x = audio frame play.
void IS31FL3731::displayModeSet( uint8_t mode ) {


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
uint8_t IS31FL3731::displayModeGet() {

    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REG );

    // get our data out
    tempbyte &= 0b00011000;
    
    return (uint8_t)( tempbyte >> 3 );

}




/// @brief Sets the start frame for autoplay
/// @param startframe The number of the frame to syart autoplay on. 0-7.
void IS31FL3731::autoplayFrameStartSet( uint8_t startframe ) {

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
uint8_t IS31FL3731::autoplayFrameStartGet() {

    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_CONFIG_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;

}





// 0x01 Picture Display Register


/// @brief Set the chips frame display pointer
/// @param framenumber The number of the frame to display. 0-7.
void IS31FL3731::frameDisplayPointerSet( uint8_t framenumber ) {
    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_PICTURE_DISPLAY_REG , framenumber );
}


/// @brief Fetches the current frame display pointer from the chip.
/// @return The current frame display pointer as a uint8_t. 0-7.
uint8_t IS31FL3731::frameDisplayPointerGet() {
    return _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_PICTURE_DISPLAY_REG );
}







// 0x02 Autoplay Control Register 1

/// @brief Sets the number of loops to play in Auto frame Play mode.
/// @param numberofloops The number of loops to play. 0 = infinite, 1-7 plays that many loops.
void IS31FL3731::autoplayNumberOfLoopsSet( uint8_t numberofloops ) {
    
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
uint8_t IS31FL3731::autoplayNumberOfLoopsGet() {

    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_ONE_REG );

    // get our data out
    tempbyte &= 0b01110000;
    
    return ( tempbyte >> 4 );
}




/// @brief Sets the number of frames to play in Auto Frame Play mode.
/// @param  numberofframes The number of frames to play. 0 = all frames, 1-7 = that many frames.
void IS31FL3731::autoplayNumberOfFramesPlayingSet( uint8_t numberofframes ) {
    
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
uint8_t IS31FL3731::autoplayNumberOfFramesPlayingGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_ONE_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}











// 0x03 Autoplay Control Register 2

/// @brief Sets the frame delay time for Auto Frame Play mode.
/// @param framedelaytime The time each frame should be shown.
void IS31FL3731::autoplayFrameDelayTimeSet( uint8_t framedelaytime ) {
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
uint8_t IS31FL3731::autoplayFrameDelayTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_TWO_REG );

    // get our data out
    tempbyte &= 0b00111111;
    
    return tempbyte;
}












// 0x05 Display Option Register

/// @brief Sets the intensity control bit.
/// @param intensitystate 0 = set the intensity of each frame independently.  1 = use frame 0 for all settings.
void IS31FL3731::intensityControlSet( uint8_t intensitystate ) {
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
uint8_t IS31FL3731::intensityControlGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG );

    // get our data out
    tempbyte &= 0b00100000;
    
    return ( tempbyte >> 5 );
}


/// @brief Enable blinking!
/// @param blinkstate The blink state. 0 for disabled, 1 for enabled.
void IS31FL3731::blinkEnableSet( uint8_t blinkstate ) {
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
uint8_t IS31FL3731::blinkEnableGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG );

    // get our data out
    tempbyte &= 0b00001000;
    
    return ( tempbyte >> 3 );
}

/// @brief Sets the blink period time.
/// @param  blinkperiodtime The amount of time to spend on each blink. 0-7 = bpt * 0.27s
void IS31FL3731::blinkPeriodTimeSet( uint8_t blinkperiodtime ) {
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
uint8_t IS31FL3731::blinkPeriodTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_DISPLAY_OPTION_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}



// 0x06 Audio Synchronisation Register.

/// @brief Set the Audio Synchronisaton state.
/// @param state The desired state as a uint8_t. 0 = disable, 1 = enable.
void IS31FL3731::audioSynchEnableSet( uint8_t state ) {
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
uint8_t IS31FL3731::audioSynchEnableGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUDIO_SYNCH_REG );

    // get our data out
    tempbyte &= 0b00000001;
    
    return tempbyte;
}



// 0x07 Frame Display State Register. ( read only )

/// @brief Returns true when the Auto Frame Play process has finished.  Automatically cleared on read.
/// @return 0 if not finished.  1 when finished.  Automatically cleared on read.
uint8_t IS31FL3731::frameDisplayInterruptGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_FRAME_STATE_REG );

    // get our data out
    tempbyte &= 0b00010000;
    
    return ( tempbyte >> 4 );
}

/// @brief Gets the number of the frame currently displayed in Auto Frame Play mode.
/// @return The frame number. 0-7.
uint8_t IS31FL3731::currentFrameDisplayGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_FRAME_STATE_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}





// 0x08 Breath Control Register 1

/// @brief Sets the fade out time for breath control
/// @param fadetime 0-7. interval 26ms.
void IS31FL3731::breathControlFadeOutTimeSet( uint8_t fadetime ) {
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
uint8_t IS31FL3731::breathControlFadeOutTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_ONE_REG );

    // get our data out
    tempbyte &= 0b01110000;
    
    return ( tempbyte >> 4 );
}


/// @brief Sets the fade in time for breath control.
/// @param fadetime 0-7. interval 26ms.
void IS31FL3731::breathControlFadeInTimeSet( uint8_t fadetime ) {
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
uint8_t IS31FL3731::breathControlFadeInTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_ONE_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}





// 0x09 Breath Control Register 2

/// @brief Sets the enable flaf for the Breath Control system.
/// @param state 0 = disable , 1 = enable.
void IS31FL3731::breathControlEnableSet( uint8_t state ) {
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
uint8_t IS31FL3731::breathControlEnableGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_TWO_REG );

    // get our data out
    tempbyte &= 0b00010000;
    
    return ( tempbyte >> 4 );
}


/// @brief Sets the time off, between fade out and fade in, for the Breath Control system.
/// @param fadetime 0-7. interval 3.5ms.
void IS31FL3731::breathControlExtinguishTimeSet( uint8_t fadetime ) {
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
uint8_t IS31FL3731::breathControlExtinguishTimeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_BREATH_CONTROL_TWO_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}







// 0x0A Shutdown Register.


/// @brief Sets the software shutdown flag on the chip.
/// @param state The state to set as a uint8_t. 0 = shutdown, 1 = normal operation.
void IS31FL3731::softwareShutdownSet( uint8_t state ) {
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
uint8_t IS31FL3731::softwareShutdownGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_SOFTWARESHUTDOWN_REG );

    // get our data out
    tempbyte &= 0b00000001;
    
    return tempbyte;
}




// 0x0B AGC Control Register.

/// @brief Set the AGC mode.
/// @param state 0 = slow mode, 1 = fast mode.
void IS31FL3731::audioagcModeSet( uint8_t state ) {
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
uint8_t IS31FL3731::audioagcModeGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG );

    // get our data out
    tempbyte &= 0b00010000;
    
    return ( tempbyte >> 4 );
}

/// @brief Set the enable flag for AGC.
/// @param state 0 = disable, 1 = enable.
void IS31FL3731::audioagcEnableSet( uint8_t state ) {
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
uint8_t IS31FL3731::audioagcEnableGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG );

    // get our data out
    tempbyte &= 0b00001000;
    
    return ( tempbyte >> 3 );
}

/// @brief Sets the gain for the AGC
/// @param gain 0-7, interval 3dB
void IS31FL3731::audioagcGainSet( uint8_t gain ) {
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
uint8_t IS31FL3731::audioagcGainGet() {
    uint8_t tempbyte = _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AGC_CONTROL_REG );

    // get our data out
    tempbyte &= 0b00000111;
    
    return tempbyte;
}





// 0x019 Audio ADC Rate Register

/// @brief Sets the ADC sample rate.
/// @param samplerate 0-255, interval 46us
void IS31FL3731::audioadcSampleRateSet( uint8_t samplerate ) {
    
    // an actual 8 bit number?!
    _chipwritebyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUDIO_ADC_RATE_REG , samplerate );

    // all done, return to caller.
    return;
}

/// @brief Gets the ADC sample rate.
/// @return 0-255, interval 46us
uint8_t IS31FL3731::audioadcSampleRateGet() {
    return _chipreadbyte( IS31FL3731_PAGE_CONTROL , IS31FL3731_ADDRESS_AUDIO_ADC_RATE_REG );

}



















