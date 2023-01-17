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



// #include <Pimoroni_5x5_rgb_matrix.h>






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
  
  IS31FL3731 myisobj;

  myisobj.begin( IS31FL3731_I2C_ADDRESS );

  myisobj.softwareShutdownSet( 1 );

  myisobj.displayModeSet( 0b00 );

  myisobj.frameDisplayPointerSet( 0 );

  
  // create a blank buffer
  uint8_t testbuffer[ 18 ] = { 0};
  for ( uint8_t i = 0 ; i < 18 ; i++ ) {
    testbuffer[ i ] = 0xFF;
  }
  myisobj.frameWriteState( 0 , testbuffer );


  // blink buffer
  uint8_t testblinkbuffer[ 18 ] = { 0 };
  for ( uint8_t i = 0 ; i < 18 ; i++ ) {
    testblinkbuffer[ i ] = 0x00;
  }
  myisobj.frameWriteBlink( 0 , testblinkbuffer );




  // create a pwm buffer
  uint8_t testpwmbuffer[ 18 * 8 ] = { 0 };
  for ( uint8_t i = 0 ; i < ( 18 * 8 ) ; i++ ) {
    testpwmbuffer[ i ] = 0x04;
  }
  myisobj.frameWritepwm( 0 , testpwmbuffer );



  lcd.setCursor( 0 , 1 );
  lcd.print( "Done!" );
  delay( 1000 );
  return;

}




















void menucommand_02() {

  // bring up the wire library as a master
  wire.begin();

  // move cursor to home
  lcd.clear();
  lcd.setCursor( 0 , 0 );
  lcd.print( "Test 2" );


  
}
















void menucommand_03() {

    // bring up the wire library as a master
  wire.begin();

  // move cursor to home
  lcd.clear();
  lcd.setCursor( 0 , 0 );

  lcd.print( "Test 3" );


  
  
  
};

























void menucommand_04() {

  // bring up the wire library as a master
  wire.begin();

  // move cursor to home
  lcd.clear();
  lcd.setCursor( 0 , 0 );

  lcd.print( "Test 4" );


  

  
};









void menucommand_05() {


// bring up the wire library as a master
  wire.begin();

  // move cursor to home
  lcd.clear();
  lcd.setCursor( 0 , 0 );

  lcd.print( "Test 5 - Active" );


  

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