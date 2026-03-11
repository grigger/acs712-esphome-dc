#include "wcs1700_component.h"

namespace esphome {
namespace wcs1700 {

static const char *const TAG = "wcs1700";


void ACS712Sensor::dump_config() {
  if (this->is_ac_) {
    this->acs_.autoMidPoint(50, 4);
    ESP_LOGCONFIG(TAG, "WCS1700 mode: AC");
  } else {
    this->acs_.autoMidPointDC(100);
    ESP_LOGCONFIG(TAG, "WCS1700 mode: DC");
  }
  
  ESP_LOGCONFIG(TAG,
                "  Pin: %u\n"
                "  MidPoint: %u\n"
                "  Noise mV: %u\n",
                this->pin_,
                this->acs_.getMidPoint(),
                this->acs_.getNoisemV());
  
  if (this->is_ac_) {
    ESP_LOGCONFIG(TAG, "  Noise mV (auto, 0-current AC line): %.2f", this->acs_.mVNoiseLevel(50, 4));
  } else {
    ESP_LOGCONFIG(TAG, "  Noise mV (auto, 0-current DC line): %.2f", this->acs_.mVNoiseLevel(1000, 100));
  }
}

void WCS1700Sensor::setup() {
  this->set_update_interval(3000);
  
  if (this->is_ac_) {
    this->acs_.autoMidPoint(50, 4);
    // ESP_LOGCONFIG(TAG, "WCS1700 mode: AC");
  } else {
    this->acs_.autoMidPointDC(100);
    // ESP_LOGCONFIG(TAG, "WCS1700 mode: DC");
  }

  // ESP_LOGCONFIG(TAG, "  Pin: %u", this->pin_);
  // ESP_LOGCONFIG(TAG, "  MidPoint: %u", this->acs_.getMidPoint());
  // ESP_LOGCONFIG(TAG, "  Noise mV: %u", this->acs_.getNoisemV());

  // if (this->is_ac_) {
  //   ESP_LOGCONFIG(TAG, "  Noise mV (auto, 0-current AC line): %u", this->acs_.mVNoiseLevel(50, 4));
  // } else {
  //   ESP_LOGCONFIG(TAG, "  Noise mV (auto, 0-current DC line): %u", this->acs_.mVNoiseLevel(1000, 100));
  // }
  
}

void ACS712Sensor::update() {
  float average = 0;
  int count = 100;
  float amps;

  if (this->is_ac_) {
    amps = this->acs_.mA_AC(50, 4) / 1000.0;   // Europe mains
  } else {
    // for (int i = 0; i < count; i++) {
    //   average += acs_.mA_DC();
    // }
    
    // amps = average / count / 1000.0;
    amps = this->acs_.mA_DC(100) / 1000.0;
  }
  
  if (absolute_) amps = fabsf(amps);

  float sensor_output_v = analogReadMilliVolts(this->pin_) / 1000.0f;
  
  current_sensor->publish_state(amps);
  power_sensor->publish_state(amps * line_voltage_);
  voltage_sensor->publish_state(sensor_output_v);
}

}  // namespace wcs1700
}  // namespace esphome
