#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ============================================================
// SMART GATE ALERT SYSTEM
// ESP32 DOIT DEVKIT V1
//
// OLED + HC-SR04 + SPEAKER SIREN + LEDs + POWER SWITCH
// ============================================================


// ============================================================
// PIN DEFINITIONS
// ============================================================

#define TRIG_PIN       5
#define ECHO_PIN       18

#define SPEAKER_PIN    25

#define RED_LED_PIN    26
#define GREEN_LED_PIN  27

#define POWER_SW_PIN   33

// OLED
#define OLED_SDA       21
#define OLED_SCL       22
#define OLED_ADDR      0x3C


// ============================================================
// OLED
// ============================================================

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

bool oledOK = false;


// ============================================================
// SPEAKER PWM
// Arduino ESP32 2.x
// ============================================================

#define SPEAKER_CHANNEL  0
#define SPEAKER_PWM_RES  10

const int SPEAKER_DUTY = 512;


// ============================================================
// SIREN
// ============================================================

const int SIREN_LOW_FREQ  = 700;
const int SIREN_HIGH_FREQ = 1500;

const unsigned long SIREN_STEP_TIME = 300;

bool sirenRunning = false;
bool sirenHighTone = false;

unsigned long lastSirenChange = 0;


// ============================================================
// DISTANCE
// ============================================================

const float CLOSED_DISTANCE = 20.0;
const float OPEN_DISTANCE   = 40.0;


// ============================================================
// TIMING
// ============================================================

const unsigned long ALARM_DELAY = 3000;
const unsigned long SENSOR_INTERVAL = 100;
const unsigned long DEBOUNCE_TIME = 50;


// ============================================================
// GATE STATE
// ============================================================

enum GateState
{
    GATE_CLOSED,
    GATE_OPEN_WAIT,
    GATE_OPEN_ALARM
};

GateState gateState = GATE_CLOSED;


// ============================================================
// GLOBALS
// ============================================================

bool systemEnabled = true;

float distanceCM = -1.0;

unsigned long lastSensorRead = 0;
unsigned long gateOpenedAt = 0;


// ============================================================
// SWITCH
// ============================================================

int lastSwitchReading = HIGH;
int stableSwitchState = HIGH;

unsigned long lastSwitchChange = 0;


// ============================================================
// OLED UPDATE CONTROL
// ============================================================

// Prevent unnecessary OLED redraws.
GateState lastOLEDState = GATE_CLOSED;

int lastCountdown = -1;

bool lastOLEDEnabled = true;


// ============================================================
// HAPPY FACE
// ============================================================

void drawHappyFace()
{
    display.drawCircle(
        64, 45, 16,
        SSD1306_WHITE
    );

    display.fillCircle(
        58, 40, 2,
        SSD1306_WHITE
    );

    display.fillCircle(
        70, 40, 2,
        SSD1306_WHITE
    );

    display.drawLine(
        57, 47,
        60, 50,
        SSD1306_WHITE
    );

    display.drawLine(
        60, 50,
        64, 52,
        SSD1306_WHITE
    );

    display.drawLine(
        64, 52,
        68, 50,
        SSD1306_WHITE
    );

    display.drawLine(
        68, 50,
        71, 47,
        SSD1306_WHITE
    );
}


// ============================================================
// WARNING FACE
// ============================================================

void drawWarningFace()
{
    display.drawCircle(
        64, 46, 15,
        SSD1306_WHITE
    );

    display.fillCircle(
        59, 42, 2,
        SSD1306_WHITE
    );

    display.fillCircle(
        69, 42, 2,
        SSD1306_WHITE
    );

    display.drawCircle(
        64, 50, 4,
        SSD1306_WHITE
    );
}


// ============================================================
// OLED STARTUP
// ============================================================

void showStartupScreen()
{
    if (!oledOK)
        return;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);

    display.setCursor(21, 8);
    display.println("SMART");

    display.setCursor(21, 30);
    display.println("GATE");

    display.setTextSize(1);

    display.setCursor(32, 52);
    display.println("ALERT SYSTEM");

    display.display();
}


// ============================================================
// OLED CLOSED
// ============================================================

void showClosedScreen()
{
    if (!oledOK)
        return;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);

    display.setCursor(37, 0);
    display.println("GATE STATUS");

    display.setTextSize(2);

    display.setCursor(19, 12);
    display.println("CLOSED");

    drawHappyFace();

    display.display();
}


// ============================================================
// OLED WAITING
// ============================================================

void showWaitingScreen(bool force = false)
{
    if (!oledOK)
        return;

    unsigned long elapsed = millis() - gateOpenedAt;

    int remaining = 0;

    if (elapsed < ALARM_DELAY)
    {
        remaining =
        (ALARM_DELAY - elapsed + 999) / 1000;
    }

    // Only redraw when countdown changes.
    if (!force && remaining == lastCountdown)
        return;

    lastCountdown = remaining;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);

    display.setCursor(36, 0);
    display.println("GATE OPEN");

    display.setTextSize(2);

    display.setCursor(5, 14);
    display.print("ALARM:");

    display.print(remaining);
    display.print("s");

    display.setTextSize(1);

    display.setCursor(27, 39);
    display.println("Please close");

    display.setCursor(38, 53);
    display.println("the gate");

    display.display();
}


