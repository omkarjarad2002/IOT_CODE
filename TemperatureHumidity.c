// servo motor

#include <Servo.h>

#define trigPin 9

#define echoPin 8

Servo servo;

int sound = 250;

void setup()
{

    Serial.begin(9600);

    pinMode(trigPin, OUTPUT);

    pinMode(echoPin, INPUT);

    servo.attach(6);
}

void loop()
{

    long duration, distance;

    digitalWrite(trigPin, LOW);

    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);

    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = (duration / 2) / 29.1;
    if (distance < 40)
    {
        Serial.print(distance);
        Serial.println(" cm");
        servo.write(90);
    }

    else
    {

        Serial.println("The distance is more than 180cm");
    }

    delay(500);
    servo.write(-90);
}
// Temperature Humidity
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const char *ssid = "Komal";
const char *password = "nuttertool";
WiFiClient client;
unsigned long myChannelNumber = 1016656;
const char *myWriteAPIKey = "XFG3D1F5D4YGXXXZ";
uint8_t temperature, humidity;
void setup()
{
    Serial.begin(115200);
    dht.begin();
    delay(10);
    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address
    Serial.println(WiFi.localIP());
    ThingSpeak.begin(client);
}
void loop()
{
    static boolean data_state = false;
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    Serial.print("Temperature Value is :");
    Serial.print(temperature);
    Serial.println("C");
    Serial.print("Humidity Value is :");
    Serial.print(humidity);
    Serial.println("%");
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel. Here, we write to field 1 and field2.
    if (data_state)
    {
        ThingSpeak.writeField(myChannelNumber, 1, temperature, myWriteAPIKey);
        data_state = false;
    }
    else
    {
        ThingSpeak.writeField(myChannelNumber, 2, humidity, myWriteAPIKey);
        data_state = true;
    }
    delay(30000); // ThingSpeak will only accept updates every 15 seconds.
}

// Soil Moisture
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
int sensorpin = A0;
int sensorvalue = 0;
int outputvalue = 0;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "lKT5GEt18dLeaUYw8os-lBfEeZ8ECM2d";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Komal";
char pass[] = "komal1313";
void setup()
{
    // Debug console
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
}
void loop()
{
    sensorvalue = analogRead(sensorpin);
    outputvalue = map(sensorvalue, 0, 1023, 0, 100); // 100,0 pottupaaru
    delay(1000);
    if (outputvalue > 74)
    {
        Serial.println("water your plant");
        Serial.print(outputvalue);
        Blynk.notify("water your plant");
        delay(1000);
    }
    else if (outputvalue < 45)
    {
        Serial.println("soil is wet enough to water");
        Serial.print(outputvalue);
        Blynk.notify("soil is wet enough to water");
        delay(1000);
    }
    Blynk.run();
}