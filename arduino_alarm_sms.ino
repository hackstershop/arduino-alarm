        #include <Ethernet.h>
        #include <SPI.h>
        int counter  = 0;// counts number of movement detctions
        boolean alarm_state = false;// define the state of the alarm
        byte mac[] = {  0x90, 0xA2, 0xDA, 0x0D, 0xF6, 0xFF };// choose mac address
        byte ip[] = {  192, 168, 1, 111}; // choose ip address
        byte gateway[] = { 192,168,1,1 };// your router gateway			
        EthernetClient client;
        const String number = "number=your cell phone number(without initial 0)&body='אזעקה בבית'";//2 paramaters of number and body. don't put the first 0 in the number!!!
        void setup() {
          Serial.begin(9600);
          Ethernet.begin(mac); // connect to the router
          delay(10000); // give the arduino some time to connect via the router
          pinMode(2, INPUT); // motion detector
          pinMode(3, INPUT); // button switch
          pinMode(5, OUTPUT); // LED
        }
        
        void loop(){
          if(Ethernet.maintain() != 2){
                Ethernet.begin(mac);
            }
           if(digitalRead(3) == HIGH){// if the button is pressed
              delay(1000);
              alarm_state = !alarm_state; // toggle between alarm states
              }
            if(alarm_state){ // if the alarm is on (ready)
              digitalWrite(5, HIGH);// turn on led
              if(digitalRead(2) == HIGH){ // if motion sensor detects movement
                delay(1000);
                Serial.println("detect motion");
                counter += 1; // add to counter
              if(counter > 10){ // if thershold counter is over 20
                client.connect("46.101.253.187",80);
                delay(1000);        
                client.println("POST /arduinosms.php HTTP/1.1");           
                client.println("Host: 46.101.253.187");
                client.println("Content-Type: application/x-www-form-urlencoded");
                client.println("Connection: close");
                client.println("User-Agent: Arduino/1.0");
                client.print("Content-Length: ");
                client.println(number.length());
                client.println();
                client.print(number);
                client.println();   
                client.stop(); 
                delay(30000); 
                alarm_state = !alarm_state; // turn off the alarm
                counter = 0; // restart counter
                digitalWrite(5, LOW); // turn off LED
               }
             }
          }
            else{ // if button is pressed to neutralize the alarm
              digitalWrite(5, LOW); // turn off led
            }
        }
          
