

#ifndef PIMORONI_11X7MATRIX_HEADER_GUARD
#define PIMORONI_11X7MATRIX_HEADER_GUARD


// header for 11x7 matrix board by pimoroni

// pull in the arduino headers
#include <Arduino.h>

// pull in the wire library
#ifndef wire
#include <Wire.h>
#define wire Wire
#endif




// a whole bunch of definitions

// the animation page that refers to the control register
#define IS31FL3731_PAGE_CONTROL 0x0B

// the control register addresses
#define IS31FL3731_ADDRESS_CONFIG_REG 0x00
#define IS31FL3731_ADDRESS_PICTURE_DISPLAY_REG 0x01
#define IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_ONE_REG 0x02
#define IS31FL3731_ADDRESS_AUTOPLAY_CONTROL_TWO_REG 0x03
#define IS31FL3731_ADDRESS_DISPLAY_OPTION_REG 0x05
#define IS31FL3731_ADDRESS_AUDIO_SYNCH_REG 0x05
#define IS31FL3731_ADDRESS_FRAME_STATE_REG 0x07
#define IS31FL3731_ADDRESS_BREATH_CONTROL_ONE_REG 0x08
#define IS31FL3731_ADDRESS_BREATH_CONTROL_TWO_REG 0x09
#define IS31FL3731_ADDRESS_SOFTWARESHUTDOWN_REG 0x0A
#define IS31FL3731_ADDRESS_AGC_CONTROL_REG 0x0B
#define IS31FL3731_ADDRESS_AUDIO_ADC_RATE_REG 0x0C







class Pimoroni_11x7matrix {


    private:

    /// @brief The pixel buffer for the on/pff state.
    uint8_t _ledstate[11];

    /// @brief The pixel buffer for the blink on/off state.
    uint8_t _ledblinkstate[11];

    /// @brief The pixel buffer for the pwm values.
    uint8_t _ledpwmstate[11][7];

    /// @brief The i2c address of the chip.
    uint8_t _i2c_address;


    /// @brief Write a single byte of data to the chip.
    /// @param framenumber The number of the frame to write to. 0x00-0x07 Animation. 0x0B Control.
    /// @param address The address within the frame to write to.
    /// @param data The data byte to write to the chip.
    void _chipwritebyte( uint8_t framenumber , uint8_t address , uint8_t data );

    /// @brief Read a single byte of data from the chip.
    /// @param framenumber The number of the frame to read from. 0x00-0x07 Animation. 0x0B Control.
    /// @param address The address within the frame to read from.
    /// @return The data byte rturned from the chip as a uint8_t.
    uint8_t _chipreadbyte( uint8_t framenumber , uint8_t address );
    
    
    


    /// @brief Optimised write to chip.
    /// @param framenumber The frame number to write to.
    void _pixelBufferStateFastWrite( uint8_t framenumber );


    /// @brief Optimised write to chip.
    /// @param framenumber The frame number to write to.
    void _pixelBufferBlinkStateFastWrite( uint8_t framenumber );


    /// @brief Optimised write to chip.
    /// @param  framenumber The frame number to write to.
    void _pixelBufferpwmStateFastWrite( uint8_t framenumber );




    /// @brief The last frame number we switched to.  0-7, 0b for control page.
    uint8_t _currentframe;

    /// @brief Switch to a different frame, if necessary.
    /// @param framenumber The frame number to switch to.
    void _switchFrame( uint8_t framenumber );











    public:

    /// @brief Constructor for Pimoroni 11x7 Matrix Driver
    Pimoroni_11x7matrix();


    /// @brief Set the i2c address and perform any setup required.
    /// @param new_i2c_address The i2c address of the chip.
    void begin( uint8_t new_i2c_address );











    /// @brief Write the pixel buffer to a frame on the chip.
    /// @param framenumber The number of the frame to write to. 0-7.
    void pixelBufferWriteAllToFrame( uint8_t framenumber );


