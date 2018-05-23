# MQTT weather display
Simple weather and time display, ready to be controlled from MQTT, based on ESP8266, ILI9341 and DS3231. Icon names are based on clientraw format weather station.

**Custom component from clientraw weather station fot HomeAssistant can be found [here](https://github.com/pilotak/Home-AssistantConfig/tree/master/custom_components/sensor)**

HomeAssistant config:

```yaml
automation:
  - id: in_temp_changed
    alias: in temp changed
    trigger:
      platform: state
      entity_id: sensor.myslipo_1_0
    condition:
      - condition: template
        value_template: '{{ trigger.from_state.state != trigger.to_state.state }}'
    action:
      service: mqtt.publish
      data_template:
        topic: "display/in_temp"
        qos: 1
        retain: true
        payload: >-
          {%- set in_temp = states.sensor.myslipo_1_0.state -%}
          {%- if in_temp is not none and in_temp != "unavailable" -%}
            {"temp": {{ in_temp }} }
          {%- endif -%}

  - id: weather_icon_changed
    alias: weather icon changed
    trigger:
      platform: state
      entity_id: sensor.clientraw_symbol
    condition:
      - condition: template
        value_template: '{{ trigger.from_state.state != trigger.to_state.state }}'
    action:
      service: mqtt.publish
      data_template:
        topic: "display/forecast"
        qos: 1
        retain: true
        payload: >-
          {%- set symbol = states.sensor.clientraw_symbol.state -%}
          {%- if symbol is not none and symbol != "unavailable" -%}
            {"icon": {{ symbol }} }
          {%- endif -%}

  - id: out_temp_changed
    alias: out temp changed
    trigger:
      platform: state
      entity_id: sensor.clientraw_temperature_c
    condition:
      - condition: template
        value_template: '{{ trigger.from_state.state != trigger.to_state.state }}'
    action:
      service: mqtt.publish
      data_template:
        topic: "display/out_temp"
        qos: 1
        retain: true
        payload: >-
          {%- set out_temp = states.sensor.clientraw_temperature_c.state -%}
          {%- if out_temp is not none and out_temp != "unavailable" -%}
            {"temp": {{ out_temp }} }
          {%- endif -%}

  - id: out_feel_temp_changed
    alias: out feel temp changed
    trigger:
      platform: state
      entity_id: sensor.clientraw_feels_like_temp_c
    condition:
      - condition: template
        value_template: '{{ trigger.from_state.state != trigger.to_state.state }}'
    action:
      service: mqtt.publish
      data_template:
        topic: "display/out_temp_feel"
        qos: 1
        retain: true
        payload: >-
          {%- set out_temp_feel = states.sensor.myslipo_4_0.state -%}
          {%- if out_temp_feel is not none and out_temp_feel != "unavailable" -%}
            {"temp": {{ out_temp_feel }} }
          {%- endif -%}
```
