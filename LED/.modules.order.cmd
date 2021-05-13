cmd_/home/pi/Desktop/device_driver/LED/modules.order := {   echo /home/pi/Desktop/device_driver/LED/LED.ko; :; } | awk '!x[$$0]++' - > /home/pi/Desktop/device_driver/LED/modules.order