// ============================================================
// OLED ALARM
// ============================================================

void showAlarmScreen()
{
    if (!oledOK)
        return;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);

    display.setCursor(19, 0);
    display.println("WARNING");

    display.setTextSize(1);

    display.setCursor(38, 21);
    display.println("GATE OPEN!");

    drawWarningFace();

    display.display();
}


// ============================================================
// OLED OFF
// ============================================================

void showSystemOffScreen()
{
    if (!oledOK)
        return;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);

    display.setCursor(37, 14);
    display.println("OFF");

    display.setTextSize(1);

    display.setCursor(25, 42);
    display.println("System Disabled");

    display.display();
}


// ============================================================
// OLED STATE RESET
// ============================================================

void resetOLEDTracking()
{
    lastOLEDState = gateState;
    lastCountdown = -1;
}


// ============================================================
// LEDS
// ============================================================

void ledsOff()
{
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
}


void closedIndicators()
{
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
}


void openIndicators()
{
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
}


void alarmIndicators()
{
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
}


// ============================================================
// SPEAKER OFF
// ============================================================

void speakerOff()
{
    ledcWrite(
        SPEAKER_CHANNEL,
        0
    );

    sirenRunning = false;
}


// ============================================================
// START SIREN
// ============================================================

void startSiren()
{
    sirenRunning = true;
    sirenHighTone = false;

    lastSirenChange = millis();

    ledcWriteTone(
        SPEAKER_CHANNEL,
        SIREN_LOW_FREQ
    );

    ledcWrite(
        SPEAKER_CHANNEL,
        SPEAKER_DUTY
    );
}


// ============================================================
// UPDATE SIREN
// ============================================================

void updateSiren()
{
    if (!sirenRunning)
        return;

    unsigned long now = millis();

    if (
        now - lastSirenChange >=
        SIREN_STEP_TIME
    )
    {
        lastSirenChange = now;

        sirenHighTone = !sirenHighTone;

        if (sirenHighTone)
        {
            ledcWriteTone(
                SPEAKER_CHANNEL,
                SIREN_HIGH_FREQ
            );
        }
        else
        {
            ledcWriteTone(
                SPEAKER_CHANNEL,
                SIREN_LOW_FREQ
            );
        }

        ledcWrite(
            SPEAKER_CHANNEL,
            SPEAKER_DUTY
        );
    }
}


// ============================================================
// ULTRASONIC
// ============================================================

float readDistanceCM()
{
    digitalWrite(
        TRIG_PIN,
        LOW
    );

    delayMicroseconds(2);

    digitalWrite(
        TRIG_PIN,
        HIGH
    );

    delayMicroseconds(10);

    digitalWrite(
        TRIG_PIN,
        LOW
    );

    unsigned long duration =
    pulseIn(
        ECHO_PIN,
        HIGH,
        30000
    );

    if (duration == 0)
        return -1.0;

    float distance =
    duration * 0.0343 / 2.0;

    if (
        distance < 2.0 ||
        distance > 400.0
    )
    {
        return -1.0;
    }

    return distance;
}


// ============================================================
// POWER SWITCH
// ============================================================

void updatePowerSwitch()
{
    int reading =
    digitalRead(
        POWER_SW_PIN
    );

    if (
        reading !=
        lastSwitchReading
    )
    {
        lastSwitchChange =
        millis();
    }

    if (
        millis() - lastSwitchChange >
        DEBOUNCE_TIME
    )
    {
        if (
            reading !=
            stableSwitchState
        )
        {
            stableSwitchState =
            reading;

            // ======================================================
            // OFF
            // ======================================================

            if (
                stableSwitchState == LOW
            )
            {
                systemEnabled = false;

                speakerOff();
                ledsOff();

                gateState =
                GATE_CLOSED;

                gateOpenedAt = 0;

                resetOLEDTracking();

                showSystemOffScreen();

                Serial.println(
                    "SYSTEM OFF"
                );
            }

            // ======================================================
            // ON
            // ======================================================

            else
            {
                systemEnabled = true;

                gateState =
                GATE_CLOSED;

                gateOpenedAt = 0;

                speakerOff();

                closedIndicators();

                resetOLEDTracking();

                showClosedScreen();

                Serial.println(
                    "SYSTEM ON"
                );
            }
        }
    }

    lastSwitchReading =
    reading;
}


// ============================================================
// SETUP
// ============================================================

