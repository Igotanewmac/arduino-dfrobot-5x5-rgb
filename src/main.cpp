#define IS31FL3731_I2C_ADDRESS 0x74


#include <Arduino.h>

#include <Wire.h>



// include the library code:
#include <LiquidCrystal.h>

// dfrobot sheild pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


// i2c library
#include <Wire.h>
#define wire Wire



// include my IS31FL3731 library
#include <IS31FL3731.h>

IS31FL3731 myisobs;



//#include <pimoroni_11x7matrix.h>

#include <Pimoroni_5x5_rgb_matrix.h>






// define the button pin

#define BUTTONPIN A0







uint8_t safestepxpos = 0;
uint8_t safestepypos = 0;
uint8_t safestepzpos = 0;

void safestep( int8_t xoffset , int8_t yoffset , int8_t zoffset ) {
  if ( ( xoffset == -1 ) && ( safestepxpos > 0 ) ) { safestepxpos--; }
  if ( ( xoffset ==  1 ) && ( safestepxpos < 4 ) ) { safestepxpos++; }
  if ( ( yoffset == -1 ) && ( safestepypos > 0 ) ) { safestepypos--; }
  if ( ( yoffset ==  1 ) && ( safestepypos < 4 ) ) { safestepypos++; }
  if ( ( zoffset == -1 ) && ( safestepzpos > 0 ) ) { safestepzpos--; }
  if ( ( zoffset ==  1 ) && ( safestepzpos < 2 ) ) { safestepzpos++; }
  return;
}






// define the menu options
int8_t menucurrentchoice = 0;

uint8_t menumaxchoices = 7;

String menutext[7] = { "I2C Scan" ,
                       "Test 1" ,
                       "Test 2" ,
                       "Test 3" ,
                       "Test 4" ,
                       "Test 5" ,
                       "Test 6"
                       };




// i2c scanner
void menucommand_00() {

  lcd.clear();

  // turn on i2c as a master
  wire.begin();

  for ( int i = 0 ; i < 127 ; i++ ) {

    // check for a connection
    wire.beginTransmission( i );

    if ( wire.endTransmission() == 0 ) {
      // device is present at this address!
      if ( i < 16 ) { lcd.print("0" ); }
      lcd.print( i , HEX );
    }
    
  }

  // done so hold here
  while (1);
  
}







// Test 1
void menucommand_01() {
  
  // bring up i2c as a master
  wire.begin();

  lcd.clear();
  lcd.setCursor( 0 , 0 );
  lcd.print( "Test 1" );
  

  // create the object.
  Pimoroni_5x5_rgb_matrix myledmatrix( IS31FL3731_I2C_ADDRESS );

  // initialise the chip.
  myledmatrix.begin( IS31FL3731_I2C_ADDRESS );

  // turn it on
  myledmatrix.softwareShutdownSet( 1 );
  
  // switch to static picture mode.
  myledmatrix.displayModeSet( 0b00 );

  // switch to frame 0
  myledmatrix.frameDisplayPointerSet( 0 );

  

  // try out some pixel commands
  
  myledmatrix.pixelpwmStateBufferFill( 0x40 );
  myledmatrix.frameWrite( 0 );


  while (1) {
    for ( uint8_t colour = 0 ; colour < 3 ; colour++ ) {
      for ( uint8_t xpos = 0 ; xpos < 5 ; xpos++ ) {
        for ( uint8_t ypos = 0 ; ypos < 5 ; ypos++ ) {
          myledmatrix.pixelStateBufferClear();
          myledmatrix.pixelSet( xpos , ypos , colour , 1 );
          myledmatrix.frameWritePixelState( 0 );
          //delay( 100 );
        }
      }
    }
  }









  
}



void menucommand_02() {

  // bring up the wire library as a master
  wire.begin();

  // move cursor to home
  lcd.clear();
  lcd.setCursor( 0 , 0 );
  lcd.print( "Test 2" );


  // create the object.
  Pimoroni_5x5_rgb_matrix myledmatrix( IS31FL3731_I2C_ADDRESS );

  // initialise the chip.
  myledmatrix.begin( IS31FL3731_I2C_ADDRESS );

  // turn it off
  myledmatrix.softwareShutdownSet( 1 );

  // switch to static picture mode.
  myledmatrix.displayModeSet( 0b00 );

  // switch to frame 0
  myledmatrix.frameDisplayPointerSet( 0 );

  /*
    // switch bank to 0
  wire.beginTransmission( 0x74 );
  wire.write( 0xFD );
  wire.write( 0x00 );
  wire.endTransmission();

  // turn down the pwm
  for ( int i = 0x24 ; i < 0xB4 ; i++ ) {
    wire.beginTransmission( 0x74 );
    wire.write( i );
    wire.write( 0x0F );
    wire.endTransmission();
  }
  */
  myledmatrix.pixelpwmStateBufferFill( 0x0F );


  while (1) {

    for ( int i = 0 ; i < 0b100000 ; i++ ) {
      myledmatrix.pixelStateBufferFill( i );
      myledmatrix.pixelpwmStateBufferFill( i + 1 );
      myledmatrix.frameWrite( 0 );
      delay( 1000 );
    }


  }
  
  
  }
















