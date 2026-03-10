#include "acs712_component.h"

namespace esphome {
namespace acs712 {

static const char *const TAG = "acs712";

void ACS712Sensor::setup() {
  this->set_update_interval(3000);
  
  if (this->is_ac_) {
    this->acs_.autoMidPoint(50, 4);
    ESP_LOGCONFIG(TAG, "ACS712 mode: AC");
  } else {
    this->acs_.autoMidPointDC(100);
    ESP_LOGCONFIG(TAG, "ACS712 mode: DC");
  }

  ESP_LOGCONFIG(TAG, "  Pin: %u", this->_pin);
  ESP_LOGCONFIG(TAG, "  MidPoint: %u", this->acs_.getMidPoint());
  ESP_LOGCONFIG(TAG, "  Noise mV: %u", this->acs_.getNoisemV());
}

void ACS712Sensor::update() {
  float average = 0;
  int count = 100;
  float amps;

  if (this->is_ac_) {
    amps = this->acs_.mA_AC(50, 4);   // Europe mains
  } else {
    // for (int i = 0; i < count; i++) {
    //   average += acs_.mA_DC();
    // }
    
    // amps = average / count / 1000.0;
    amps = this->acs_.mA_DC(100);
  }

  float sensor_output_v = analogReadMilliVolts(this->pin_) / 1000.0f;
  
  current_sensor->publish_state(amps);
  power_sensor->publish_state(amps * line_voltage_);
  voltage_sensor->publish_state(sensor_output_v);
}

}  // namespace acs712
}  // namespace esphome
