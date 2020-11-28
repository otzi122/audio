const int pinAudio [2] = { 3, 4 };
const int pinLed = 1;

int audioChannels = sizeof( pinAudio ) / sizeof( pinAudio[0] );

int   readIndex = 0;
float readings[100];
float total = 0;
float audioInAverage = 0;

int autoOnVal = 2;
int autoOnSec = 3;
int autoOffVal = 0;
int autoOffSec = 60;

int speedRefresh = 500;
int averageRefresh = 5;

int statusSpeaker = 0;
int audioOnCount = 0;
int audioOffCount = 0;

void setup()
{
  pinMode( pinLed, OUTPUT );

  Serial.begin( 9600 );
}

void loop()
{
  getStatusSpeaker();

  if( statusSpeaker == 0 )
  {
    readAudio( averageRefresh );
    audioOnCount = turnToggle( autoOnVal, autoOnSec, audioOnCount, 1 );
  }
  else
  {
    readAudio( averageRefresh );
    audioOffCount = turnToggle( autoOffVal, autoOffSec, audioOffCount, 0 );
  }

  Serial.print("Audio: ");
  Serial.print( audioInAverage );
  Serial.print("\n");

  delay(speedRefresh);
}

void getStatusSpeaker()
{
  if( statusSpeaker == 1 )
  {
    digitalWrite( pinLed, HIGH);
  }
  else
  {
    digitalWrite( pinLed, LOW);
  }
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

int turnToggle( float value, int repeat, int count, int mode )
{
  if( audioInAverage >= value && mode == 1 || audioInAverage <= value && mode == 0 )
  {
    count++;
  }
  else
  {
    count = 0;
  }

  if( count >= ( 1000 / speedRefresh ) * repeat )
  {
    statusSpeaker = mode;
    blinkLed( pinLed );
    /*analogWrite(A3, map(1, 0, 5, 0, 255));*/
  }

  Serial.print("Contador: ");
  Serial.print( count );
  Serial.print("\n");

  return count;
}

void readAudio( int numReadings )
{
  for(int i = 0; i < audioChannels ; i++)
  {
    total = total - readings[readIndex];
    readings[readIndex] = analogRead( pinAudio[i] );
    total = total + readings[readIndex];
    readIndex = readIndex + 1;

    if ( readIndex >= ( numReadings * audioChannels) )
    {
      readIndex = 0;
    }

    audioInAverage = total / ( numReadings * audioChannels );
  }
}
