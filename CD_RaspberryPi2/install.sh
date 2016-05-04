#! /bin/sh

#sudo raspi-config
#sudo dpkg-reconfigure locales
#sudo dpkg-reconfigure tzdata

ROOT_UID="0"

#Check if run as root
if [ "$UID" -ne "$ROOT_UID" ] ; then
	echo "Root permission is needed. Please command as \"sudo -\""
	exit 1
fi

echo "Start to install Smart Home IoT on Raspberry Pi. It takes 25 minutes...\n"

apt-get update
apt-get -y dist-upgrade
apt-get install -y vim gcc apache2 php5 mariadb-server php5-mysql phpmyadmin git libmysqlclient-dev

mkdir /home/pi/public_html 
chown pi:pi /home/pi/public_html

git clone https://github.com/janghe11/CapstoneDesign_MicroProcessor.git /home/pi/CapstoneDesign_MicroProcessor
chown -R pi:pi /home/pi/CapstoneDesign_MicroProcessor

\cp /home/pi/CapstoneDesign_MicroProcessor/CD_RaspberryPi2/etc/apache2/sites-available/000-default.conf /etc/apache2/sites-available/000-default.conf
chown root:root /etc/apache2/sites-available/000-default.conf

\cp /home/pi/CapstoneDesign_MicroProcessor/CD_RaspberryPi2/etc/apache2/apache2.conf /etc/apache2/sites-available/apache2.conf
chown root:root /etc/apache2/sites-available/apache2.conf 

#echo "network={" >> /etc/wpa_supplicant/wpa_supplicant.conf
#echo "        ssid="Capstone_MP"" >> /etc/wpa_supplicant/wpa_supplicant.conf
#echo "        psk="capstonemp12"" >> /etc/wpa_supplicant/wpa_supplicant.conf
#echo "}" >> /etc/wpa_supplicant/wpa_supplicant.conf

\cp /home/pi/CapstoneDesign_MicroProcessor/CD_RaspberryPi2/home/pi/public_html/netdata /home/pi/public_html/
chown www-data:www-data -R /home/pi/public_html/netdata

\cp /home/pi/CapstoneDesign_MicroProcessor/CD_RaspberryPi2/home/pi/public_html/control.php /home/pi/public_html
chown pi:pi /home/pi/public_html/control.php

apt-get autoremove
apt-get autoclean

echo"Smart Home IoT on Raspberry Pi installated successfully. Try to reboot...\n"

reboot