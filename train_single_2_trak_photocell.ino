// Mega Arduino
//
// 2 Track bidirectional signal controller
//

const int photo_cell_track_1_east_pin = 0;// initialize analog pin 0, connected with photovaristor
const int photo_cell_track_1_west_pin = 1;// initialize analog pin 1, connected with photovaristor
const int photo_cell_track_2_east_pin = 2;// initialize analog pin 2, connected with photovaristor
const int photo_cell_track_2_west_pin = 3;// initialize analog pin 3, connected with photovaristor

const int led_track_1_cell_1 = 9;
const int led_track_1_cell_2 = 8;
const int led_track_2_cell_1 = 7;
const int led_track_2_cell_2 = 6;

const int track_1_west_red_pin = 22;
const int track_1_west_yellow_pin = 24;
const int track_1_west_green_pin = 26;
const int track_1_west_power_pin = 28;

const int track_1_east_red_pin = 23;
const int track_1_east_yellow_pin = 25;
const int track_1_east_green_pin = 27;
const int track_1_east_power_pin = 29;


const int track_2_west_red_pin = 46;
const int track_2_west_yellow_pin = 48;
const int track_2_west_green_pin = 50;
const int track_2_west_power_pin = 52;

const int track_2_east_red_pin = 47;
const int track_2_east_yellow_pin = 49;
const int track_2_east_green_pin = 51;
const int track_2_east_power_pin = 53;

const int minDiff = 40;
const float photoCellDistance = 0.6875; // 11/16 inches
const float mileInInches = 63360;

const int delay_secs = 1000;
const int red_delay_secs = 8000;
const int yellow_delay_secs = 8000;
//const int red_delay_secs = 5000;
//const int yellow_delay_secs = 5000;

//const int encoderGroundPin = 31;
//const int encoderPowerPin = 33;
//const int encoder0PinA = 35;
//const int encoder0PinB = 37;
//const int encoderButtonPin = 39;
//const unsigned int minEncoderValue = 0;
//const unsigned int maxEncoderValue = minDiff * 2;
//
//volatile unsigned int  sensitivetyDiff = minDiff;

enum class  PhotoCellState {
  none,
  uncovered,
  covered
};

enum class PhotoPosition {
  none,
  east,
  west
};


enum class Direction {
  none,
  east,
  west
};


class photoCell
{
  private:
    int photoCellPin;
    int ledPin;
    int trackNum;
    PhotoPosition photoCellPos;

    int photocell_val = 0;// value read from photo cell pin
    int baseline_light = 0; // baseline calibration level
    int baseline_total = 0; // baseline total
    unsigned long first_photo_cell_time;
    unsigned long last_photo_cell_time;

    PhotoCellState photCell1State;
    String photoCellName;

  public:
    photoCell()
    { // deafualt empty constructor
    }

    photoCell(
      int p_photoCellPin,
      int p_ledPin,
      int p_trackNum,
      PhotoPosition p_photoCellPos)
    {
      photoCellPin = p_photoCellPin;
      ledPin = p_ledPin;
      pinMode(ledPin, OUTPUT);
      trackNum = p_trackNum;
      photoCellPos = p_photoCellPos;
      photCell1State = PhotoCellState::none;
      first_photo_cell_time = 0;
      last_photo_cell_time = 0;

      if (photoCellPos == PhotoPosition::east)
      {
        photoCellName = "East";
      }
      else if (photoCellPos == PhotoPosition::west)
      {
        photoCellName = "West";
      }
      else
      {
        photoCellName = "None";
      }

      calibratePhotoCell();
    }

    calibratePhotoCell()
    {
      int baseline_count = 1;
      String photoCellStr;
      String baselineLightStr;
      baseline_total = 0;

      while ( baseline_count <= 2)
      {
        delay(250);// wait for 0.01
        photocell_val = analogRead(photoCellPin);// read the analog value of the sensor and assign it to val
        baseline_total += photocell_val;
        baseline_light = baseline_total / baseline_count;
        //sprintf(photoCellStr, "Track %d photocell %s value: %d", trackNum, photoCellName,photocell_val);
        /*photoCellStr = "Track " ;
        photoCellStr += trackNum;
        photoCellStr += " photocell ";
        photoCellStr +=  photoCellName ;
        photoCellStr +=  " value: " ;
        photoCellStr +=  photocell_val ;*/

        //photoCellStr = "Track " + trackNum + " photocell " + photoCellName + " value: " + photocell_val;
        //Serial.println(photoCellStr );

        //sprintf(baselineLightStr, "Track %d photocell %s baseline_light: %d", trackNum, photoCellName,baseline_light);
        //baselineLightStr = "Track " + trackNum + " photocell " + photoCellName + " baseline_light: " + baseline_light;
        /*baselineLightStr = "Track " ;
        baselineLightStr += trackNum;
        baselineLightStr += " photocell ";
        baselineLightStr += photoCellName;
        baselineLightStr +=  " baseline_light: " ;
        baselineLightStr += baseline_light;
        Serial.println(baselineLightStr );*/
        baseline_count ++;
      }
    }

