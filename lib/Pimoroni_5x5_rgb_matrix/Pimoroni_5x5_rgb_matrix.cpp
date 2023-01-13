
#include <Arduino.h>

#include <Wire.h>
#define wire Wire

#include <Pimoroni_5x5_rgb_matrix.h>





Pimoroni_5x5_rgb_matrix::Pimoroni_5x5_rgb_matrix( uint8_t new_i2c_address ) {

    _i2c_address = new_i2c_address;

}

Pimoroni_5x5_rgb_matrix::~Pimoroni_5x5_rgb_matrix() {

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







































