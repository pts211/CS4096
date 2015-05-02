#!/bin/sh
#Run by doing chmod +x ./install_my_apps.sh, then running.
echo "Install script starting..."
function install_dev
{
  #apt-get -y update  # To get the latest package lists

  ########## DEVELOPMENT TOOLS ##########
    #libgtest-dev: google test
    #openssh-server: enable ssh connection.
    #valgrind: memory leak check
    #kcachegrind: program efficiency stuff 
  echo "Installing the essentials..."
  apt-get -y install build-essential git libgtest-dev cmake openssh-server valgrind kcachegrind graphviz doxygen gcc-4.8 g++-4.8 
  
  #Install Google Test framework library
  echo "Configuring Google Test libraries..."
  cd /usr/src/gtest
  cmake CmakeLists.txt
  make
  cp *.a /usr/lib
  echo "Google Test configuration complete."
  
  #Install libftdi library
  echo "Installing LibFTDI library..."
  apt-get -y install libusb-1.0-0-dev libconfuse-dev swig python-dev libboost-all-dev libftdi-dev
  
  #This might not be needed.
  #cd ~
  #mkdir libftdi
  #cd libftdi
  #git clone git://developer.intra2net.com/libftdi
  #cd libftdi
  #mkdir build
  #cd build
  #cmake  -DCMAKE_INSTALL_PREFIX="/usr" ../
  #make
  #sudo make install

  echo "LibFTDI instal complete."
}

function install_libftdi {
  #Install libftdi library
  echo "Installing LibFTDI library..."
  apt-get -y install libusb-1.0-0-dev libconfuse-dev swig python-dev libboost-all-dev libftdi-dev
  
  cd ~
  mkdir libftdi
  cd libftdi
  git clone git://developer.intra2net.com/libftdi
  cd libftdi
  mkdir build
  cd build
  cmake  -DCMAKE_INSTALL_PREFIX="/usr" ../
  make
  sudo make install

  echo "LibFTDI instal complete."

}

function install_kinect_stuff {
  #Install libftdi library
  echo "Installing Kinect Utilities library..."
  apt-get -y install gstreamer1.0
  

  echo "Kinect Utilities install complete."

}

function install_qt_dep {
  #QT X11 Dependencies.
  echo "Installing qt dependencies..."
  apt-get -y install libfontconfig1-dev libfreetype6-dev libx11-dev libxcursor-dev libxext-dev libxfixes-dev libxft-dev libxi-dev libxrandr-dev libxrender-dev xterm
  apt-get -y install libXcomposite-dev libxslt-dev libgstreamer-plugins-base0.10-0

  #Might also need: (:i386 installs 32bit only, I think)
  #apt-get -y install libgl-mesa:i386 libgl-mesa ia32-libs ia32-libs:i386 lib32z1 lib32ncurses5 lib32z2-1.0  
}

function download_qt {
  echo "Downloading QT 5.3.2..."
  cd ~
  wget http://download.qt.io/official_releases/qt/5.3/5.3.2/qt-opensource-linux-x64-5.3.2.run
  chmod +x ./qt-opensource-linux-x64-5.3.2.run
}

function install_WiringPi {
  echo "Installing WiringPi libraries..."
  cd ~
  git clone https://github.com/WiringPi/WiringPi.git
  cd WiringPi
  ./build
  
  echo "Verifying WiringPi install..."
  gpio -v
  gpio readall
  
  echo "WiringPi install complete."
}

function samba_add {
  echo "Installing SAMBA..."
  apt-get -y install samba
   echo "Creating SAMBA user..."
   (echo "$pass"; echo "$pass") | smbpasswd -s -a $user
    #Edit smb.conf to add share.

    echo "Now configure the samba share with:"
    echo "sudo nano /etc/samba/smb.conf"

    #restart samba
    echo "Restart samba:"
    echo "service smbd restart"
    #check for errors in conf file
    echo "and then test for errors:"
    echo "testparam"
}

function install_tools {
  ########## USEFUL THINGS ##########
  #xsysinfo: system resource view.
  echo "Installing useful tools..."
  apt-get -y install xsysinfo dos2unix xfe
}
user=pi
pass=pi
#install_dev
#install_libftdi
#install_kinect_stuff
#install_qt_dep
#download_qt
#install_WiringPi
samba_add
#install_tools


#apt-get -y install <package name> -y
#etc.
echo "Install script completed."