    void checkPhotoCellState()
    {
      /*String baselineStr;
        baselineStr = "Track " ;
        baselineStr += trackNum;
        baselineStr += " photocell ";
        baselineStr += photoCellName;
        baselineStr +=  " baseline_light: " ;
        baselineStr += baseline_light;
        Serial.println(baselineStr );*/

      photocell_val = analogRead(photoCellPin); // read the analog value of the sensor and assign it to val
      /*String photocellValStr;
        photocellValStr = "Track " ;
        photocellValStr += trackNum;
        photocellValStr += " photocell ";
        photocellValStr +=  photoCellName ;
        photocellValStr +=  " value: " ;
        photocellValStr +=  photocell_val ;
        Serial.println(photocellValStr);*/

      int brightness = photocell_val - baseline_light;
      if ( brightness > minDiff  )
      {
        /*Serial.print("Photocell COVERED: " );
          Serial.println(photoCellPin );
          Serial.print("ledPin: " );
          Serial.print(ledPin );*/
        if (first_photo_cell_time == 0)
        {
          first_photo_cell_time = millis();
        }
        last_photo_cell_time = millis();
        digitalWrite(ledPin, LOW);
        photCell1State = PhotoCellState::covered;
      }
      else
      {
        /*Serial.print("Photocell CLEAR: " );
          Serial.println(photoCellPin );
          Serial.print("ledPin: " );
          Serial.print(ledPin );*/
        digitalWrite(ledPin, HIGH);
        photCell1State = PhotoCellState::uncovered;
      }
      /*Serial.print(" photo_cell_time: " );
        Serial.print(photo_cell_time );
        Serial.println("");*/

      return ;
    }

    unsigned long getFirstPhotoCellTime()
    {
      return first_photo_cell_time;
    }
    unsigned long getLastPhotoCellTime()
    {
      return last_photo_cell_time;
    }

    void resetFirstPhotoCellTime()
    {
      first_photo_cell_time = 0;
    }

    void resetLastPhotoCellTime()
    {
      last_photo_cell_time = 0;
    }

    PhotoCellState getPhotoCellState()
    {
      return photCell1State;
    }
};

class trainSignal
{
  private:
    Direction signalDirection;
    int trackNum;
    int redPin;
    int yellowPin;
    int greenPin;
    int powerPin;
  public:
    trainSignal()
    { // default constructor
      redPin = 0;
      yellowPin = 0;
      greenPin = 0;
    }

    trainSignal(
      Direction p_signalDirection,
      int p_trackNum,
      int p_redPin,
      int p_yellowPin,
      int p_greenPin,
      int p_powerPin
    )
    { // Initialize
      signalDirection = p_signalDirection;
      trackNum = p_trackNum;
      redPin = p_redPin;
      yellowPin = p_yellowPin;
      greenPin = p_greenPin;
      powerPin = p_powerPin;

      pinMode(redPin, OUTPUT);
      pinMode(yellowPin, OUTPUT);
      pinMode(greenPin, OUTPUT);
      pinMode(powerPin, OUTPUT);
    }

    void displayRedSignal()
    {
      digitalWrite(greenPin, HIGH);
      digitalWrite(yellowPin, HIGH);
      digitalWrite(redPin, LOW);
      digitalWrite(powerPin, HIGH);
    }

    void displayYellowSignal()
    {
      digitalWrite(greenPin, HIGH);
      digitalWrite(yellowPin, LOW);
      digitalWrite(redPin, HIGH);
      digitalWrite(powerPin, HIGH);
    }

