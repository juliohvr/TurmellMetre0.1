
  // This example code is in the public domain.

  import processing.serial.*;

  int bgcolor;           // Background color
  int fgcolor;           // Fill color
  Serial myPort;                       // The serial port
  int[] serialInArray = new int[38];    // Where we'll put what we receive
  int serialCount = 0;                 // A count of how many bytes we receive
  int m1s1, m1s2, m1s3, m2s1, m2s2, m3s1, m3s2;                // Draw wire sensors binary values
  int m1l1, m1l2, m1l3, m2l1, m2l2, m3l1, m3l2;                // Draw wire distances in mm
  int rotX1, rotY1, rotZ1, gForceX1, gForceY1, gForceZ1;   //MPU1
  int rotX2, rotY2, rotZ2, gForceX2, gForceY2, gForceZ2;  //MPU2
  int scale, offset, scale10; //scale and offset for the stage
  int ax1Off, ay1Off, az1Off, gx1Off, gy1Off, gz1Off; // offsets for the MPU1
  int ax2Off, ay2Off, az2Off, gx2Off, gy2Off, gz2Off; // offsets for the MPU2
  boolean firstContact = false;        // Whether we've heard from the microcontroller

  void setup() {
    size(700, 512);  // Stage size
    noStroke();      // No border on the next thing drawn

    // Set the starting values for position of the rectangles
    m1l1 = height;
    m1l2 = height;
    m1l3 = height;
    m2l1 = height;
    m2l2 = height;
    m3l1 = height;
    m3l2 = height;
    rotX1 = height;
    rotY1 = height;
    rotZ1 = height;
    gForceX1 = height;
    gForceY1 = height;
    gForceZ1 = height;
    rotX2 = height;
    rotY2 = height;
    rotZ2  = height;
    gForceX2 = height;
    gForceY2 = height;
    gForceZ2 = height;
    ax1Off = -750;
    ay1Off = 750;
    az1Off = 280;
    gx1Off = 110;
    gy1Off = -150;
    gz1Off = -100;
    ax2Off = -330;
    ay2Off = 500;
    az2Off = 1600;
    gx2Off = 100;
    gy2Off = 300;
    gz2Off = -140;
    // Print a list of the serial ports for debugging purposes
    // if using Processing 2.1 or later, use Serial.printArray()
    println(Serial.list());

    // I know that the first port in the serial list on my Mac is always my FTDI
    // adaptor, so I open Serial.list()[0].
    // On Windows machines, this generally opens COM1.
    // Open whatever port is the one you're using.
    String portName = Serial.list()[0];
    myPort = new Serial(this, portName, 9600);
  }

  void draw() {
    background(42,35,255);
    fill(255,23,65);
    //scale 16 bits MPU to the screen
    scale=(16536*2)/512;
    //scale 10 bits ADC
    scale10=128;
    offset=100;
    //calculate the data in mm 
    m1l1=(10000/239)*m1s1+8000;
    m1l2=(10000/330)*m1s2+5300;
    m1l3=(10000/246)*m1s3+6900;
    m2l1= (10000/265)*m2s1+13000;
    m2l2=(10000/177)*m2s2+8400;
    m3l1=(10000/252)*m3s1+7800;
    m3l2=(10000/242)*m3s2+11500;
    // Draw the shapes
    rect(1*width/20,m1l1/(scale10)+offset,10,10 );
    rect(2*width/20,m1l2/(scale10)+offset,10,10 );
    rect(3*width/20,m1l3/(scale10)+offset,10,10 );
    rect(4*width/20,m2l1/(scale10)+offset,10,10 );
    rect(5*width/20,m2l2/(scale10)+offset,10,10 );
    rect(6*width/20,m3l1/(scale10)+offset,10,10 );
    rect(7*width/20,m3l2/(scale10)+offset,10,10 );
    // Draw MPU1 values
    rect(8*width/20,rotX1/scale+offset,10,10 );
    rect(9*width/20,rotY1/scale+offset,10,10 );
    rect(10*width/20,rotZ1/scale+offset,10,10 );
    rect(11*width/20,gForceX1/scale+offset,10,10 );
    rect(12*width/20,gForceY1/scale+offset,10,10 );
    rect(13*width/20,gForceZ1/scale+offset,10,10 );
    rect(14*width/20,rotX2/scale+offset,10,10 );
    rect(15*width/20,rotY2/scale+offset,10,10 );
    rect(16*width/20,rotZ2/scale+offset,10,10 );
    rect(17*width/20,gForceX2/scale+offset,10,10 );
    rect(18*width/20,gForceY2/scale+offset,10,10 );
    rect(19*width/20,gForceZ2/scale+offset,10,10 );
  }

  void serialEvent(Serial myPort) {
    // read a byte from the serial port:
    int inByte = myPort.read();
    // if this is the first byte received, and it's an A, clear the serial
    // buffer and note that you've had first contact from the microcontroller.
    // Otherwise, add the incoming byte to the array:
    if (firstContact == false) {
      if (inByte == 'A') {
        myPort.clear();          // clear the serial port buffer
        firstContact = true;     // you've had first contact from the microcontroller
        myPort.write('A');       // ask for more
      }
    }
    else {
      // Add the latest byte from the serial port to array:
      serialInArray[serialCount] = inByte;
      serialCount++;

      // If we have 37 bytes:
      if (serialCount > 37) {
                //draw-wire sensors
        m1s1 = serialInArray[0]<<8|serialInArray[1];
        m1s2 = serialInArray[2]<<8|serialInArray[3];
        m1s3 = serialInArray[4]<<8|serialInArray[5];
        m2s1 = serialInArray[6]<<8|serialInArray[7];
        m2s2 = serialInArray[8]<<8|serialInArray[9];
        m3s1 = serialInArray[10]<<8|serialInArray[11];
        m3s2 = serialInArray[12]<<8|serialInArray[13];
                //MPU1 gyro
        rotX1 = (byte(serialInArray[14])<<8|serialInArray[15]) + gx1Off;
        rotY1 = (byte(serialInArray[16])<<8|serialInArray[17]) + gy1Off;
        rotZ1 = (byte(serialInArray[18])<<8|serialInArray[19]) + gz1Off;
        //MPU1 accelerometer
        gForceX1 =(byte(serialInArray[20])<<8|serialInArray[21]) + ax1Off;
        gForceY1 =(byte(serialInArray[22])<<8|serialInArray[23]) + ay1Off;
        gForceZ1 =(byte(serialInArray[24])<<8|serialInArray[25]) + az1Off;
        //MPU2 gyro
        rotX2 = (byte(serialInArray[26])<<8|serialInArray[27]) + gx2Off;
        rotY2 = (byte(serialInArray[28])<<8|serialInArray[29]) + gy2Off;
        rotZ2 = (byte(serialInArray[30])<<8|serialInArray[31]) + gz2Off;
        //MPU2 accelerometer
        gForceX2 = (byte(serialInArray[32])<<8|serialInArray[33]) + ax2Off;
        gForceY2 = (byte(serialInArray[34])<<8|serialInArray[35]) + ay2Off;
        gForceZ2 = (byte(serialInArray[36])<<8|serialInArray[37]) + az2Off;  


        
        // print the values (for debugging purposes only):
        println(m1s1 + "\t" + m1s2 + "\t" + m1s3 + "\t" + m2s1 + "\t" + m2s2 + "\t" + m3s1 + "\t" + m3s2 + "\t");
        println(rotX1 + "\t" + rotY1 + "\t" + rotZ1 + "\t" + gForceX1 + "\t" + gForceY1 + "\t" + gForceZ1 + "\t");
        println(rotX2 + "\t" + rotY2 + "\t" + rotZ2 + "\t" + gForceX2 + "\t" + gForceY2 + "\t" + gForceZ2 + "\t");

        // Send a capital A to request new sensor readings:
        myPort.write('A');
        // Reset serialCount:
        serialCount = 0;
      }
    }
  }
