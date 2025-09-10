#include <WiFi.h>
#include <WebServer.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

const char* ssid = "*********";
const char* password = "***********";

WiFiServer tcpServer(5000);
WiFiClient tcpClient;
WebServer httpServer(80);

#define PANEL_WIDTH 64
#define PANEL_HEIGHT 64
#define PANELS_NUMBER 2
#define PIN_E 18

MatrixPanel_I2S_DMA* dma_display = nullptr;

volatile int second_ = 0, minute_ = 0, hour_ = 0;
volatile int day_ = 1, date_ = 1, month_ = 1, year_ = 2025;
volatile uint16_t humidity = 0;
volatile int16_t temperature = 0;

String customMessage = "";
String lastMessage = "";
int scrollX = 0;
unsigned long lastScrollTime = 0;
const int scrollSpeed = 50;

// Day names with Sunday at index 0
const char* dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Clock</title>
  <style>
    body { background:#111; color:#eee; font-family:sans-serif;
           display:flex; flex-direction:column; align-items:center;
           justify-content:center; height:100vh; margin:0; }
    .big { font-size:56px; margin:0; }
    .medium { font-size:32px; margin:4px 0; }
    .centered { text-align:center; }
    form { margin-top:20px; }
    input[type=text] { padding:8px; font-size:18px; width:300px; max-width:90vw; }
    button { padding:8px 15px; font-size:18px; margin-left:10px; cursor:pointer; }
    #response { margin-top:10px; color:lightgreen; min-height:20px; }
  </style>
</head>
<body>
  <h1>ESP32 Clock / Status</h1>
  <div class="centered">
    <div id="time" class="big">--:--:--</div>
    <div id="date" class="medium">--/--/--</div>
    <div id="day" class="medium">Day: --</div>
    <div id="humidity" class="medium">Humidity: --%</div>
    <div id="temperature" class="medium">Temperature: --C</div>
  </div>
  <h2>Send Message</h2>
  <form id="messageForm">
    <input type="text" id="messageInput" placeholder="Enter message" />
    <button type="submit">Send</button>
    <button type="button" id="clearBtn">Clear Message</button>
  </form>
  <p id="response"></p>
  <script>
    document.getElementById("messageForm").addEventListener("submit", async function(e){
      e.preventDefault(); const text = document.getElementById("messageInput").value;
      if (!text.trim()) return;
      const resp = await fetch("/set_message", {
        method:"POST", body:new URLSearchParams({text}),
        headers:{"Content-Type":"application/x-www-form-urlencoded"}
      });
      document.getElementById("response").textContent = await resp.text();
      document.getElementById("messageInput").value = "";
    });
    document.getElementById("clearBtn").addEventListener("click", async function(){
      const resp = await fetch("/clear_message",{method:"POST"});
      document.getElementById("response").textContent = await resp.text();
      document.getElementById("messageInput").value = "";
    });
    async function fetchStatus(){
      const resp = await fetch('/status');
      const j = await resp.json();
      const dayNames = ["Sun","Mon","Tue","Wed","Thu","Fri","Sat"];
      document.getElementById('time').textContent = ("0"+j.hour).slice(-2)+":"+
                                                         ("0"+j.minute).slice(-2)+":"+
                                                         ("0"+j.second).slice(-2);
      document.getElementById('date').textContent = ("0"+j.date).slice(-2)+"/"+
                                                        ("0"+j.month).slice(-2)+"/"+
                                                        ("0"+j.year).slice(-2);
      document.getElementById('day').textContent = "Day: " + (j.day>=1 && j.day<=7 ? dayNames[j.day-1] : j.day);
      document.getElementById('humidity').textContent = "Humidity: " + j.humidity.toFixed(1)+"%";
      document.getElementById('temperature').textContent = "Temperature: " + j.temperature.toFixed(1)+"C";
    }
    setInterval(fetchStatus, 1000);
    fetchStatus();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  httpServer.send_P(200, "text/html", index_html);
}

void handleStatus() {
  char buf[200];
  snprintf(buf, sizeof(buf),
    "{\"second\":%d,\"minute\":%d,\"hour\":%d,\"day\":%d,\"date\":%d,\"month\":%d,\"year\":%d,"
    "\"humidity\":%.1f,\"temperature\":%.1f}",
    second_, minute_, hour_, day_, date_, month_, year_,
    humidity / 10.0f, temperature / 10.0f);
  httpServer.send(200, "application/json", buf);
}

