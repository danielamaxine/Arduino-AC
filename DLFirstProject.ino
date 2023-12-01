#include <dht_nonblocking.h>
#include <LiquidCrystal.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11
#define ENABLE 6
#define DIRA 4
#define DIRB 5

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
/*
 * Initialize the serial port.
 */
void setup( )
{
  Serial.begin( 9600);
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("sensing...");
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
}
/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );
  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }
  return( false );
}
/*
 * Main program loop.
 */
void loop( )
{
  float temperature;
  float humidity;
  float fahrenheit;

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true )
  {
    fahrenheit = (((temperature*9)/5)+32);
    Serial.print( "T = " );
    Serial.print( fahrenheit, 1 );
    Serial.println( "%" );
    lcd.print( "temp: " );
    lcd.print( fahrenheit );
    if (fahrenheit > 78)
    {
      Serial.println( "TURN FAN ON" );
      digitalWrite(ENABLE,HIGH); // enable on
      digitalWrite(DIRA,HIGH); //one way
      digitalWrite(DIRB,LOW);
    }
    else
    {
      Serial.println( "turn it off :(" );
      digitalWrite(ENABLE,LOW); //all done
    }
  }
}
