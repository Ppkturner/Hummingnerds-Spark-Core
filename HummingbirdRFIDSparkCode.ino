/* Hummingbird RFID Feeder Project (ECS 193AB)
 * Team: Hummingnerds      (2015/01 - 2015/06)
 * Team Members: Arjun, Dustin, Huu, Scott
 * Copyright (c) 2015 Hummingnerds
 */

#define TAG_SIZE 14
#define MAX_TAGS 20

TCPClient tcp;

//protos
void PrintTag();
void PrintAll();
void ReadTag();
void TransmitData();
void CheckTag();
void ClearAndReset();


//globals
bool NeedToUpload = FALSE;
char RXDataStr[ TAG_SIZE + 1 ] = {0};
char TagData[ MAX_TAGS ][ TAG_SIZE + 1 ] = {0}; // we can store 20 tags max
char tempTag[TAG_SIZE + 1] = {0};
int LiveTags = 0;

void setup()  {
  Serial.begin(9600); //USB / Console
  Serial1.begin(9600); //TX/RX

  // Now open your Serial Terminal, and hit any key to continue!
  //while( !Serial.available() ) { SPARK_WLAN_Loop(); }
  //char test[ TAG_SIZE + 1 ] = {"ABCDEFG123"};

  delay(5000); //Slight delay on boot...
}

void loop() {
  ReadTag();

  if( NeedToUpload == TRUE ) {
      TransmitData();
  } //delay(250); // allow rx buff to finish

  if( LiveTags > 10 ) {
    //WE REALLY DO NEED TO UPLOAD THOUGH...
    PrintAll();
  }
}

void PrintAll() { //for debugging
  Serial.println("\n\n");
  Serial.println("Printing All Tags:");
  for( int i = 0; i < LiveTags; i++ ) {
    Serial.print("#");
    Serial.print(i);
    Serial.print("  ");
    Serial.println(TagData[ i ]);
  }
  for(;;){} //STOP HERE...
}

void CheckTag() {
  if( LiveTags == 0 ) { //No tags stored yet..
    for( int j = 0; j < TAG_SIZE; j++ ) {
      TagData[ 0 ][ j ] = RXDataStr[ j ];
      RXDataStr[ j ] = 0; //Clear this spot..
    } //end for
    Serial.println("First Tag Detected.");
    LiveTags++;
    NeedToUpload = TRUE; 
  } else { //Check tags...
    bool IsUnique = true;

    for( int i = 0; i < LiveTags; i++) {
      if( strcmp(TagData[ i ], RXDataStr) == 0 ){
        IsUnique = false;
        Serial.println("Duplicate Tag Detected.");
        break;
      } //end if
    } //end for

    if( IsUnique == true ){
      Serial.print("New Tag Detected: ");
      for( int j = 0; j < TAG_SIZE; j++ ) {
        TagData[ LiveTags + 1 ][ j ] = RXDataStr[ j ];
        RXDataStr[ j ] = 0; //Clear this spot..
      } //end for
      Serial.println(TagData[ LiveTags + 1 ]);
      LiveTags++;

      //we had a bug with no transmit, so this is our quick patch:
      NeedToUpload = TRUE; 
    } //END: if( IsUnique == true )
  }//end else-if
}

void ReadTag(){
  if( Serial1.available() > 0 ){
    delay(100); // delay for rx buff

    for( int i = 0 ; i < TAG_SIZE ; i++ ){ //Read from RX
      RXDataStr[i] = Serial1.read();
    }

    //these two lines are for debugging
    Serial.print("Tag UID: ");
    Serial.print(RXDataStr);
    Serial.println();

    Serial1.flush(); //clear the shit to stop a half or double read
    //NeedToUpload = TRUE;

    CheckTag(); //See if its a unique tag or not..
  }
}

void TransmitData() {
  Serial.print("Connecting...  ");

  if( tcp.connect("rfid.hummingbirdhealth.org", 80) ) {
    Serial.print("Connected...  ");
    
    
    
    for( int i = 0; i < LiveTags; i++ ) {
        for (int j = 0; j < TAG_SIZE + 1; j++){
            tempTag[i] = TagData[i][j];
            Serial.print(tempTag[i]);
        }
      tcp.print("GET /~data/spark.php?bUID=");
      Serial.print(tempTag);
        tcp.print(tempTag);
        tcp.print("&sUID=");
        tcp.print( Spark.deviceID() );
        tcp.println(" HTTP/1.0");
      tcp.println("Host: rfid.hummingbirdhealth.org");
      tcp.println("Content-Length: 0");
      tcp.println();

      Serial.print("HAR HAR HAR HAR!!!! ");

      delay(1000);

      Serial.println("  ...  PLEASE?????? Done.");
    }

    //We need to clear stuff and reset things..
    if( LiveTags != 0 ) {
      for( int i = 0; i < MAX_TAGS; i++ ) {
        strcpy(TagData[ i ], "");
      }
    }
    strcpy(RXDataStr, "");
    NeedToUpload = FALSE;
    LiveTags = 0;

    //this still does not always work...
    Serial1.flush(); // make sure serial is flushed??
  } else {
    Serial.println("Connection failed.");
  }
  tcp.stop();
}
