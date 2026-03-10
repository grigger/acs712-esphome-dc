#ifndef ACS712_COMPONENT_H
#define ACS712_COMPONENT_H

#include "ACS712.h"
#include "esphome.h"
#include "esphome/core/component.h"       // Contiene la definición de PollingComponent
#include "esphome/components/sensor/sensor.h"  // Contiene la definición de sensor::Sensor


namespace esphome {
namespace acs712 {

class ACS712Sensor : public PollingComponent {
   public:
    ACS712Sensor(uint8_t pin, float voltage, uint16_t adc_bits, float mV_per_amp, float line_voltage)
        : PollingComponent(3000),
           pin_(pin),
           acs_(pin, voltage, adc_bits, mV_per_amp),
           line_voltage_(line_voltage),
           is_ac_(false),
           current_sensor(nullptr),
           power_sensor(nullptr),
           voltage_sensor(nullptr) {}

    void setup() override;
    void update() override;

    void set_is_ac(bool is_ac) { this->is_ac_ = is_ac; }
    void set_noisemV(float noisemV) { acs_.setNoisemV(noisemV); }
    void set_mid_point(uint16_t mid_point) { acs_.setMidPoint(mid_point); }

    // Nuevos setters para los sensores configurados desde YAML
    void set_current_sensor(sensor::Sensor *sensor) { current_sensor = sensor; }
    void set_power_sensor(sensor::Sensor *sensor) { power_sensor = sensor; }
    void set_voltage_sensor(sensor::Sensor *sensor) { voltage_sensor = sensor; }

   private:
    ACS712 acs_;
    uint8_t pin_;
    float line_voltage_;
    bool is_ac_;
    sensor::Sensor *current_sensor;
    sensor::Sensor *power_sensor;
    sensor::Sensor *voltage_sensor;
};

}  // namespace acs712
}  // namespace esphome

#endif
