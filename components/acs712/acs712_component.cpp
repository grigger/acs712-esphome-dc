#include "acs712_component.h"

namespace esphome {
namespace acs712 {

static const char *const TAG = "acs712";

void ACS712Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "ACS712\n"
                "  Mode: %s\n"
                "  Pin: %u\n"
                "  MidPoint: %u (%s)\n"
                "  Noise mV: %u (%s)\n"
                "  Noise Suppress: (%s)\n",
                (this->is_ac_ ? "AC" : "DC"),
                this->pin_,
                this->acs_.getMidPoint(),
                (this->manual_midpoint_set_ ? "manual" : "auto"),
                this->acs_.getNoisemV(),
                (this->manual_noise_set_ ? "manual" : "auto"),
                (this->noise_suppress_ ? "yes" : "no"));
  
  if (this->is_ac_) {
    ESP_LOGCONFIG(TAG, "  Noise mV (auto, 0-current AC line): %.2f", this->acs_.mVNoiseLevel(this->freq_ac, this->samples_ac));
  } else {
    ESP_LOGCONFIG(TAG, "  Noise mV (auto, 0-current DC line): %.2f", this->acs_.mVNoiseLevel(this->freq_dc, this->samples_dc));
  }
}

void ACS712Sensor::setup() {
  this->set_update_interval(3000);

  if (!this->manual_midpoint_set_) {
    if (this->is_ac_) {
      this->acs_.autoMidPoint(this->freq_ac, this->samples_ac);
    } else {
      this->acs_.autoMidPointDC(this->samples_dc);
    }
  }

  if (!this->manual_noise_set_) {
    if (this->is_ac_) {
      this->acs_.setNoisemV(this->acs_.mVNoiseLevel(this->freq_ac, this->samples_ac));
    } else {
      this->acs_.setNoisemV(this->acs_.mVNoiseLevel(this->freq_dc, this->samples_dc));
    }
  }
}

void ACS712Sensor::update() {
  float amps;

  if (this->is_ac_) {
    amps = this->acs_.mA_AC(this->freq_ac, this->samples_ac) / 1000.0;
  } else {
    amps = this->acs_.mA_DC(this->samples_dc) / 1000.0;
  }
  
  if (absolute_) amps = fabsf(amps);

  float sensor_output_v = analogReadMilliVolts(this->pin_) / 1000.0f;
  
  current_sensor->publish_state(amps);
  power_sensor->publish_state(amps * line_voltage_);
  voltage_sensor->publish_state(sensor_output_v);
}


}  // namespace acs712
}  // namespace esphome
