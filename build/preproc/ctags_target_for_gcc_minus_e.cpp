# 1 "E:\\Desktop\\SpeedTrap\\SpeedTrap.ino"
# 2 "E:\\Desktop\\SpeedTrap\\SpeedTrap.ino" 2
# 3 "E:\\Desktop\\SpeedTrap\\SpeedTrap.ino" 2
# 4 "E:\\Desktop\\SpeedTrap\\SpeedTrap.ino" 2

# 6 "E:\\Desktop\\SpeedTrap\\SpeedTrap.ino" 2



int HTTP_PORT = 80;
String HTTP_METHOD = "POST"; // or "POST"
char HOST_NAME[] = "speedingiscool.loclx.io"; // hostname of web server:
String PATH_NAME = "/save/";

int INST_ID = 0;
int direction = 1;
int16_t prev_distance=0;
int16_t distance;
float speed;

// CONSTANTS
float HEIGHT = 0.381; // (current 15 inches) Height off the ground in meters
int THETA = 35; // angle from 90 degrees across the street
float cosTheta = 0.5735; // cosine of 90-THETA

int16_t get_distance_mm(void);
void send_SpeedInstance();
void send_StillAlive();
void calc_speed();
bool function_to_call(void *argument);
auto timer = timer_create_default();

bool function_to_call(void *argument /* optional argument given to in/at/every */) {
  send_StillAlive();
  Serial.println("Pin 3 is "+ String(digitalRead((17ul))));
  return true; // to repeat the action - false to stop
}

void setup() {
  Serial.begin(115200);
  wifi_setup();
  sensor_setup();

  pinMode(A3, (0x2));
  attachInterrupt(( A3 ), calc_speed, 4);

  timer.every(5000, function_to_call);
}


void loop() {
  timer.tick();
  //Serial.println("Pin 3 is "+ String(digitalRead(PIN_A3)));

}


void calc_speed() {
  Serial.println("interrupt received");
  if (vl53.dataReady()) {
    prev_distance = distance;
    distance = get_distance_mm();
    speed = cosTheta*(prev_distance-distance)/33/* Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!*/ ;
    send_SpeedInstance();
  }

}

int16_t get_distance_mm() {
    // new measurement for the taking!
    int16_t distance = vl53.distance();

    Serial.print((reinterpret_cast<const __FlashStringHelper *>(("Distance: "))));
    Serial.print(distance);
    Serial.println(" mm");

    // data is read out, time for another reading!
    vl53.clearInterrupt();
    return distance;
}


void send_SpeedInstance() {
  if(client.connect(HOST_NAME, HTTP_PORT)){
    // send HTTP header
    client.println(HTTP_METHOD + " " + PATH_NAME + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: keep-alive");
    client.println("captured-speed: "+String(speed));
    client.println("direction: 1");
    client.println("custom-text: TEST LIVE "+String(INST_ID));
    client.println(); // end HTTP request header
    INST_ID = INST_ID+1;
  }
}

void send_StillAlive() {
  if(client.connect(HOST_NAME, HTTP_PORT)){
    // send HTTP header
    client.println(HTTP_METHOD + " " + PATH_NAME + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: keep-alive");
    client.println("captured-speed: 0");
    client.println("direction: 0");
    client.println("custom-text: TEST LIVE "+String(INST_ID));
    client.println(); // end HTTP request header
    INST_ID = INST_ID+1;
  }
}