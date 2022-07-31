#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "NILESH";
const char* password = "1234567890";


const char* awsEndpoint = "a30xom1njimvh2-ats.iot.ap-south-1.amazonaws.com";


#define Relay1 15
#define Relay2 2
#define Relay3 4


static const char certificatePemCrt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUYlRgKGepd5TZb8CCnckDuxxkRHgwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMDQyNTEwMjcw
NVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALySSwWhmoL0em9phQlo
efpJyxzSfsPHjCP0h2UhXy6G1W7HubRRu/gghrtr4ZAxL9wAQ8UBM/9COR25Xmam
0GEIKkBGtIa92nZ/eS6b4JJohrOd9plSAekmbgSkNnnt6dIp2YGbO517NihRrfed
8xPUId6Pj3q14bgtC2vJUNJe1StLAmrtfi3AIQwV8SXI9y0xCO8O/aLXV8W3judA
Db3EwgA5+SliZoVc71FIIR/VqW4rNtTKmb4/XA0OgzeFMV40yGo0bIwef37XJBao
PT6Y7QTCMCH+eVtXpUBOFuOoX/bdP6BBSYY5htHBTUjn5+PgvqePbx1zEbJHUlFI
ZdkCAwEAAaNgMF4wHwYDVR0jBBgwFoAU+pZharmiWwPFd3PtOR2yvQjUGy4wHQYD
VR0OBBYEFAJ4bFnLBADQ4sOkofap6tcpgGyrMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQA1qSVITFLtgP8hHYGIPwkv0Isb
wHuXtVBwfN6zcXZfEwLKAu+XjLrMgivjsRFfaT/2fe9zSQ98IlldoysWf0rWx5ou
bq9ctSlZf8eK2wWbWqjwOQVUk6O5d7EF2TWLxwuZ5P/xuuvdepClDURJkdZ4ifjf
qxlBAdGLA3jLR/8p6i3VEqwnGdj0csFO/R7Zud9pBhfdTuiI0TJL2qVuXMJjnE0i
Jl35tFz1QYI9QbNydS//Kpq5G35jOruGevF43bjOav8J/fXSgolY5cJh3yDGTXOk
bRkpv6qKXyuqHf9q5iHRrJQs70lc9J2hp+Tz3j9N3kFX1K8WukGiyMFZX6Ys
-----END CERTIFICATE-----
)EOF";