    /// @brief Write the pixel state buffer to a frame on the chip.
    /// @param framenubmer The number of the frame to write. 0-7.
    void pixelBufferStateWriteToFrame( uint8_t framenumber );

    /// @brief Write the pixel blink state buffer to a frame on the chip.
    /// @param framenumber The number of the frame to write to. 0-7.
    void pixelBufferBlinkStateWriteToFrame( uint8_t framenumber );

    /// @brief Write the pixel pwm state buffer to a frame on the chip.
    /// @param framenumber The number of the frame to write to. 0-7.
    void pixelBufferpwmStateWriteToFrame( uint8_t framenumber );




    /// @brief Sets the pixel buffers for state, blink and pwm to all zero.
    void pixelBufferClearAll();


    /// @brief Sets the pixel buffer for state to all zero.
    void pixelBufferStateClear();

    /// @brief Sets the pixel buffer for blink state to all zero.
    void pixelBufferBlinkStateClear();

    /// @brief Sets the pixel buffer for pwm value to all zero.
    void pixelBufferpwmStateClear();



    /// @brief Set all pixels state to the given value.
    /// @param data 0 = off, 1 = on.
    void pixelBufferStateFill( uint8_t data );

    /// @brief Set all pixels blink state to the given value
    /// @param data 0 = off, 1 = on.
    void pixelBufferBlinkStateFill( uint8_t data );

    /// @brief Set all pixels pwm value to the given value.
    /// @param data 0-255. 0 is fully off, 255 is fully on.
    void pixelBufferpwmStateFill( uint8_t data );









    /// @brief Sets a pixel to on or off in the pixel buffer.
    /// @param xpos The x position, with zero at the bottom left.
    /// @param ypos The y position, with the zero at the bottom left.
    /// @param state The state, 1 for on, 0 for off.
    void pixelSet( uint8_t xpos , uint8_t ypos , uint8_t state );

    /// @brief Gets the value of a pixel from the pixel buffer.
    /// @param xpos The x position, with zero at the bottom left.
    /// @param ypos The y position, with zero at the bottom left.
    /// @return The state of the pixel as a uint8_t.  0 for off, 1 for on.
    uint8_t pixelGet( uint8_t xpos , uint8_t ypos );





    /// @brief Set the blink flag for a pixel on or off in the pixel buffer.
    /// @param xpos The x position, with zero at the bottom left.
    /// @param ypos The y position, with zero at the bottom left.
    /// @param state The state of the blink flag as a uint8_t.  0 for off, 1 for on.
    void pixelBlinkSet( uint8_t xpos , uint8_t ypos , uint8_t state );

    /// @brief Get the state of a pixels blink flag from the pixel buffer.
    /// @param xpos The x position, with zero at the bottom left.
    /// @param ypos The y position, with zero at the bottom left.
    /// @return The state of the pixel as a uint8_t.  0 for off, 1 for on.
    uint8_t pixelBlinkGet( uint8_t xpos , uint8_t ypos );





    /// @brief Set the pwm value for a pixel in the pixel buffer
    /// @param xpos The x position of the pixel, with zero at the bottom left.
    /// @param ypos The y position of the pixel, with zero at the bottom left.
    /// @param state The pwm value to set, as a uint8_t.  0 is full off, 255 is full on.
    void pixelpwmSet( uint8_t xpos , uint8_t ypos , uint8_t state );

    /// @brief Get the pwm value for a pixel from the pixel buffer.
    /// @param xpos The x position of the pixel, with zero at the bottom left.
    /// @param ypos The y position of the pixel, with zero at the bottom left.
    /// @return The pwm value of the pixel as a uint8_t.  0 is fully off, 255 is fully on.
    uint8_t pixelpwmGet( uint8_t xpos , uint8_t ypos );









    // register config functions

    // 0x00 configuration register

