# Необходимые пакеты
система Raspberry Pi OS Trixie 32 bit

обновление:
sudo apt update
sudo apt full-upgrade
reboot

sudo apt install
cmake
pigpio
git

для разработки:
qt6-base-dev
qt6-serialport-dev
qt6-websockets-dev
qt6-httpserver-dev
libopencv-core-dev
libopencv-highgui-dev
libopencv-imgcodecs-dev
libopencv-imgproc-dev
libopencv-video-dev
libopencv-videoio-dev
libopencv-objdetect-dev
libopencv-calib3d-dev
libopencv-features2d-dev
libopencv-dnn-dev
libopencv-photo-dev
libopencv-stitching-dev

для пользователя:
libqt6network6
libopencv-core410
libopencv-highgui410
libopencv-imgcodecs410
libopencv-imgproc410
libopencv-videoio410
libopencv-objdetect410
libopencv-calib3d410

# Сборка
запуск qmake:
qmake -d <builld path>/ <project path>/MyProject.pro - 
qmake -o <builld path>/Makefile <project path>/MyProject.pro

сгенерированная qtcreator
<qmake path>/qmake <project path>/MyProject.pro -spec linux-g++ CONFIG+=qtquickcompiler && /bin/make qmake_all

ручная сборка
qmake6 <project path>/MyProject.pro -o <builld path>/Makefile
make -j3
make clean


сборка:
cd <project path>
make -j3

# Сборка OpenCV из исходников (если нельзя скачать готовый пакет)
git clone https://github.com/opencv/opencv.git
cd ./opencv
git checkout <version> - git checkout 4.14.0
mkdir build
cd ./build
mkdir install
sudo cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/home/phoenix/opencv-source/build/install \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_TESTS=OFF \
    -DBUILD_PERF_TESTS=OFF \
    -DBUILD_opencv_python_tests=OFF
sudo make -j3
sudo cmake --install .

# Настройка камеры
в файле /boot/firmware/config.txt
- закомментировать строчку camera_auto_detect
- добавить start_x=1

# Активация COM-порта
sudo raspi-config -> interface options -> serial port -> no -> yes

# Запуск точки доступа
сперва нужно поместить файл Hotspot.nmconnection в /etc/NetworkManager/system-connections/
sudo mv Hotspot.nmconnection /etc/NetworkManager/system-connections/
sudo chown root:root /etc/NetworkManager/system-connections/Hotspot.nmconnection
sudo chmod 600 /etc/NetworkManager/system-connections/Hotspot.nmconnection
sudo systemctl restart NetworkManager
sudo nmcli connection reload
sudo nmcli connection up Hotspot

# Автозапуск
поместить файл launch_server.service в /etc/systemd/system
1) sudo mv launch_server.service /etc/systemd/system/
2) sudo systemctl enable launch_server

# Копирование системы
1) wget -O image-backup https://raw.githubusercontent.com/raspberrypi/documentation/develop/documentation/asciidoc/computers/os/scripts/image-backup
2) sudo chmod +x image-backup

1) копируем систему с помощью Win32 Disk Imager
2) Используя систему Linux сжимаем образ: sudo pishrink.sh phoenix.img phoenix_compact.img

sudo mount /dev/sda1 /mnt/flash
sudo dd if=/dev/mmcblk0 of=/mnt/flash/phoenix.img bs=4M status=progress conv=fsync
sudo dd if=/dev/sde of=/mnt/d/img/phoenix.img bs=4M status=progress conv=fsync

1) git clone https://github.com/billw2/rpi-clone.git
2) cd rpi-clone
3) sudo cp rpi-clone* /usr/local/sbin/
4) sudo rpi-clone sda -v

# Прочее
ls /mnt/flash/lib/arm-linux-gnueabihf/libpxbackend

ls /mnt/flash/lib | grep libpxbackend

find /mnt/flash/lib/ *pxbackend

grep -r "libpxbackend" /mnt/flash/lib

sudo cp /mnt/flash/lib/arm-linux-gnueabihf/qt6/plugins/tls/libqopensslbackend.so ./lib
sudo cp /mnt/flash/lib/arm-linux-gnueabihf/qt6/plugins/tls/libqcertonlybackend.so ./lib

# Сборка pigpio
sudo cmake .. -DCMAKE_INSTALL_PREFIX=/home/mes/pigpio/build/install