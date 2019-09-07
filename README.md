# MQTT weather display
[![Build Status](https://travis-ci.org/pilotak/mqtt-weather-display.svg?branch=master)](https://travis-ci.org/pilotak/mqtt-weather-display)

Simple weather and time display, ready to be controlled from MQTT, based on ESP8266, ILI9341 and DS3231. Icon names are based on clientraw format weather station.

**Custom component from clientraw weather station for HomeAssistant can be found [here](https://github.com/pilotak/homeassistant-clientraw)**

HomeAssistant config:

```yaml
# automations.yaml
- alias: Forecast change
  initial_state: True
  trigger:
    platform: state
    entity_id: sensor.clientraw_symbol
  condition:
    condition: template
    value_template: '{{ trigger.from_state.state != trigger.to_state.state }}'
  action:
    service: mqtt.publish
    data_template:
      topic: "mqtt-weather-display/forecast"
      qos: 1
      retain: true
      payload: >
        {%- set symbol = states.sensor.clientraw_symbol.state -%}
        {%- if symbol is not none and symbol != "unavailable" -%}
          {{ symbol }}
        {%- endif -%}

- alias: Outside temp change
  initial_state: True
  trigger:
    platform: state
    entity_id: sensor.clientraw_temperature
  condition:
    condition: template
    value_template: '{{ trigger.from_state.state != trigger.to_state.state }}'
  action:
    service: mqtt.publish
    data_template:
      topic: "mqtt-weather-display/out-temp"
      qos: 1
      retain: true
      payload: >
        {%- set out_temp = states.sensor.clientraw_temperature.state -%}
        {%- if out_temp is not none and out_temp != "unavailable" -%}
          {{ out_temp }}
        {%- endif -%}

- alias: Inside temp change
  initial_state: True
  trigger:
    platform: state
    entity_id: sensor.myslipotemp_1_0
  condition:
    condition: template
    value_template: '{{ trigger.from_state.state != trigger.to_state.state }}'
  action:
    service: mqtt.publish
    data_template:
      topic: "mqtt-weather-display/in-temp"
      qos: 1
      retain: true
      payload: >
        {%- set in_temp = states.sensor.myslipotemp_1_0.state -%}
        {%- if in_temp is not none and in_temp != "unavailable" -%}
          {{ in_temp }}
        {%- endif -%}

- alias: Outside feel temp change
  initial_state: True
  trigger:
    platform: state
    entity_id: sensor.clientraw_humidex
  condition:
    condition: template
    value_template: '{{ trigger.from_state.state != trigger.to_state.state }}'
  action:
    service: mqtt.publish
    data_template:
      topic: "mqtt-weather-display/out-temp-feel"
      qos: 1
      retain: true
      payload: >
        {%- set out_temp_feel = states.sensor.clientraw_humidex.state -%}
        {%- if out_temp_feel is not none and out_temp_feel != "unavailable" -%}
          {{ out_temp_feel }}
        {%- endif -%}
```
