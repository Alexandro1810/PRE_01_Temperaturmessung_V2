#include <stdio.h>
#include <math.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ADC_CHANNEL ADC_CHANNEL_4    // GPIO32
#define ADC_UNIT ADC_UNIT_1

static const char *TAG = "Thermistor";

void app_main(void)
{
<<<<<<< HEAD
    // --- Konstanten ---
    const float Vcc = 3.3;          // Versorgungsspannung
    const float Rref = 10000.0;     // Referenzwiderstand in Ohm
    const float Beta = 3435.0;      // Beta-Wert des NTC
    const float T0 = 298.15;        // 25°C in Kelvin
    const float R0 = 10000.0;       // NTC-Widerstand bei 25°C

=======
    // --- Grundwerte definieren --- supi
    const float U = 3.3;        // Versorgungsspannung
    const float R1 = 10000.0;   // Referenzwiderstand in Ohm
>>>>>>> f3fde9cf89066d901ba459f33c6c50b762818d15
    int adc_raw = 0;
    float Vout = 0.0;
    float Rntc = 0.0;
    float TempK = 0.0;
    float TempC = 0.0;

    // --- ADC konfigurieren ---
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,     // bis ~3.3V
        .bitwidth = ADC_BITWIDTH_12,  // 0–4095
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &config));

    ESP_LOGI(TAG, "Starte Temperaturmessung...");

    // --- Endlosschleife ---
    while (1)
    {
        // ADC-Wert lesen
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_raw));

        // Spannung berechnen
        Vout = ((float)adc_raw / 4095.0f) * Vcc;

        // Widerstand des NTC (Spannungsteiler-Formel)
        if (Vout > 0 && Vout < Vcc)
            Rntc = (Vout * Rref) / (Vcc - Vout);
        else
            Rntc = -1;

        // Temperatur berechnen (Beta-Gleichung)
        if (Rntc > 0)
        {
            // Richtige Beta-Formel
            TempK = Beta / (log(Rntc / R0) + (Beta / T0));
            TempC = TempK - 273.15;
        }
        else
        {
            TempC = -999.0;
        }

        

        // --- Rohwerte + Berechnung ausgeben ---
        ESP_LOGI(TAG,
                 "\nRAW ADC: %d"
                 "\nUT (Volt): %.4f V"
                 "\nR(T): %.2f Ohm"
                 "\nTemperatur: %.2f °C\n",
                 "\nvout: %.2f "
                 adc_raw, Vout, Rntc, TempC, Vout);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Widerstand: Roh 10k
// Thermistor: 1,6V
// Spannung: 3,3V