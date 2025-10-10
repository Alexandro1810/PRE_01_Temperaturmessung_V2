#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ADC_CHANNEL ADC_CHANNEL_0   // z. B. GPIO36 (ADC1_CH0)
#define ADC_UNIT ADC_UNIT_1

static const char *TAG = "Thermistor";

void app_main(void)
{
    // --- Grundwerte definieren ---
    const float U = 3.3;        // Versorgungsspannung
    const float R1 = 10000.0;   // Referenzwiderstand in Ohm
    int adc_raw = 0;
    float UT = 0.0;
    float RT = 0.0;

    // --- ADC konfigurieren ---
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,     // Bis ~3.3 V
        .bitwidth = ADC_BITWIDTH_12,  // 0–4095
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &config));

    // --- Endlosschleife ---
    while (1)
    {
        // ADC-Wert lesen
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_raw));

        // Spannung berechnen
        UT = ((float)adc_raw / 4095.0f) * U;

        // Widerstand berechnen (Spannungsteiler-Formel)
        if (UT > 0 && UT < U)
            RT = (UT * R1) / (U - UT);
        else
            RT = -1;

        // Ausgabe
        ESP_LOGI(TAG, "ADC: %d | UT = %.3f V | RT = %.1f Ohm", adc_raw, UT, RT);

        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 Sekunde warten
    }
}