void menucommand_03() {

    // bring up the wire library as a master
  wire.begin();

  // move cursor to home
  lcd.clear();
  lcd.setCursor( 0 , 0 );

  lcd.print( "Test 3" );


  // create the object.
  Pimoroni_5x5_rgb_matrix myledmatrix( IS31FL3731_I2C_ADDRESS );

  // initialise the chip.
  myledmatrix.begin( IS31FL3731_I2C_ADDRESS );

  // turn it off
  myledmatrix.softwareShutdownSet( 1 );

  // switch to static picture mode.
  myledmatrix.displayModeSet( 0b00 );

  // switch to frame 0
  myledmatrix.frameDisplayPointerSet( 0 );

    // switch bank to 0
  wire.beginTransmission( 0x74 );
  wire.write( 0xFD );
  wire.write( 0x00 );
  wire.endTransmission();

  // turn down the pwm
  for ( int i = 0x24 ; i < 0xB4 ; i++ ) {
    wire.beginTransmission( 0x74 );
    wire.write( i );
    wire.write( 0x0F );
    wire.endTransmission();
  }


  myledmatrix.pixelStateBufferClear();
  myledmatrix.pixelpwmStateBufferFill( 0x04 );
  myledmatrix.frameWrite(0);



  while (1) {
    
    // turn off the old pixel
    myledmatrix.pixelSet( safestepxpos , safestepypos , safestepzpos , 0 );

    // do the actual move
    // slow it down to only one direction at a time...
    switch (random(3))
    {
    case 0:
      safestep( random( -1 , 2 ) , 0 , 0 );
      break;
    case 1:
      safestep( 0 , random( -1 , 2 ) , 0 );
      break;
    case 2:
      safestep( 0 , 0 , random( -1 , 2 ) );
      break;
    
    default:
      break;
    }


    //safestep( random( -1 , 2 ) , random( -1 , 2 ) , random( -1 , 2 ) );

    // now turn on the new pixel
    myledmatrix.pixelSet( safestepxpos , safestepypos , safestepzpos , 1 );

    // update the screen
    myledmatrix.frameWrite(0);

    delay( 100 );


  }
  
  
  
};

























void menucommand_04() {

  // bring up the wire library as a master
  wire.begin();

  // move cursor to home
  lcd.clear();
  lcd.setCursor( 0 , 0 );

  lcd.print( "Test 4" );


  // create the object.
  Pimoroni_5x5_rgb_matrix myledmatrix( IS31FL3731_I2C_ADDRESS );

  // initialise the chip.
  myledmatrix.begin( IS31FL3731_I2C_ADDRESS );

  // turn it off
  myledmatrix.softwareShutdownSet( 1 );

  // switch to static picture mode.
  myledmatrix.displayModeSet( 0b00 );

  // switch to frame 0
  myledmatrix.frameDisplayPointerSet( 0 );

  myledmatrix.pixelStateBufferClear();
  myledmatrix.frameWrite(0);


  myledmatrix.pixelStateBufferFill( 0xFF );
  myledmatrix.pixelpwmStateBufferClear();


  while (1) {

    uint8_t counter = 0;

    do {

      
      counter++;

      myledmatrix.pixelpwmStateBufferFill( counter );
      myledmatrix.frameWrite( 0 );
      //delay( 256 - counter );

    } while ( counter != 0xFF );

    //delay( 1000 );

    do {

      
      counter--;
      myledmatrix.pixelpwmStateBufferFill( counter );
      myledmatrix.frameWrite(0);

      //delay( 256 - counter );

    } while ( counter != 0x00 );

   // delay( 1000 );


  }


  
};









void menucommand_05() {


// bring up the wire library as a master
  wire.begin();

  // move cursor to home
  lcd.clear();
  lcd.setCursor( 0 , 0 );

  lcd.print( "Test 5 - Active" );


  // create the object.
  Pimoroni_5x5_rgb_matrix myledmatrix( IS31FL3731_I2C_ADDRESS );

  // initialise the chip.
  myledmatrix.begin( IS31FL3731_I2C_ADDRESS );

  // turn it off
  myledmatrix.softwareShutdownSet( 1 );

  // switch to static picture mode.
  myledmatrix.displayModeSet( 0b00 );

  // switch to frame 0
  myledmatrix.frameDisplayPointerSet( 0 );

  myledmatrix.blinkEnableSet( 1 );
  myledmatrix.blinkPeriodTimeSet( 1 );

  myledmatrix.pixelStateBufferFill( 0xFF );
  myledmatrix.pixelpwmStateBufferFill( 0x04 );
  myledmatrix.pixelBlinkStateBufferFill( 0b10101 );
  //myledmatrix.pixelBlinkStateBufferClear();
  myledmatrix.frameWrite(0);

  while (1);

};








