

// driver class for pimoroni 5x5 rgb matrix






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
                                                            {  5 , 5 } , { 13 , 7 } , { 13 , 1 } , { 13 , 2 } , { 13 , 2 }
                                                        }
                                                    }
};






class Pimoroni_5x5_rgb_matrix {
    
    private:

        uint8_t _i2c_address = 0x74;



        /// @brief The last frame number we switched to.  0-7, 0b for control page.
        uint8_t _currentframe;

        /// @brief Switch to a different frame, if necessary.
        /// @param framenumber The frame number to switch to.
        void _switchFrame( uint8_t framenumber );

    public:

        Pimoroni_5x5_rgb_matrix( uint8_t new_i2c_address );
        ~Pimoroni_5x5_rgb_matrix();
        
};