static const char privatePemKey[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAvJJLBaGagvR6b2mFCWh5+knLHNJ+w8eMI/SHZSFfLobVbse5
tFG7+CCGu2vhkDEv3ABDxQEz/0I5HbleZqbQYQgqQEa0hr3adn95LpvgkmiGs532
mVIB6SZuBKQ2ee3p0inZgZs7nXs2KFGt953zE9Qh3o+PerXhuC0La8lQ0l7VK0sC
au1+LcAhDBXxJcj3LTEI7w79otdXxbeO50ANvcTCADn5KWJmhVzvUUghH9Wpbis2
1MqZvj9cDQ6DN4UxXjTIajRsjB5/ftckFqg9PpjtBMIwIf55W1elQE4W46hf9t0/
oEFJhjmG0cFNSOfn4+C+p49vHXMRskdSUUhl2QIDAQABAoIBAFAcTmbsCrpLpqAi
UfR3QCrGotVxz4zY/35e6g5aqRcZDR9tFkW224ZEOzjwUflyDAUIz8GoCzEMLzuj
fqlZgPlYAKsh3pvHmEHzUcYtTrCkW2V/XD4yisFEJEUwhjwyiL1SLidTjXYP3Cgw
2xlxLs1jt6fXkXruPiCCcnGuKvnTuUjP4TuSwxJrKa9TrHhLSyOQ5bqRTGS71yhk
FXTI7fIA64sE8QDVB9uuOQmIfd3vTNLhdAjHGTBvztlCQjy6rEtH/FLlr4O7rb4r
i/+L6OnzxZPb4IQS8FfwfFfficLb4wVxkGFM17uxepvMTLohixVDl9sPFAhJjO6A
2ymVvkkCgYEA2+ag6uGRc9hBDH0Zv+PKG8sDfE+pctCVdSelZXNLjwbtY0WXQC+a
CkZdaXx1YBNK+ypJdUM4+YBy27k1JyC3toF6ILgzqIjufzMaYYApLtwolRhgA4tA
xlND7Msuoq3l+2tG+I75D2zaPivKH5bYPXDoql0+keogKC32qItPk6sCgYEA24cK
LTRTnK9dGHiTAaXQ2gh9+Ra3cLCHZgIZK3MM6NTTmrdSMd7JXYuSOjwCz7OUwE23
uk17BovfFMte2oatCA6eAgQvfVQghaOusMuRt5/MWbExWL4QD9858x3dqORoUnl8
g/viSNexNMaioyB4G18aKb6tCl7yk6ybwCGBqIsCgYAAmOXC/Y3ZlLLIh+Z0APhh
jvGZetXo++wL/2mRlviTkN9AL8YzZG20rXA/gxkGg8bmW70LVn1nIBQvyJb+XjZh
hoSAZj+tq4GKUuc1cJWsDOkHTqOkHtK0NzGuCPc9dd90CVyN/EVs50zIcnIAF/bK
Y+4d2Fb9Knef5FARMR+sNQKBgDDO3mFcj/Z3OuCU9kJYho3jc3atKU3hwcjZUJVL
eCRQJVfxyFGUMfsda/6MlydsyvbSkLXYQjEbahbqfIVjihcPgOv72D8qNCziTpW6
PQSxf7K1Zb5QS6vruqumCUFivxsPLbL+jroZUqnkPAa+0lVrmJqNbVWv9IkGyH44
NOavAoGBANBUkdhXglwGI8O3QbOIzWi35PACY/LiWBWruMmxLiIGU8V5RLrMXKOY
PrPSxPgcOMGhUcB86U+AyiBtrWARf5dN0LPNECnRuyL8LhKR1ExThmbKlId5E6Kb
9UAWvbRnsd8bbKrjTt5o5euBjjQofh7vTm7Z+nWiUbD5RXndq/zt
-----END RSA PRIVATE KEY-----
)EOF";


static const char caPemCrt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

BearSSL::X509List client_crt(certificatePemCrt);
BearSSL::PrivateKey client_key(privatePemKey);
BearSSL::X509List rootCert(caPemCrt);

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

#define AWS_IOT_PUBLISH_TOPIC   "user"

#define AWS_IOT_SUBSCRIBE_TOPIC1 "esp32/relay1"
#define AWS_IOT_SUBSCRIBE_TOPIC2 "esp32/relay2"
#define AWS_IOT_SUBSCRIBE_TOPIC3 "esp32/relay3"

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());
  
  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  
  wiFiClient.setClientRSACert(&client_crt, &client_key);
  wiFiClient.setTrustAnchors(&rootCert);
  pubSubCheckConnect();
  digitalWrite(Relay1,1);
  digitalWrite(Relay2,1);
  digitalWrite(Relay3,1);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubClient.loop();
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  messageHandler(topic,payload,length);  
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("user");
    }
    Serial.println(" connected");
    //pubSubClient.subscribe("");
    //pubSubClient.setCallback(messageHandler);
    pubSubClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC1);
    pubSubClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC2);
    pubSubClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC3);
    }
  
}

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);

  if ( strstr(topic, "esp32/relay1") )
  {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);
    String Relay_data = doc["status"];
    int r = Relay_data.toInt();
    Serial.print(r);
    digitalWrite(Relay1, !r);
    Serial.print("Relay1 - "); Serial.println(Relay_data);
  }

  if ( strstr(topic, "esp32/relay2") )
  {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);
    String Relay_data = doc["status"];
    int r = Relay_data.toInt();
    digitalWrite(Relay2, !r);
    Serial.print("Relay2 - "); Serial.println(Relay_data);
  }

  if ( strstr(topic, "esp32/relay3") )
  {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);
    String Relay_data = doc["status"];
    int r = Relay_data.toInt();
    digitalWrite(Relay3, !r);
    Serial.print("Relay3 - "); Serial.println(Relay_data);
  }



}

void setCurrentTime() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
