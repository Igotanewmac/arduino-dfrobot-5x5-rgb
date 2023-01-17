
#ifndef _IS31FL3731_HEADER_GUARD
#define _IS31FL3731_HEADER_GUARD



// IS31FL3731 library header

// include Arduino header
#include <Arduino.h>

#ifndef wire
// Wire library
#include <Wire.h>
#define wire Wire
#endif






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

// frame buffer addresses
#define IS31FL3731_ADDRESS_FRAME_BUFFER_STATE 0x00
#define IS31FL3731_ADDRESS_FRAME_BUFFER_BLINK 0x12
#define IS31FL3731_ADDRESS_FRAME_BUFFER_PWM 0x24








class IS31FL3731 {

    private:

        /* data */

        /// @brief The i2c address of the chip.
        uint8_t _i2c_address = 0x00;

        /// @brief The last frame number we switched to.  0-7, 0b for control page.
        uint8_t _currentframe;

        /// @brief Switch to a different frame, if necessary.
        /// @param framenumber The frame number to switch to.
        void _switchFrame( uint8_t framenumber );

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




    public:

        /// @brief Constructor.  Doesn't do anything but create the object.
        IS31FL3731();

        /// @brief Destructor.  Doesn't actually do anything, included fir future expansion if needed.
        ~IS31FL3731();


        /// @brief Set the i2c address and perform any setup required.
        /// @param i2c_address The i2c address of the chip.
        void begin( uint8_t i2c_address );


        // frame write commands
        // just take a pointer to the right sized array and blit it to the right frame.
        // let the parent handle the mapping and pixel buffering.

        
        /// @brief Write the pixel state to the chip.
        /// @param framenumber The number of the frame to write to. 0-7.
        /// @param framedata The frame data as an array of 18 uint8_t's.
        void frameWriteState( uint8_t framenumber , uint8_t *framedata );

        /// @brief Write the pixel blink state to the chip.
        /// @param framenumber The number of the frame to write to. 0-7.
        /// @param framedata The frame data as an array of 18 uint8_t's.
        void frameWriteBlink( uint8_t framenumber , uint8_t *framedata );

        /// @brief Write the pixel pwm state to the chip.
        /// @param framenumber The number of the frame to write to. 0-7.
        /// @param framedata The frame data as an array of 18*8 uint8_t's.
        void frameWritepwm( uint8_t framenumber , uint8_t *framedata );

        








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
