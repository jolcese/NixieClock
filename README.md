
```
curl -X POST "http://10.30.0.249/config?nixieMode=0"
curl -X POST "http://10.30.0.249/config?ledMode=0"
curl -X POST "http://10.30.0.249/reboot"

// uint8_t ledMode = 1; // 0 - Rainbow / 1 - Fixed Orange
// uint8_t nixieMode = 1; // 0 - Off / 1 - Clock / 2 - all digits 0-9 / 3 - every individual bit
```



Todo

- Timezone
- Auto on/off

Done

- Fix EEPROM
