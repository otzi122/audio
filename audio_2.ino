const char pinAudioLeft  = 3;
const char pinAudioRight = 2;
const char pinLed = 1;
const char pinSwitch = 1;

int speedRefresh = 500;
int averageRefresh = 5;
float audioIn = 0;

int statusSpeaker = 0;
int count = 0;

void setup()
{
  Serial.begin(9600);
  pinMode( pinLed, OUTPUT );
  blinkLed( pinLed );
}

void loop()
{
  updateStatusSpeaker();

  if( statusSpeaker == 0 ) //Speaker is off (Auto-On)
  {
    turnToggle( 5, 200, 5, 1 ); //Min, Max, Repeat, Mode
  }
  else // Speaker is On (Auto-Off)
  {
    turnToggle( 0, 5, 30, 0 ); //Min, Max, Repeat, Mode
  }
  
  Serial.print("Contador: ");
  Serial.print( count );
  Serial.print(" Audio: ");
  Serial.print( audioInAverage );
  Serial.print("\n");

  delay(speedRefresh);
}

void updateStatusSpeaker()
{
  //Get status
  //If change reset count
  //save new status
  digitalWrite( pinLed, ( statusSpeaker == 1 ) ? HIGH : LOW);
}

void blinkLed( int pintLed )
{
  for(int i=0; i < 7; i++)
  {
    digitalWrite( pintLed, ( i % 2 == 0 ) ? HIGH : LOW );
    delay(100);
  }

  digitalWrite( pintLed, LOW );
}

void turnToggle( float minValue, float maxValue, int repeat, int mode )
{
  audioIn = analogRead( pinAudio );
  if( audioIn >= minValue && audioIn <= maxValue )
    count++;
  else
    count = 0;
  

  if( count >= ( 1000 / speedRefresh ) * repeat )
  {
    statusSpeaker = mode;
    blinkLed( pinLed );
    count = 0;
  }
}