void menucommand_06() {


// bring up the wire library as a master
  wire.begin();

  // move cursor to home
  lcd.clear();
  lcd.setCursor( 0 , 0 );

  lcd.print( "Test 6 - Active" );

  lcd.setCursor( 0 , 1 );
  lcd.print( "Twinkles!" );


  // create the object.
  Pimoroni_5x5_rgb_matrix myledmatrix( IS31FL3731_I2C_ADDRESS );

  // initialise the chip.
  myledmatrix.begin( IS31FL3731_I2C_ADDRESS );

  // turn it off
  myledmatrix.softwareShutdownSet( 1 );

  // switch to static picture mode.
  myledmatrix.displayModeSet( 0b00 );

  // switch to frame 0
  myledmatrix.frameDisplayPointerSet( 0 );

  //myledmatrix.blinkEnableSet( 1 );
  //myledmatrix.blinkPeriodTimeSet( 1 );

  myledmatrix.pixelStateBufferFill( 0xFF );
  myledmatrix.pixelpwmStateBufferFill( 0x00 );
  myledmatrix.pixelBlinkStateBufferClear();
  //myledmatrix.pixelBlinkStateBufferClear();
  myledmatrix.frameWrite(0);
    

  while (1) {

    uint8_t xpos = random( 0 , 5 );
    uint8_t ypos = random( 0 , 5 );

    // get our red pwm value
    uint8_t pwmvalue = myledmatrix.pixelpwmStateGet( xpos , ypos , 0 );

    // change it a little
    int8_t randomvalue = random( -1 , 2 );

    switch (randomvalue)
    {
    case -1:
      if ( pwmvalue > 0 ) { pwmvalue--; }
      break;
    case  0:
      break;
    case  1:
      if ( pwmvalue < 0x10 ) { pwmvalue++; }
      break;
    
    default:
      break;
    }
    
    // now write it back...
    myledmatrix.pixelpwmStateSet( xpos , ypos , 0 , pwmvalue );
    myledmatrix.pixelpwmStateSet( xpos , ypos , 1 , pwmvalue );
    myledmatrix.pixelpwmStateSet( xpos , ypos , 2 , pwmvalue );

    // now update the frame
    myledmatrix.frameWrite( 0 );


  }


};


































void domenuaction() {

  lcd.setCursor( 0 , 1 );
  lcd.print( "Executing" );
  delay( 750 );

  switch (menucurrentchoice)
  {
  case 0:
    menucommand_00();
    break;
  
  case 1:
    menucommand_01();
    break;

  case 2:
    menucommand_02();
    break;
  
  case 3:
    menucommand_03();
    break;
  
  case 4:
    menucommand_04();
    break;
  case 5:
    menucommand_05();
    break;
  case 6:
    menucommand_06();
    break;
  
  default:
    break;
  }

  return;

}




void displaymenu() {

  lcd.clear();
  lcd.setCursor( 0 , 0 );

  lcd.print( menutext[ menucurrentchoice ] );

}




// button scores
// none   1023
// right  0
// up     131
// down   308
// left   481
// select 720



void checkbutton() {

  int buttonraw = analogRead( BUTTONPIN );

  if ( buttonraw > 1000 ) { /* no button is pressed */ return; }

  if ( buttonraw < 100 ) { /* right button is pressed */ }

  if ( ( buttonraw > 100 ) && ( buttonraw < 200 ) ) { /* up button is pressed */ menucurrentchoice++; }
  if ( ( buttonraw > 250 ) && ( buttonraw < 350 ) ) { /* down button is pressed */ menucurrentchoice--; }
  if ( ( buttonraw > 450 ) && ( buttonraw < 500 ) ) { /* left button is pressed */ }
  if ( ( buttonraw > 700 ) && ( buttonraw < 750 ) ) { /* select button is pressed */ domenuaction(); }
  


  if ( menucurrentchoice == menumaxchoices ) { menucurrentchoice = ( menumaxchoices - 1 ); }
  if ( menucurrentchoice < 0 ) { menucurrentchoice = 0; }

  displaymenu();

}












void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  // set the buttonpin to input
  pinMode( BUTTONPIN , INPUT );

  Serial.begin( 9600 );

  // show the menu screen
  displaymenu();

}






void loop() {

  checkbutton();

  //lcd.clear();
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  // lcd.print( analogRead( BUTTONPIN ) );

  delay( 500 );


}








































//