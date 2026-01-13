/*Copyrights (c) 2025 FIA Operating Systems, Neksha DeSilva and www.nekshadesilva.com*/
/*FiaPhy Solar Rad and Heat Flux Derivation through Relative Humidity, Temperature and Barometric Pressure.*/
/*ARM M0 Example*/
/*For more guides and tutorials, visit https://fiaos.org/fiaphy*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <FiaPhy.h>


#define PIN_DHT_REF  2
#define PIN_DHT_FLUX 3
#define DHT_TYPE     DHT11

const float LATITUDE    = 6.9271;
const float LONGITUDE   = 79.8612;
const float ALTITUDE_M  = 10.0;
const float BASE_PRESSURE_HPA = 1014.0;  // Colombo data - testing purposes only. Since DHT11 has no barometric pressure out.

LiquidCrystal_I2C lcd(0x27, 16, 2); 
DHT dht_ref(PIN_DHT_REF, DHT_TYPE);
DHT dht_flux(PIN_DHT_FLUX, DHT_TYPE);

FiaPhy::PhysicsEngine engine;
FiaPhy::INRFilter inr_ref;
FiaPhy::INRFilter inr_flux;

unsigned long last_sample_time = 0;
const unsigned long SAMPLE_INTERVAL = 2000;

void setup() {
   
    Serial.begin(9600);
    while(!Serial);
    Serial.println(F("\n=== SYSTEM BOOT (LITE MODE) ==="));

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("FiaPhy Lib. v1.1.2");
    lcd.setCursor(0, 1);
    lcd.print("Memory = 1");
    delay(1000);

    dht_ref.begin();
    dht_flux.begin();


    engine.setLocation(LATITUDE, LONGITUDE);
    
    FiaPhy::CalibrationParams calib;
    calib.thermal_time_constant_s = 45.0f; 
    calib.solar_absorptivity = 0.90f;
    engine.setCalibration(calib);

    Serial.println(F("Success"));
    lcd.clear();
    lcd.print("Reading data..");
}

void loop() {
    unsigned long current_time = millis();

    if (current_time - last_sample_time >= SAMPLE_INTERVAL) {
        last_sample_time = current_time;


        float t_ref = dht_ref.readTemperature();
        float h_ref = dht_ref.readHumidity();
        float t_flux = dht_flux.readTemperature();
        float h_flux = dht_flux.readHumidity();

        if (isnan(t_ref) || isnan(t_flux)) {
            Serial.println(F("Sensor Error"));
            lcd.setCursor(0, 0); lcd.print("Sensor Err");
            return;
        }

  
        FiaPhy::DifferentialFrame frame;
        float sim_pressure = BASE_PRESSURE_HPA + (random(-10, 10) / 100.0);

     
        frame.ref.temperature_C = t_ref;
        frame.ref.humidity_RH = h_ref;
        frame.ref.pressure_hPa = sim_pressure;
        frame.ref.complete = true;

       
        frame.flux.temperature_C = t_flux;
        frame.flux.humidity_RH = h_flux;
        frame.flux.pressure_hPa = sim_pressure;
        frame.flux.complete = true;

        frame.valid = true; 

      
        FiaPhy::RadiationResult result = engine.compute(frame, inr_ref, inr_flux);

        
        Serial.print(F("Ref:")); Serial.print(t_ref, 1);
        Serial.print(F(" Flux:")); Serial.print(t_flux, 1);
        Serial.print(F(" | GHI:")); Serial.println(result.ghi_Wm2);

        updateLCD(result, t_ref, t_flux);
    }
}

void updateLCD(FiaPhy::RadiationResult result, float t_ref, float t_flux) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sun: ");
    lcd.print((int)result.ghi_Wm2);
    lcd.print(" W/m2");

    lcd.setCursor(0, 1);
    lcd.print("Flx: ");
    lcd.print((int)result.heat_flux_Wm2);
    lcd.print(" W/m2");
}