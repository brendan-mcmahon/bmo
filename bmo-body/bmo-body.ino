//#include <WiFi.h>
//#include <WebSocketsClient.h>

//const char* ssid = "guacamole";
//const char* password = "SwingSets09";
//
//WebSocketsClient webSocket;

const int numButtons = 7;
const int buttonPins[] = {4, 5, 13, 14, 18, 17, 16};
int buttonStates[numButtons] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

void setup() {
  Serial.println("Starting...");
  Serial.begin(115200);

//  WiFi.begin(ssid, password);
//  Serial.print("Connecting to WiFi");
//  while (WiFi.status() != WL_CONNECTED) {
//    
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("\nConnected to WiFi");
//
//  webSocket.begin("192.168.1.94", 8080, "/");
//  webSocket.onEvent(webSocketEvent);

  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  Serial.println("Ready");
}

void loop() {
//  webSocket.loop();

  for (int i = 0; i < numButtons; i++) {
    int currentState = digitalRead(buttonPins[i]);

    if (currentState == LOW && buttonStates[i] == HIGH) {
      String message = String(i) + ":DOWN";
      Serial.println(message);
//      webSocket.sendTXT(message);
    }
    
    if (currentState == HIGH && buttonStates[i] == LOW) {
      String message = String(i) + ":UP";
      Serial.println(message);
//      webSocket.sendTXT(message);
    }

    buttonStates[i] = currentState;
  }

  delay(50);
}

//void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
//  switch (type) {
//    case WStype_DISCONNECTED:
//      Serial.println("Disconnected from WebSocket server");
//      break;
//    case WStype_CONNECTED:
//      Serial.println("Connected to WebSocket server");
//      webSocket.sendTXT("ESP32 connected");
//      break;
//    case WStype_TEXT:
//      Serial.printf("Received: %s\n", payload);
//      break;
//    default:
//      break;
//  }
//}
