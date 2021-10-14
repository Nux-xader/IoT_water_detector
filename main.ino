#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define trigPin 25
#define echoPin 26

long durasi;
int jarak;

const char* ssid = "XXXX";
const char* password = "XXXX";

// Initialize Telegram BOT
#define BOTtoken "XXX:XXX"

#ifdef ESP8266
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;


// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);

    // Print the received message
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    if (text == "/start" or text == "/help") {
      String msg = "Welcome, " + from_name + "im water detector IoT Technology.\n\n";
      msg += "send to me message\n";
      msg += "/check to check your water level\n";
      bot.sendMessage(chat_id, msg, "");
    }

    if (text == "/check") {
      String msg = "The distance now is ";
      digitalWrite(trigPin, LOW);
      delayMicroseconds(5);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      durasi = pulseIn(echoPin, HIGH);
      jarak = (durasi*0.034)/2;
      msg += String(jarak);
      msg += "Cm";
      bot.sendMessage(chat_id, msg, "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

#ifdef ESP8266
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
#endif

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
#ifdef ESP32
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
#endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
