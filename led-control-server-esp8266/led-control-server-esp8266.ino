#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char * ssid = STASSID;
const char * password = STAPSK;

/* Create a server instance and specify the port to listen on as an argument */
WiFiServer server(80);

void
setup() {
  /* Initialize serial UART communication with host processor at 115200 baud */
  Serial.begin(115200);

  delay(500);

//  Serial.println();
//  Serial.print("Connecting to Wi-Fi network: ");
//  Serial.println(ssid);

  /* Initiate the Wi-Fi connection */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  /* Wait for connection to the Wi-Fi network */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print(".");
  }

  Serial.println("");
//  Serial.println("Wi-Fi connected");
//  Serial.println(WiFi.localIP());

  /* Start the web server */
  server.begin();
}

void
loop() {
  WiFiClient client = server.available();

  if (client) {
    // Serial.println("\n[Client connected]");
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\r');
        // Serial.println(line);

        /* Check if there is a GET request in the current line */
        if (line.indexOf("GET /?") != -1) {
          String command = line.substring(line.indexOf("GET /?text=") + strlen("GET /?text="), line.indexOf("HTTP/1.1"));;
          Serial.println(command);
        }
        
        /* Wait for end of client's request, that is marked with an empty line */
        if (line.length() == 1 && line[0] == '\n') {
          client.println(prepare_html_page());
          break;
        }
      }
    }

    /* Give the web browser time to receive the data */
    delay(1);

    /* Close the connection */
    client.stop();
    // Serial.println("[Client disonnected]");
  }
}

/* Prepare a web page to be send to a client (web browser) */
String
prepare_html_page(void) {
  String html_page =
    String("HTTP/1.1 200 OK\r\n") +
    "Content-Type: text/html\r\n" +
    "Connection: close\r\n" +
    "\r\n" +
    "<!DOCTYPE HTML>" +
    "<html>" +

    "<title>LED Control</title>" +
    "<h1 align=center><font color=003FFF><span>LED Control</font></span></h1>\r\n" +

    "<form align=center>" +
    "<label align=center for=\"text\">Brightness: </label>" +
    "<input type=\"text\" id=\"text\" name=\"text\">" +
    "<input type=\"submit\" value=\"Send\">" +
    "</form>" +
    
    "</html>" +
    "\r\n";

  return html_page;
}
