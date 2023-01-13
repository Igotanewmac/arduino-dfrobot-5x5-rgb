
#ifndef IS31FL3731_HEADER_GUARD
#define IS31FL3731_HEADER_GUARD



// IS31FL3731 library header

// include Arduino header
#include <Arduino.h>

// Wire library
#include <Wire.h>
#define wire Wire








// a whole bunch of definitions

// the animation page that refers to the control register
#define IS31FL3731_PAGE_CONTROL 0x0B

#define IS31FL3731_ADDRESS_CONFIG_REGISTER 0x00
#define IS31FL3731_ADDRESS_PICTURE_DISPLAY_REG 0x01
#define IS31FL3731_ADDRESS_SOFTWARESHUTDOWN 0x0A











class IS31FL3731 {
private:
    /// @brief i2c address of chip
    uint8_t _i2c_address;



public:
    /// @brief Constructor for IS31FL3731 LED matrix driver.
    /// @param address I2C address of the chip.
    IS31FL3731();


    /// @brief Set the i2c address of the chip.
    /// @param _i2c_address The i2c address of the chip.
    void i2caddressset( uint8_t new_i2c_address );

    /// @brief  Returns the current i2c address as a uint8_t
    uint8_t i2caddressget();




    /// @brief Write a byte to the chip
    /// @param page The page to write to, 0x0-7 animation, 0xB control.
    /// @param address The address within the page to write to.
    /// @param data The data to write
    void write( uint8_t page , uint8_t address , uint8_t data );

    /// @brief Read a byte from the chip
    /// @param page The page to read from.  0x0-7 animation, 0xB control.
    /// @param address The address within the page to write to.
    uint8_t read( uint8_t page , uint8_t address );




    
    /// @brief Sets the software shutdown state to 0 ( shutdown ) or 1 ( normal operation ). 
    /// @param state 0 or 1. 0 is shutdown, 1 is normal operation.
    void shutdownset( uint8_t state );

    /// @brief Retreives the current software shutdown state.
    uint8_t shutdownget();


    /// @brief Set the mode in the configuration register.
    /// @param mode 0b00 = picture mode, 0b01 = auto frame play, 0b1x = audio frame play.
    void modeset( uint8_t mode );
    
    /// @brief Gets the current mode from the chip.
    uint8_t modeget();


    /// @brief Set the current frame number to display.
    /// @param frame the number of the frame to display.  0-7.
    void frameset( uint8_t frame );

    /// @brief returns the current frame number from the chip.
    uint8_t frameget();







    




};








#endif