    void displayGreenSignal()
    {
      digitalWrite(greenPin, LOW);
      digitalWrite(yellowPin, HIGH);
      digitalWrite(redPin, HIGH);
      digitalWrite(powerPin, HIGH);
    }

};

class track
{
  private:
    int trackNum;
    photoCell east_photo;
    photoCell west_photo;

    trainSignal east_signal;
    trainSignal west_signal;

    Direction trainDirection;
    unsigned long last_photo_cell_time;
    unsigned long eastPhotoTime;
    unsigned long westPhotoTime;
    PhotoCellState eastPhotoCellState;
    PhotoCellState westPhotoCellState;
    PhotoCellState prev_eastPhotoCellState;
    PhotoCellState prev_westPhotoCellState;

    void calculateDirection()
    {
      eastPhotoTime = east_photo.getFirstPhotoCellTime();
      westPhotoTime = west_photo.getFirstPhotoCellTime();
      if (eastPhotoTime > 0 && westPhotoTime > 0)
      {
        if (eastPhotoTime < westPhotoTime)
        {
          trainDirection = Direction::west;
        }
        else
        {
          trainDirection = Direction::east;
        }
      }
      else
      {
        trainDirection = Direction::none;
      }

      /*Serial.print("track: " );
        Serial.println(trackNum);
        Serial.print("eastPhotoTime: " );
        Serial.println(eastPhotoTime );
        Serial.print("westPhotoTime: " );
        Serial.println(westPhotoTime );
        Serial.print(" trainDirection: " );
        switch (trainDirection)
        {
        case Direction::east:
          Serial.println("EAST" );
        case Direction::west:
          Serial.println("WEST" );
        case Direction::none:
          Serial.println("NONE" );
        }*/
    }

    void resetSignal()
    {
      last_photo_cell_time = 0;
      east_photo.resetLastPhotoCellTime();
      west_photo.resetLastPhotoCellTime();
      east_signal.displayGreenSignal();
      west_signal.displayGreenSignal();
      trainDirection = Direction::none;
    }

    void setLastCoveredTime()
    {
      //Serial.println("");
      eastPhotoTime = east_photo.getLastPhotoCellTime();
      westPhotoTime = west_photo.getLastPhotoCellTime();
      if ( eastPhotoTime > westPhotoTime  )
      {
        last_photo_cell_time = eastPhotoTime;
        //Serial.println("eastPhotoTime > westPhotoTime");
      }
      else if (westPhotoTime > eastPhotoTime)
      {
        last_photo_cell_time = westPhotoTime;
        //Serial.println("westPhotoTime > eastPhotoTime");
      }
      else if (eastPhotoTime == westPhotoTime)
      {
        last_photo_cell_time = westPhotoTime;
        //Serial.println("westPhotoTime == eastPhotoTime");
      }
      else
      {
        //Serial.println("last_photo_cell_time = 0");
        last_photo_cell_time = 0;
      }
    }

    void displaySignal()
    {
      unsigned long curTime =  millis();
      eastPhotoTime = east_photo.getLastPhotoCellTime();
      westPhotoTime = west_photo.getLastPhotoCellTime();
      eastPhotoCellState = east_photo.getPhotoCellState();
      westPhotoCellState = west_photo.getPhotoCellState();


      /*Serial.println("");
      Serial.print("curTime: ");
      Serial.println(curTime);
      Serial.print("eastPhotoTime: ");
      Serial.println(eastPhotoTime);
      Serial.print("westPhotoTime: ");
      Serial.println(westPhotoTime);
      Serial.print("last_photo_cell_time: ");
      Serial.println(last_photo_cell_time);
      delay(1000);*/

      
      if ( eastPhotoCellState == PhotoCellState::covered || westPhotoCellState == PhotoCellState::covered)
      {  // One of the photos cell is covered always display red
        east_signal.displayRedSignal();
        west_signal.displayRedSignal();
      }
      else if (eastPhotoCellState == PhotoCellState::uncovered && westPhotoCellState == PhotoCellState::uncovered)
      { // check state of signal
        if ( eastPhotoTime == 0 && westPhotoTime == 0)
        {
          east_signal.displayGreenSignal();
          west_signal.displayGreenSignal();
        }
        else
        {
          // if direction is east set west bound green and east bound based on delay
          if ( trainDirection == Direction::east)
          {
            if ( curTime < last_photo_cell_time + red_delay_secs)
            {
              west_signal.displayGreenSignal();
              east_signal.displayRedSignal();
            }
            else if ( curTime < last_photo_cell_time + red_delay_secs + yellow_delay_secs)
            {
              west_signal.displayGreenSignal();
              east_signal.displayYellowSignal();
            }
            else
            {
              resetSignal();
            }
          }
          // if direction is west set east bound green and west bound based on delay
          else if ( trainDirection == Direction::west)
          {
            if ( curTime < last_photo_cell_time + red_delay_secs)
            {
              west_signal.displayRedSignal();
              east_signal.displayGreenSignal();
            }
            else if ( curTime < last_photo_cell_time + red_delay_secs + yellow_delay_secs)
            {
              west_signal.displayYellowSignal();
              east_signal.displayGreenSignal();
            }
            else
            {
              resetSignal();
            }

          }
          else if ( trainDirection == Direction::none)
          {
            resetSignal();
          }
        }
      }

      // Store previous Photo Cell state
      prev_eastPhotoCellState = eastPhotoCellState;
      prev_westPhotoCellState = westPhotoCellState;
    }

