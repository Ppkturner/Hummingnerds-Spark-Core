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
        for (int j = 0; j < TAG_SIZE; j++){
            tempTag[j] = TagData[i][j];
            //Serial.print(tempTag);
            //Serial.print("\n");
        }
        // String tempTagString = String(tempTag);
      tcp.print("GET /~data/spark.php?sUID=");
        tcp.print( Spark.deviceID() );
        tcp.print("&bUID=");
        for (int i = 0; i < TAG_SIZE; i++){
            printEachChar(tempTag[i]);
        }
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
    strcpy(tempTag, "");
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

void printEachChar(char i){
    if (i == '0') tcp.print("0");
    else if (i == '1') tcp.print("1");
    else if (i == '2') tcp.print("2");
    else if (i == '3') tcp.print("3");
    else if (i == '4') tcp.print("4");
    else if (i == '5') tcp.print("5");
    else if (i == '6') tcp.print("6");
    else if (i == '7') tcp.print("7");
    else if (i == '8') tcp.print("8");
    else if (i == '9') tcp.print("9");
    
    else if (i == 'A') tcp.print("A");
    else if (i == 'B') tcp.print("B");
    else if (i == 'C') tcp.print("C");
    else if (i == 'D') tcp.print("D");
    else if (i == 'E') tcp.print("E");
    else if (i == 'F') tcp.print("F");
    else if (i == 'G') tcp.print("G");
    else if (i == 'H') tcp.print("H");
    else if (i == 'I') tcp.print("I");
    else if (i == 'J') tcp.print("J");
    else if (i == 'K') tcp.print("K");
    else if (i == 'L') tcp.print("L");
    else if (i == 'M') tcp.print("M");
    else if (i == 'N') tcp.print("N");
    else if (i == 'O') tcp.print("O");
    else if (i == 'P') tcp.print("P");
    else if (i == 'Q') tcp.print("Q");
    else if (i == 'R') tcp.print("R");
    else if (i == 'S') tcp.print("S");
    else if (i == 'T') tcp.print("T");
    else if (i == 'U') tcp.print("U");
    else if (i == 'V') tcp.print("V");
    else if (i == 'W') tcp.print("W");
    else if (i == 'X') tcp.print("X");
    else if (i == 'Y') tcp.print("Y");
    else if (i == 'Z') tcp.print("Z");
}