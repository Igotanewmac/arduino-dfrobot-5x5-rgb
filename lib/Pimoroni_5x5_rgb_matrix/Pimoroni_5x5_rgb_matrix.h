

// driver class for pimoroni 5x5 rgb matrix

#ifndef PIMORONI_5X5_RGB_INCLUDE_GUARD
#define PIMORONI_5X5_RGB_INCLUDE_GUARD



#include <IS31FL3731.h>









class  Pimoroni_5x5_rgb_matrix : public IS31FL3731 {
    
    private:

        uint8_t _i2c_address = 0x74;

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
    





        /// @brief The pixel buffer for the on/pff state.
        uint8_t _ledstate[ 18 ] = { 0 };

        /// @brief The pixel buffer for the blink on/off state.
        uint8_t _ledblinkstate[ 18 ] = { 0 };

        /// @brief The pixel buffer for the pwm values.
        uint8_t _ledpwmstate[ 18 ][ 8 ] = { 0 };




        




    public:

        Pimoroni_5x5_rgb_matrix( uint8_t new_i2c_address );
        ~Pimoroni_5x5_rgb_matrix();

        /// @brief Set the i2c address and perform any setup required.
        /// @param new_i2c_address The i2c address of the chip.
        void begin( uint8_t new_i2c_address );




        // ((( ------------------- pizel code goes here ======================= )))



        /// @brief Set a pixels power state to on or off.
        /// @param xpos The x position, with 0 at the left.
        /// @param ypos The y position, with 0 at the top.
        /// @param colour The colour bank to set. 0 = red, 1 = blue , 2 = green.
        /// @param state  The on-off state of the pixel. 0 = off, 1 = on.
        void pixelSet( uint8_t xpos , uint8_t ypos , uint8_t colour , uint8_t state );

        /// @brief Get a pixels power state.
        /// @param xpos The x position, with 0 on the left.
        /// @param ypos The y position, with 0 at the top.
        /// @param colour The colour bank to check. 0 = red, 1 = blue , 2 = green.
        /// @return The on-off state of the pixel, as a uint8_t. 0 = off, 1 = on.
        uint8_t pixelGet( uint8_t xpos , uint8_t ypos , uint8_t colour );

        /// @brief Clear the pixel state buffer to all zero.
        void pixelStateBufferClear();

        /// @brief Fills the pixel buffer with a given byte.
        /// @param statebyte The byte to use for fill.  LSB is leftmost pixel. order XXX43210.
        void pixelStateBufferFill( uint8_t statebyte );





        /// @brief Set a pixels blink state to on or off.
        /// @param xpos The x position, with 0 at the left.
        /// @param ypos The y position, with 0 at the top.
        /// @param colour The colour bank to set. 0 = red, 1 = blue , 2 = green.
        /// @param state  The on-off blink state of the pixel. 0 = off, 1 = on.
        void pixelBlinkSet( uint8_t xpos , uint8_t ypos , uint8_t colour , uint8_t state );

        /// @brief Get a pixels blink state.
        /// @param xpos The x position, with 0 on the left.
        /// @param ypos The y position, with 0 at the top.
        /// @param colour The colour bank to check. 0 = red, 1 = blue , 2 = green.
        /// @return The on-off blink state of the pixel, as a uint8_t. 0 = off, 1 = on.
        uint8_t pixelBlinkGet( uint8_t xpos , uint8_t ypos , uint8_t colour );

        /// @brief Clear the pixel blink state buffer to all zero.
        void pixelBlinkStateBufferClear();

        /// @brief Fills the pixel blink state buffer with a given byte.
        /// @param statebyte The byte to use for fill.  LSB is leftmost pixel. order XXX43210.
        void pixelBlinkStateBufferFill( uint8_t statebyte );





        /// @brief Set a pixels pwm value.
        /// @param xpos The x position, with 0 at the left.
        /// @param ypos The y position, with 0 at the top.
        /// @param colour The colour bank to set. 0 = red, 1 = blue , 2 = green.
        /// @param state  The pwm value of the pixel. 0x00-0xFF.
        void pixelpwmStateSet( uint8_t xpos , uint8_t ypos , uint8_t colour , uint8_t state );

        /// @brief Get a pixels pwm state.
        /// @param xpos The x position, with 0 on the left.
        /// @param ypos The y position, with 0 at the top.
        /// @param colour The colour bank to check. 0 = red, 1 = blue , 2 = green.
        /// @return The pwm value of the pixel, as a uint8_t. 0-255.
        uint8_t pixelpwmStateGet( uint8_t xpos , uint8_t ypos , uint8_t colour );
        


        /// @brief Clear the pixel pwm buffer to all zero.
        void pixelpwmStateBufferClear();

        /// @brief Fills the pixel pwm buffer with a given byte.
        /// @param statebyte The byte to use for fill.  0x00-0xFF.
        void pixelpwmStateBufferFill( uint8_t statebyte );










        /// @brief Writes the in-memory pixel buffer to an on-chip frame buffer.
        /// @param framenumber The frame number to write to.  0-7.
        void frameWrite( uint8_t framenumber );


        /// @brief Writes the in-memory pixel state buffer to an on-chip frame buffer.
        /// @param framenumber The frame number to write to.  0-7.
        void frameWritePixelState( uint8_t framenumber );







};










#endif




