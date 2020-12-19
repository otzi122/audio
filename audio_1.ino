int leftPin = A0, rightPin = A1; 
int pinLed = 5;
int optoPin = 2;
int dropDelay = 5;                                        // hold time before dropping the leds
float dropFactor = .94;                                   // value for dropping the leds
int swapLeftRight = false;                                // swap the left and right input values or not

int leftValue = 0, rightValue = 0, maxReadValue = 0;
int leftValueN = 0, rightValueN = 0;
int leftAnalogValue = 0, rightAnalogValue = 0;
int audioInAverage = 0;
int leftPeak = 0, rightPeak = 0;
int leftPeakTime = 0, rightPeakTime = 0;
int prevLeftValue = 0, prevRightValue = 0;

int prevLeftAnalogValue = 0, prevRightAnalogValue = 0;
int leftDropTime, rightDropTime;

int statusSpeaker = 0;
int count = 0;

void setup() {
  Serial.begin(9600);
  pinMode( pinLed, OUTPUT );
  pinMode( optoPin, OUTPUT );
  blinkLed( pinLed );
}

void loop() {

  updateStatusSpeaker();
  readValues();
  storePrevValues();

  if( statusSpeaker == 0 ) //Speaker is off (Auto-On)
  {
    turnToggle( 5, 200, 5, 1 ); //Min, Max, Repeat, Mode
  }
  else // Speaker is On (Auto-Off)
  {
    turnToggle( 0, 5, 30, 0 ); //Min, Max, Repeat, Mode
  }

  delay( 100 );
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
  digitalWrite( pinLed, LOW );
}

void readValues() {
  
  leftAnalogValue = analogRead(leftPin);
  rightAnalogValue = analogRead(rightPin);

  if (leftAnalogValue < prevLeftAnalogValue) {
    leftDropTime++;
    if (leftDropTime > dropDelay) {
      leftAnalogValue = prevLeftAnalogValue * dropFactor;
      leftDropTime = 0;
    }
    else
      leftAnalogValue = prevLeftAnalogValue;
  }
   
  if (rightAnalogValue < prevRightAnalogValue) {
    rightDropTime++;
    if (rightDropTime > dropDelay) {
      rightAnalogValue = prevRightAnalogValue * dropFactor;
      rightDropTime = 0;
    }
    else
      rightAnalogValue = prevRightAnalogValue;
  }
  

  Serial.print(leftAnalogValue);
  Serial.print(" ");
  Serial.println(rightAnalogValue);
  Serial.print(" ");
  Serial.println(count);
}

void storePrevValues() {
  prevLeftAnalogValue = leftAnalogValue;
  prevRightAnalogValue = rightAnalogValue;

  prevLeftValue = leftValue;
  prevRightValue = rightValue;
}

void turnToggle( float minValue, float maxValue, int repeat, int mode )
{
  audioInAverage = ( leftAnalogValue + rightAnalogValue ) / 2 ;

  if( audioInAverage >= minValue && audioInAverage <= maxValue )
    count++;
  else
    count = 0;
  
  if( count >=  10 * repeat )
  {
    statusSpeaker = mode;
    digitalWrite(optoPin, HIGH);
    delay(1500);
    digitalWrite(optoPin, LOW);
    blinkLed( pinLed );
    count = 0;
  }
} 