  public:
    track()
    {
      trainDirection = Direction::none;
      last_photo_cell_time = 0;
    }

    track(
      int p_trackNum,
      int p_photoCellEastPin, int p_eastLedPin,
      int p_east_red_pin, int p_east_yellow_pin, int p_east_green_pin, int p_east_power_pin,
      int p_photoCellWastPin, int p_wastLedPin,
      int p_west_red_pin, int p_west_yellow_pin, int p_west_green_pin, int p_west_power_pin
    )
    {
      trackNum = p_trackNum;
      trainDirection = Direction::none;
      last_photo_cell_time = 0;
      east_photo = photoCell(p_photoCellEastPin, p_eastLedPin, p_trackNum, PhotoPosition::east);
      west_photo = photoCell(p_photoCellWastPin, p_wastLedPin, p_trackNum, PhotoPosition::west);
      east_signal = trainSignal(Direction::east, 1, p_east_red_pin, p_east_yellow_pin, p_east_green_pin, p_east_power_pin);
      west_signal = trainSignal(Direction::west, 1, p_west_red_pin, p_west_yellow_pin, p_west_green_pin, p_west_power_pin);
    }

    void checkTrack()
    {
      east_photo.checkPhotoCellState();
      west_photo.checkPhotoCellState();
      if ( trainDirection == Direction::none)
      {
        calculateDirection();
      }
      if (prev_eastPhotoCellState == PhotoCellState::covered && east_photo.getPhotoCellState() == PhotoCellState::uncovered)
      {
        east_photo.resetFirstPhotoCellTime();
        //calculateDirection();
      }
      if (prev_westPhotoCellState == PhotoCellState::covered && west_photo.getPhotoCellState() == PhotoCellState::uncovered)
      {
        west_photo.resetFirstPhotoCellTime();
        //calculateDirection();
      }
      setLastCoveredTime();
      displaySignal();
    }
};

track track_1;
track track_2;
//rotaryEncoder rotary1;
void setup()
{
  Serial.begin(9600);// set baud rate at “9600”
  Serial.println("" );
  track_1 = track(
              1,
              photo_cell_track_1_east_pin, led_track_1_cell_1,
              track_1_east_red_pin, track_1_east_yellow_pin, track_1_east_green_pin, track_1_east_power_pin,
              photo_cell_track_1_west_pin, led_track_1_cell_2,
              track_1_west_red_pin, track_1_west_yellow_pin, track_1_west_green_pin, track_1_west_power_pin
            );
  track_2 = track(
              2,
              photo_cell_track_2_east_pin, led_track_2_cell_1,
              track_2_east_red_pin, track_2_east_yellow_pin, track_2_east_green_pin, track_2_east_power_pin,
              photo_cell_track_2_west_pin, led_track_2_cell_2,
              track_2_west_red_pin, track_2_west_yellow_pin, track_2_west_green_pin, track_2_west_power_pin
            );
  //  rotary1 = rotaryEncoder (
  //      encoderGroundPin, encoderPowerPin, encoder0PinA,
  //      encoder0PinB, encoderButtonPin);

  Serial.println("Setup Complete");
}

void loop()
{
  track_1.checkTrack();
  track_2.checkTrack();

  //Serial.print("sensitivetyDiff: ");
  //Serial.println(sensitivetyDiff);
  //delay(3000);
}
