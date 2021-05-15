#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



// Update the button pin mapping based on the board used.
// Here we'ar using a Feather M0 board (SAMD21)
#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5



//============================================================================================
// Vars 
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);


// Vars used in the interrupts handling functions
volatile bool bPressedButtonA = false;
volatile bool bPressedButtonB = false;
volatile bool bPressedButtonC = false;

// Vars used for the logic associated to buttons
int uiMode = 0;
int uiDSLR = 0;
bool bUpdatePrint = false;

// Texts to display to the screen
char* strMode[] = { "Lightning Trigger", "Timelapse", "HDR Mode" };
char* strDSLR[] = { "Canon DSLR", "Nikon DSLR"};



//============================================================================================
// ISR 
void buttonA_ISR()
{
    bPressedButtonA = true;
}

void buttonB_ISR()
{
    bPressedButtonB = true;
}



//============================================================================================
// Function used to update screen text
void UpdateDisplay()
{
    display.clearDisplay();
    display.display();

    // text display tests
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Photoshield r2.1");
    display.println("---------------------");
    //display.println("Lightning Trigger");
    display.println(strMode[uiMode]);
    //display.println("Canon DSLR");
    display.println(strDSLR[uiDSLR]);
    display.setCursor(0, 0);
    display.display(); // actually display all of the above
}




//============================================================================================
// Setup function to initialize all the components 
void setup() 
{
    Serial.begin(9600);

    Serial.println("OLED FeatherWing test");
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

    Serial.println("OLED begun");

    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.display();
    delay(1000);

    // Clear the display buffer.
    display.clearDisplay();
    display.display();

    Serial.println("IO test");

    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);

    // Attach interrupts
    attachInterrupt(digitalPinToInterrupt(BUTTON_A), buttonA_ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_B), buttonB_ISR, FALLING);

    // Display the default text
    UpdateDisplay();
}



//============================================================================================
// Loop function
void loop() 
{
    // Check if button were pressed, and if one was pressed, update the logic
    if (bPressedButtonA)
    {
        uiMode = (uiMode + 1) % 3;
        bPressedButtonA = false;
        bUpdatePrint = true;
    }


    if (bPressedButtonB)
    {
        uiDSLR = (uiDSLR + 1) % 2;
        bPressedButtonB = false;
        bUpdatePrint = true;
    }


    // Check if the screen content must be updated 
    if (bUpdatePrint)
    {
        UpdateDisplay();
        bUpdatePrint = false;
    }


    delay(10);
    display.display();
}   
