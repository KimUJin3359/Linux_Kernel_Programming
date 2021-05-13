cmd_/home/pi/Desktop/device_driver/002/modules.order := {   echo /home/pi/Desktop/device_driver/002/002.ko; :; } | awk '!x[$$0]++' - > /home/pi/Desktop/device_driver/002/modules.order