    /// @brief Sets the display mode on the chip.
    /// @param mode The mode number to set. 0b00 = picture mode, 0b01 = auto frame play, 0b1x = audio frame play.
    void displayModeSet( uint8_t mode );

    /// @brief Gets the display mode from the chip.
    /// @return The current display mode number as a uint8_t. 0b00 = picture mode, 0b01 = auto frame play, 0b1x = audio frame play.
    uint8_t displayModeGet();

    /// @brief Sets the start frame for autoplay
    /// @param startframe The number of the frame to syart autoplay on. 0-7.
    void autoplayFrameStartSet( uint8_t startframe );

    /// @brief Gets the start frame for autoplay
    /// @return The number of the frame to start autoplay on as a uint8_t. 0-7.
    uint8_t autoplayFrameStartGet();





    // 0x01 Picture Display Register

    /// @brief Set the chips frame display pointer
    /// @param framenumber The number of the frame to display. 0-7.
    void frameDisplayPointerSet( uint8_t framenumber );


    /// @brief Fetches the current frame display pointer from the chip.
    /// @return The current frame display pointer as a uint8_t. 0-7.
    uint8_t frameDisplayPointerGet();




    // 0x02 Autoplay Control Register 1

    /// @brief Sets the number of loops to play in Auto frame Play mode.
    /// @param numberofloops The number of loops to play. 0 = infinite, 1-7 plays that many loops.
    void autoplayNumberOfLoopsSet( uint8_t numberofloops );

    /// @brief Gets the number of loops to play in Auto Frame Play mode.
    /// @return The number of loops to play.  0 = infinite, 1-7 plays that many loops.
    uint8_t autoplayNumberOfLoopsGet();


    /// @brief Sets the number of frames to play in Auto Frame Play mode.
    /// @param  numberofframes The number of frames to play. 0 = all frames, 1-7 = that many frames.
    void autoplayNumberOfFramesPlayingSet( uint8_t numberofframes );

    /// @brief Gets the number of frames to play in an Auto Frame Play mode.
    /// @return The number of frames to play as a uint8_t. 0 = all framed, 1-7 = that many frames.
    uint8_t autoplayNumberOfFramesPlayingGet();



    // 0x03 Autoplay Control Register 2

    /// @brief Sets the frame delay time for Auto Frame Play mode.
    /// @param framedelaytime The time each frame should be shown.
    void autoplayFrameDelayTimeSet( uint8_t framedelaytime );

    /// @brief Gets the frame delay time for Auto Frame Play mode.
    /// @return The frame delay time as a uint8_t.
    uint8_t autoplayFrameDelayTimeGet();




    // 0x05 Display Option Register

    /// @brief Sets the intensity control bit.
    /// @param intensitystate 0 = set the intensity of each frame independently.  1 = use frame 0 for all settings.
    void intensityControlSet( uint8_t intensitystate );

    /// @brief Gets the intensity control bit.
    /// @return The intensity control bit, as a uint8_1. 0 = set the intensity of each frame independently.  1 = use frame 0 for all settings.
    uint8_t intensityControlGet();

    /// @brief Enable blinking!
    /// @param blinkstate The blink state. 0 for disabled, 1 for enabled.
    void blinkEnableSet( uint8_t blinkstate );

    /// @brief Get the current blink state.
    /// @return The current blink enable state as a uint8_t. 0 for disabled, 1 for enabled.
    uint8_t blinkEnableGet();

    /// @brief Sets the blink period time.
    /// @param  blinkperiodtime The amount of time to spend on each blink. 0-7 = bpt * 0.27s
    void blinkPeriodTimeSet( uint8_t blinkperiodtime );

    /// @brief Gets the blink period time
    /// @return The blink period time multiplier, as a uint8_t.  0-7 = bpt * 0.27s
    uint8_t blinkPeriodTimeGet();



    // 0x06 Audio Synchronisation Register.

