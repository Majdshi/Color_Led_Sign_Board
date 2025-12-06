#include <WiFi.h>
#include <WebServer.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

const char* ssid = "****";
const char* password = "****";

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

uint8_t text_size = 2;

uint32_t Data_Color=0xff0000;
uint32_t Text_Color=0xff00f0;

long countdownTotalSeconds = 0;      // remaining seconds of countdown
bool countdownRunning = false;
bool countdownExpired = false;

// Optionally: store the original start seconds for reset
long countdownStartSeconds = 0;

String displayMode = "countdown";  // or "message"

// Day names with Sunday at index 0
const char* dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Clock</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      background: #111; color: #eee; font-family: sans-serif;
      display: flex; flex-direction: column; align-items: center;
      justify-content: center; height: 100%; margin: 0; padding: 20px;
    }

    h1, h2 { text-align: center; }

    .big { font-size: 56px; margin: 0; }
    .medium { font-size: 32px; margin: 4px 0; }
    .centered { text-align: center; }

    input[type=text], input[type=number], input[type=color] {
      padding: 8px; font-size: 18px; border-radius: 5px;
      border: none; background: #333; color: #eee;
    }

    button, input[type=submit] {
      padding: 10px 20px; font-size: 16px;
      margin: 5px; cursor: pointer;
      background-color: #444; color: #fff;
      border: none; border-radius: 5px;
      transition: background 0.3s;
    }

    button:hover, input[type=submit]:hover {
      background-color: #666;
    }

    #response {
      margin-top: 10px;
      color: lightgreen;
      min-height: 20px;
      text-align: center;
    }

    /* Countdown + Color Row */
    .horizontal-row {
      display: flex;
      gap: 20px;
      margin-top: 20px;
      flex-wrap: wrap;
      justify-content: center;
    }

    .box {
      background-color: #222;
      padding: 20px;
      border-radius: 10px;
      min-width: 250px;
    }

    .countdown-box .input-row {
      display: flex;
      justify-content: space-around;
      flex-wrap: wrap;
      margin-bottom: 20px;
    }

    .time-input {
      display: flex;
      flex-direction: column;
      align-items: center;
      margin: 10px;
    }

    .form-row {
      display: flex;
      gap: 30px;
      flex-wrap: wrap;
      justify-content: center;
      margin-top: 20px;
    }

    .form-row form {
      background: #222;
      padding: 15px 25px;
      border-radius: 10px;
    }

    label {
      font-size: 18px;
      margin-right: 10px;
    }

    input[type="radio"] {
      transform: scale(1.3);
      margin-right: 8px;
    }
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

  <!-- Countdown + Color Pickers in Horizontal Row -->
  <div class="horizontal-row">
    <div class="box countdown-box">
      <h2>Countdown Timer</h2>
      <form id="countdownForm">
        <div class="input-row">
          <div class="time-input">
            <label for="hours">Hours</label>
            <input type="number" name="hours" id="hours" min="0" value="0">
          </div>
          <div class="time-input">
            <label for="minutes">Minutes</label>
            <input type="number" name="minutes" id="minutes" min="0" value="0">
          </div>
          <div class="time-input">
            <label for="seconds">Seconds</label>
            <input type="number" name="seconds" id="seconds" min="0" value="0">
          </div>
        </div>
        <div style="text-align: center;">
          <button type="submit">Start</button>
          <button type="button" id="stopBtn">Stop</button>
          <button type="button" id="resetBtn">Reset</button>
        </div>
      </form>
    </div>

    <div class="box">
      <h3>Data Color</h3>
      <form id="data_color">
        <label for="data_fav_color">Pick Color:</label><br>
        <input type="color" id="data_fav_color" name="data_fav_color" value="#ff0000"><br>
        <input type="submit" value="Apply">
      </form>
    </div>

    <div class="box">
      <h3>Message Text Color</h3>
      <form id="text_color">
        <label for="Text_fav_color">Pick Color:</label><br>
        <input type="color" id="Text_fav_color" name="Text_fav_color" value="#ff00f0"><br>
        <input type="submit" value="Apply">
      </form>
    </div>
  </div>

  <!-- Text Size + Display Mode in Horizontal Row -->
  <div class="form-row">
    <form id="TextSizeForm">
      <h3>Message Size</h3>
      <label><input type="radio" name="textSize" value="1"> 8px</label><br>
      <label><input type="radio" name="textSize" value="2"> 16px</label>
    </form>

    <form id="displayModeForm">
      <h3>Display Mode</h3>
      <label><input type="radio" name="displayMode" value="countdown" checked> Countdown</label><br>
      <label><input type="radio" name="displayMode" value="message"> Message</label>
    </form>
  </div>

  <!-- JavaScript Section -->
  <script>
    // Message sending and clearing
    document.getElementById("messageForm").addEventListener("submit", async function(e) {
      e.preventDefault();
      const text = document.getElementById("messageInput").value;
      if (!text.trim()) return;
      const resp = await fetch("/set_message", {
        method: "POST",
        body: new URLSearchParams({ text }),
        headers: { "Content-Type": "application/x-www-form-urlencoded" }
      });
      document.getElementById("response").textContent = await resp.text();
      document.getElementById("messageInput").value = "";
    });

    document.getElementById("clearBtn").addEventListener("click", async function() {
      const resp = await fetch("/clear_message", { method: "POST" });
      document.getElementById("response").textContent = await resp.text();
      document.getElementById("messageInput").value = "";
    });

    // Text size
    document.getElementById("TextSizeForm").addEventListener("change", async function(e) {
      if (e.target.name === "textSize") {
        const size = e.target.value;
        const resp = await fetch("/textsize", {
          method: "POST",
          body: new URLSearchParams({ size }),
          headers: { "Content-Type": "application/x-www-form-urlencoded" }
        });
        document.getElementById("response").textContent = await resp.text();
      }
    });

    // Data color
    document.getElementById("data_color").addEventListener("submit", async function(e){
      e.preventDefault();
      const color = document.getElementById("data_fav_color").value;
      const resp = await fetch("/data_color_change", {
        method: "POST",
        body: new URLSearchParams({ dcolor: color }),
        headers: { "Content-Type": "application/x-www-form-urlencoded" }
      });
      document.getElementById("response").textContent = await resp.text();
    });

    // Text color
    document.getElementById("text_color").addEventListener("submit", async function(e) {
      e.preventDefault();
      const color = document.getElementById("Text_fav_color").value;
      const resp = await fetch("/Text_color_change", {
        method: "POST",
        body: new URLSearchParams({ Tcolor: color }),
        headers: { "Content-Type": "application/x-www-form-urlencoded" }
      });
      document.getElementById("response").textContent = await resp.text();
    });

    // Countdown control
    document.getElementById("countdownForm").addEventListener("submit", async function(e) {
      e.preventDefault();
      const hours = document.getElementById("hours").value;
      const minutes = document.getElementById("minutes").value;
      const seconds = document.getElementById("seconds").value;
      const resp = await fetch("/start_countdown", {
        method: "POST",
        body: new URLSearchParams({ hours, minutes, seconds }),
        headers: { "Content-Type": "application/x-www-form-urlencoded" }
      });
      document.getElementById("response").textContent = await resp.text();
    });

    document.getElementById("stopBtn").addEventListener("click", async function() {
      const resp = await fetch("/stop_countdown", { method: "POST" });
      document.getElementById("response").textContent = await resp.text();
    });

    document.getElementById("resetBtn").addEventListener("click", async function() {
      const resp = await fetch("/reset_countdown", { method: "POST" });
      document.getElementById("response").textContent = await resp.text();
    });

    // Display mode
    document.getElementById("displayModeForm").addEventListener("change", async function(e) {
      if (e.target.name === "displayMode") {
        const mode = e.target.value;
        const resp = await fetch("/set_display_mode", {
          method: "POST",
          body: new URLSearchParams({ mode }),
          headers: { "Content-Type": "application/x-www-form-urlencoded" }
        });
        document.getElementById("response").textContent = await resp.text();
      }
    });

    // Live status update
    async function fetchStatus() {
      const resp = await fetch('/status');
      const j = await resp.json();
      const dayNames = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
      document.getElementById('time').textContent =
        ("0" + j.hour).slice(-2) + ":" +
        ("0" + j.minute).slice(-2) + ":" +
        ("0" + j.second).slice(-2);
      document.getElementById('date').textContent =
        ("0" + j.date).slice(-2) + "/" +
        ("0" + j.month).slice(-2) + "/" +
        ("0" + j.year).slice(-2);
      document.getElementById('day').textContent =
        "Day: " + (j.day >= 1 && j.day <= 7 ? dayNames[j.day - 1] : j.day);
      document.getElementById('humidity').textContent =
        "Humidity: " + j.humidity.toFixed(1) + "%";
      document.getElementById('temperature').textContent =
        "Temperature: " + j.temperature.toFixed(1) + "C";
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

void handleTextSizePost(){

  if(httpServer.hasArg("size")){
    text_size = httpServer.arg("size").toInt();
    if (text_size == 1) {
      httpServer.send(200, "text/plain", "Text Size is 8px");
    } else if (text_size == 2) {
      httpServer.send(200, "text/plain", "Text Size is 16px");
    } else {
      httpServer.send(400, "text/plain", "Invalid text size");
    }
    }

}

void handelDataColorPost(){
  if(httpServer.hasArg("dcolor")){
    String hexColor = httpServer.arg("dcolor");
    if (hexColor.startsWith("#") && hexColor.length() == 7) {
      long rgb = strtol(hexColor.substring(1).c_str(), NULL, 16);
      Data_Color = rgb;
      httpServer.send(200, "text/plain", "Color Has Changed");
    } else {
      httpServer.send(400, "text/plain", "Invalid Color Format");
    }
  } else {
    httpServer.send(400, "text/plain", "Missing dcolor");
  }
}

void handelTextColorPost(){
  if(httpServer.hasArg("Tcolor")){
    String hexColor = httpServer.arg("Tcolor");
    if (hexColor.startsWith("#") && hexColor.length() == 7) {
      long rgb = strtol(hexColor.substring(1).c_str(), NULL, 16);
      Text_Color = rgb;
      httpServer.send(200, "text/plain", "Color Has Changed");
    } else {
      httpServer.send(400, "text/plain", "Invalid Color Format");
    }
  } else {
    httpServer.send(400, "text/plain", "Missing Tcolor");
  }
}



void handleStopCountdown() {
  countdownRunning = false;
  httpServer.send(200, "text/plain", "Countdown stopped");
}

void handleResetCountdown() {
  countdownTotalSeconds = countdownStartSeconds;
  countdownRunning = false;
  countdownExpired = false;
  httpServer.send(200, "text/plain", "Countdown reset");
}

void handleSetDisplayMode() {
  if (httpServer.hasArg("mode")) {
    String mode = httpServer.arg("mode");
    if (mode == "countdown" || mode == "message") {
      displayMode = mode;
      httpServer.send(200, "text/plain", "Display mode set to " + mode);
    } else {
      httpServer.send(400, "text/plain", "Invalid display mode");
    }
  } else {
    httpServer.send(400, "text/plain", "Missing mode");
  }
}

void handleStartCountdown() {
  if (httpServer.hasArg("hours") && httpServer.hasArg("minutes") && httpServer.hasArg("seconds")) {
    int h = httpServer.arg("hours").toInt();
    int m = httpServer.arg("minutes").toInt();
    int s = httpServer.arg("seconds").toInt();

    if (h < 0) h = 0;
    if (m < 0) m = 0;
    if (s < 0) s = 0;

    countdownTotalSeconds = (long)h * 3600 + (long)m * 60 + s;
    countdownStartSeconds = countdownTotalSeconds;
    countdownRunning = true;
    countdownExpired = false;
    httpServer.send(200, "text/plain", "Countdown started");
  } else {
    httpServer.send(400, "text/plain", "Missing fields");
  }
}


uint16_t get565From24(uint32_t color24) {
  uint8_t r = (color24 >> 16) & 0xFF;
  uint8_t g = (color24 >> 8) & 0xFF;
  uint8_t b = color24 & 0xFF;
  return dma_display->color565(r, g, b);
}

long getCurrentRtcSeconds() {
  return hour_ * 3600L + minute_ * 60L + second_;
}

void onCountdownFinish() {
  // E.g. show a message, flash, trigger GPIO, etc.
  customMessage = "COUNTDOWN DONE";
  scrollX = dma_display->width();
  // Or you could disable something, or call some handler
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
  Serial.print("IP : "+ WiFi.localIP().toString() );
  delay(5000);
  dma_display->clearScreen(); 
  tcpServer.begin();
  httpServer.on("/", handleRoot);
  httpServer.on("/status", handleStatus);
  httpServer.on("/set_message", HTTP_POST, handleMessagePost);
  httpServer.on("/clear_message", HTTP_POST, handleClearMessage);
  httpServer.on("/textsize", HTTP_POST, handleTextSizePost);
  httpServer.on("/data_color_change", HTTP_POST, handelDataColorPost);
  httpServer.on("/Text_color_change",HTTP_POST,handelTextColorPost);
  httpServer.on("/stop_countdown", HTTP_POST, handleStopCountdown);
  httpServer.on("/reset_countdown", HTTP_POST, handleResetCountdown);
  httpServer.on("/set_display_mode", HTTP_POST, handleSetDisplayMode);
  httpServer.on("/start_countdown", HTTP_POST, handleStartCountdown);


  httpServer.begin();
}

void loop() {
  httpServer.handleClient();

  static long lastRtcSec = -1;

  if (!tcpClient) {
    tcpClient = tcpServer.available();
  }

  if (tcpClient && tcpClient.connected() && tcpClient.available()) {
    String line = tcpClient.readStringUntil('\n');
    line.trim();
    int vals[9], idx = 0;
    char bufC[line.length() + 1];
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

      // Clear screen
      dma_display->fillRect(0, 0, dma_display->width(), 48, dma_display->color565(0, 0, 0));

      // Show time, date, day, humidity, temperature (common to both modes)
      dma_display->setTextColor(get565From24(Data_Color));
      dma_display->setTextSize(1);
      dma_display->setCursor(40, 2);
      dma_display->printf("%02d:%02d:%02d", hour_, minute_, second_);
      dma_display->setCursor(40, 14);
      dma_display->printf("%02d/%02d/20%d", date_, month_, year_);

      const char* dname = (day_ >= 1 && day_ <= 7) ? dayNames[day_ - 1] : "???";
      dma_display->setCursor(44, 26);
      dma_display->print("Day: ");
      dma_display->print(dname);

      dma_display->setCursor(2, 2);
      dma_display->printf("%.1f%%", humidity / 10.0f);
      dma_display->setCursor(2, 14);
      dma_display->printf("%.1fC", temperature / 10.0f);

      // Countdown logic if enabled
      if (displayMode == "countdown" && countdownRunning && !countdownExpired) {
        long currentSec = getCurrentRtcSeconds();
        

        if (lastRtcSec < 0) lastRtcSec = currentSec;

        long elapsed = currentSec - lastRtcSec;
        if (elapsed < 0) elapsed += 86400;  // Handle rollover

        if (elapsed > 0) {
          countdownTotalSeconds -= elapsed;
          lastRtcSec = currentSec;
        }

        if (countdownTotalSeconds <= 0) {
          countdownTotalSeconds = 0;
          countdownRunning = false;
          countdownExpired = true;
          onCountdownFinish();
        }
      }

      // Display countdown if in countdown mode
      if (displayMode == "countdown") {
        int ch = countdownTotalSeconds / 3600;
        int cm = (countdownTotalSeconds % 3600) / 60;
        int cs = countdownTotalSeconds % 60;
        
            dma_display->fillRect(0, 48, dma_display->width(), 16, dma_display->color565(0, 0, 0));

        dma_display->setTextSize(2);
        dma_display->setTextColor(get565From24(Data_Color));
        dma_display->setCursor(10, 48);
        dma_display->printf("%02d:%02d:%02d", ch, cm, cs);
      }
    }
  }

  // Message display (only if in message mode)
  if (displayMode == "message") {
    int msgY = 48, msgH = 16;
    static unsigned long lastDraw = 0;
    if (!customMessage.isEmpty() && millis() - lastDraw >= 60) {
      lastDraw = millis();
      dma_display->fillRect(0, msgY, dma_display->width(), msgH, dma_display->color565(0, 0, 0));
      dma_display->setTextSize(text_size);
      dma_display->setTextColor(get565From24(Text_Color));

      int tw = (text_size == 1) ? customMessage.length() * 6 : customMessage.length() * 12;

      if (tw > dma_display->width()) {
        scrollX--;
        if (scrollX < -tw) scrollX = dma_display->width();
        dma_display->setCursor(scrollX, (text_size == 1) ? msgY + 8 : msgY);
      } else {
        dma_display->setCursor((dma_display->width() - tw) / 2, msgY);
      }
      dma_display->print(customMessage);
      dma_display->setTextSize(1);
    } else if (customMessage.isEmpty()) {
      dma_display->fillRect(0, msgY, dma_display->width(), msgH, dma_display->color565(0, 0, 0));
    }
  }

  if (tcpClient && !tcpClient.connected()) tcpClient.stop();
}