void setup()
{
    Serial.begin(115200);

    delay(200);


    // ==========================================================
    // ULTRASONIC
    // ==========================================================

    pinMode(
        TRIG_PIN,
        OUTPUT
    );

    pinMode(
        ECHO_PIN,
        INPUT
    );

    digitalWrite(
        TRIG_PIN,
        LOW
    );


    // ==========================================================
    // LEDS
    // ==========================================================

    pinMode(
        RED_LED_PIN,
        OUTPUT
    );

    pinMode(
        GREEN_LED_PIN,
        OUTPUT
    );

    ledsOff();


    // ==========================================================
    // POWER SWITCH
    // ==========================================================

    pinMode(
        POWER_SW_PIN,
        INPUT_PULLUP
    );


    // ==========================================================
    // SPEAKER
    // ==========================================================

    ledcSetup(
        SPEAKER_CHANNEL,
        2000,
        SPEAKER_PWM_RES
    );

    ledcAttachPin(
        SPEAKER_PIN,
        SPEAKER_CHANNEL
    );

    ledcWrite(
        SPEAKER_CHANNEL,
        0
    );


    // ==========================================================
    // OLED
    // ==========================================================

    Wire.begin(
        OLED_SDA,
        OLED_SCL
    );

    delay(100);

    if (
        display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDR
        )
    )
    {
        oledOK = true;

        Serial.println(
            "OLED OK - ADDRESS 0x3C"
        );

        showStartupScreen();

        delay(1500);

        showClosedScreen();
    }
    else
    {
        oledOK = false;

        Serial.println(
            "OLED ERROR!"
        );
    }


    // ==========================================================
    // READY
    // ==========================================================

    closedIndicators();

    Serial.println();
    Serial.println(
        "================================"
    );

    Serial.println(
        "    SMART GATE ALERT SYSTEM"
    );

    Serial.println(
        "================================"
    );

    Serial.println(
        "SYSTEM READY"
    );

    Serial.println();
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop()
{
    // ==========================================================
    // POWER SWITCH
    // ==========================================================

    updatePowerSwitch();


    // ==========================================================
    // SYSTEM OFF
    // ==========================================================

    if (!systemEnabled)
    {
        speakerOff();
        ledsOff();

        // Don't redraw OLED continuously.
        static bool offShown = false;

        if (!offShown)
        {
            showSystemOffScreen();
            offShown = true;
        }

        delay(20);

        return;
    }


    // ==========================================================
    // CURRENT TIME
    // ==========================================================

    unsigned long now =
    millis();


    // ==========================================================
    // SENSOR
    // ==========================================================

    if (
        now - lastSensorRead >=
        SENSOR_INTERVAL
    )
    {
        lastSensorRead =
        now;

        float newDistance =
        readDistanceCM();


        if (newDistance > 0)
        {
            distanceCM =
            newDistance;

            Serial.print(
                "Distance: "
            );

            Serial.print(
                distanceCM,
                1
            );

            Serial.println(
                " cm"
            );


            // ======================================================
            // CLOSED
            // ======================================================

            if (
                distanceCM <=
                CLOSED_DISTANCE
            )
            {
                if (
                    gateState !=
                    GATE_CLOSED
                )
                {
                    Serial.println(
                        "GATE CLOSED"
                    );
                }

                gateState =
                GATE_CLOSED;

                gateOpenedAt = 0;

                speakerOff();

                closedIndicators();

                lastCountdown = -1;

                showClosedScreen();
            }


            // ======================================================
            // OPEN
            // ======================================================

            else if (
                distanceCM >=
                OPEN_DISTANCE
            )
            {
                if (
                    gateState ==
                    GATE_CLOSED
                )
                {
                    gateState =
                    GATE_OPEN_WAIT;

                    gateOpenedAt =
                    now;

                    speakerOff();

                    openIndicators();

                    lastCountdown = -1;

                    showWaitingScreen(true);

                    Serial.println();
                    Serial.println(
                        "GATE OPEN"
                    );

                    Serial.println(
                        "3 SECOND TIMER STARTED"
                    );
                }
            }

            // ======================================================
            // 20-40 CM
            //
            // Keep previous state.
            // ======================================================
        }
    }


    // ==========================================================
    // WAITING
    // ==========================================================

    if (
        gateState ==
        GATE_OPEN_WAIT
    )
    {
        unsigned long openTime =
        now - gateOpenedAt;

        openIndicators();

        // OLED updates only when
        // countdown number changes.
        showWaitingScreen(false);


        if (
            openTime >=
            ALARM_DELAY
        )
        {
            gateState =
            GATE_OPEN_ALARM;

            alarmIndicators();

            startSiren();

            showAlarmScreen();

            Serial.println();
            Serial.println(
                "================================"
            );

            Serial.println(
                "        !!! WARNING !!!"
            );

            Serial.println(
                "        GATE STILL OPEN"
            );

            Serial.println(
                "        SIREN STARTED"
            );

            Serial.println(
                "================================"
            );
        }
    }


    // ==========================================================
    // ALARM
    // ==========================================================

    if (
        gateState ==
        GATE_OPEN_ALARM
    )
    {
        alarmIndicators();

        updateSiren();

        // OLED stays on alarm screen.
        // No unnecessary redraw.
    }


    delay(5);
}