void handleMessagePost() {
  if (httpServer.hasArg("text")) {
    customMessage = httpServer.arg("text");
    scrollX = dma_display->width();
    lastMessage = "";
    httpServer.send(200, "text/plain", "Message received");
  } else httpServer.send(400, "text/plain", "Missing 'text'");
}

void handleClearMessage() {
  customMessage = "";
  scrollX = 0;
  lastMessage = "";
  httpServer.send(200, "text/plain", "Message cleared");
}

void setup() {
  Serial.begin(115200);
  HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANELS_NUMBER);
  mxconfig.gpio.e = PIN_E;
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(128);
  dma_display->fillScreen(0);

  WiFi.begin(ssid, password);
   dma_display->setCursor(10, 20);
  dma_display->print("Connecting to Wi-Fi");
  delay(1000);
  dma_display->clearScreen(); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print("."); 
  }
  Serial.println("\nWiFi connected");
  dma_display->setCursor(10, 20);
  dma_display->print(WiFi.localIP().toString());
  delay(5000);
  dma_display->clearScreen(); 
  tcpServer.begin();
  httpServer.on("/", handleRoot);
  httpServer.on("/status", handleStatus);
  httpServer.on("/set_message", HTTP_POST, handleMessagePost);
  httpServer.on("/clear_message", HTTP_POST, handleClearMessage);
  httpServer.begin();
}

void loop() {
  httpServer.handleClient();

  if (!tcpClient){
     tcpClient = tcpServer.available();
  }

  if (tcpClient && tcpClient.connected() && tcpClient.available()) {
    String line = tcpClient.readStringUntil('\n');
    line.trim();
    int vals[9], idx = 0;
    char bufC[line.length()+1];
    line.toCharArray(bufC, sizeof(bufC));
    char* tok = strtok(bufC, ",");
    while (tok && idx < 9) {
      vals[idx++] = atoi(tok);
      tok = strtok(NULL, ",");
    }
    if (idx == 9) {
      second_ = vals[0]; minute_ = vals[1]; hour_ = vals[2];
      day_ = vals[3]; date_ = vals[4]; month_ = vals[5]; year_ = vals[6];
      humidity = vals[7]; temperature = vals[8];

      dma_display->fillRect(0,0,dma_display->width(),48,dma_display->color565(0,0,0));
      dma_display->setTextColor(dma_display->color565(255,255,0));
      dma_display->setTextSize(1);
      dma_display->setCursor(40,2);
      dma_display->printf("%02d:%02d:%02d", hour_, minute_, second_);
      dma_display->setCursor(40,14);
      dma_display->printf("%02d/%02d/20%d", date_, month_, year_);

      const char* dname = (day_>=1 && day_<=7) ? dayNames[day_-1] : "???";
      dma_display->setCursor(44,26);
      dma_display->print("Day: ");
      dma_display->print(dname);

      dma_display->setCursor(2,2);
      dma_display->printf("%.1f%%", humidity/10.0f);
      dma_display->setCursor(2,14);
      dma_display->printf("%.1fC", temperature/10.0f);
    }
  }

  int msgY=48, msgH=16;
  static unsigned long lastDraw=0;
  if (!customMessage.isEmpty() && millis()-lastDraw >= 50) {
    lastDraw = millis();
    dma_display->fillRect(0,msgY,dma_display->width(),msgH, dma_display->color565(0,0,0));
    dma_display->setTextSize(2);
    dma_display->setTextColor(dma_display->color565(0,255,255));
    int tw = customMessage.length()*12;
    if (tw > dma_display->width()) {
      scrollX--;
      if (scrollX < -tw) scrollX = dma_display->width();
      dma_display->setCursor(scrollX, msgY);
    } else {
      dma_display->setCursor((dma_display->width()-tw)/2, msgY);
    }
    dma_display->print(customMessage);
    dma_display->setTextSize(1);
  } else if (customMessage.isEmpty()) {
    dma_display->fillRect(0,msgY,dma_display->width(),msgH, dma_display->color565(0,0,0));
  }

  if (tcpClient && !tcpClient.connected()) tcpClient.stop();
}
