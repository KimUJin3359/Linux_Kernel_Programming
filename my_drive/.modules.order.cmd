cmd_/home/pi/Desktop/device_driver/my_drive/modules.order := {   echo /home/pi/Desktop/device_driver/my_drive/my_drive.ko; :; } | awk '!x[$$0]++' - > /home/pi/Desktop/device_driver/my_drive/modules.order