    /// @brief Set the Audio Synchronisaton state.
    /// @param state The desired state as a uint8_t. 0 = disable, 1 = enable.
    void audioSynchEnableSet( uint8_t state );

    /// @brief Get the Audio Synchronisation state.
    /// @return The desired state as a uint8_t.  0 = disabled, 1 = enabled.
    uint8_t audioSynchEnableGet();



    // 0x07 Frame Display State Register. ( read only )

    /// @brief Returns true when the Auto Frame Play process has finished.  Automatically cleared on read.
    /// @return 0 if not finished.  1 when finished.  Automatically cleared on read.
    uint8_t frameDisplayInterruptGet();

    /// @brief Gets the number of the frame currently displayed in Auto Frame Play mode.
    /// @return The frame number. 0-7.
    uint8_t currentFrameDisplayGet();





    // 0x08 Breath Control Register 1

    /// @brief Sets the fade out time for breath control
    /// @param fadeouttime 0-7. interval 26ms.
    void breathControlFadeOutTimeSet( uint8_t fadetime );

    /// @brief Gets the fade out time for breath control.
    /// @return 0-7. interval 26ms.
    uint8_t breathControlFadeOutTimeGet();


    /// @brief Sets the fade in time for breath control.
    /// @param fadeintime 0-7. interval 26ms.
    void breathControlFadeInTimeSet( uint8_t fadetime );

    /// @brief Gets the fade in time for breath control.
    /// @return 0-7. interval 26ms.
    uint8_t breathControlFadeInTimeGet();


    // 0x09 Breath Control Register 2

    /// @brief Sets the enable flaf for the Breath Control system.
    /// @param state 0 = disable , 1 = enable.
    void breathControlEnableSet( uint8_t state );

    /// @brief Gets the enable flag for the Breath Control system.
    /// @return 0 = disable , 1 = enable.
    uint8_t breathControlEnableGet();


    /// @brief Sets the time off, between fade out and fade in, for the Breath Control system.
    /// @param fadetime 0-7. interval 3.5ms.
    void breathControlExtinguishTimeSet( uint8_t fadetime );

    /// @brief Gets the time off, between fade out and fade in, for the Breath Control system.
    /// @return 0-7. interval 3.5ms
    uint8_t breathControlExtinguishTimeGet();







    // 0x0A Shurdown Register.

    /// @brief Sets the software shutdown flag on the chip.
    /// @param state The state to set as a uint8_t. 0 = shutdown, 1 = normal operation.
    void softwareShutdownSet( uint8_t state );

    /// @brief Gets the software shutdown flag from the chip.
    /// @return the flag as a uint8_t. 0 = shutdown, 1 = normal operation.
    uint8_t softwareShutdownGet();



    // 0x0B AGC Control Register.

    /// @brief Set the AGC mode.
    /// @param state 0 = slow mode, 1 = fast mode.
    void audioagcModeSet( uint8_t state );

    /// @brief Get the AGC mode.
    /// @return 0 = slow mode, 1 = fast mode.
    uint8_t audioagcModeGet();

    /// @brief Set the enable flag for AGC.
    /// @param state 0 = disable, 1 = enable.
    void audioagcEnableSet( uint8_t state );

    /// @brief Get the enable flag for AGC.
    /// @return 0 = disable, 1 = enable.
    uint8_t audioagcEnableGet();

    /// @brief Sets the gain for the AGC
    /// @param gain 0-7, interval 3dB
    void audioagcGainSet( uint8_t gain );

    /// @brief Gets the gain for AGC.
    /// @return 0-7, interval 3dB.
    uint8_t audioagcGainGet();





    // 0x019 Audio ADC Rate Register

    /// @brief Sets the ADC sample rate.
    /// @param samplerate 0-255, interval 46us
    void audioadcSampleRateSet( uint8_t samplerate );

    /// @brief Gets the ADC sample rate.
    /// @return 0-255, interval 46us
    uint8_t audioadcSampleRateGet();








};






























#endif

