#!/bin/sh
#Run by doing chmod +x ./install_my_apps.sh, then running.
echo "Install script starting..."
function install_dev
{
  #apt-get update  # To get the latest package lists

  ########## DEVELOPMENT TOOLS ##########
    #libgtest-dev: google test
    #openssh-server: enable ssh connection.
    #valgrind: memory leak check
    #kcachegrind: program efficiency stuff 
  echo "Installing the essentials..."
  apt-get install build-essential git libgtest-dev cmake openssh-server valgrind kcachegrind graphviz

  #Install Google Test framework library
  echo "Configuring Google test libraries..."
  cd /usr/src/gtest
  cmake CmakeLists.txt
  make
  cp *.a /usr/lib
}

function install_qt_dep {
  #QT X11 Dependencies.
  echo "Installing qt dependencies..."
  apt-get install libfontconfig1-dev libfreetype6-dev libx11-dev libxcursor-dev libxext-dev libxfixes-dev libxft-dev libxi-dev libxrandr-dev libxrender-dev xterm -y
  apt-get install libXcomposite-dev libxslt-dev libgstreamer-plugins-base0.10-0 -y

  #Might also need: (:i386 installs 32bit only, I think)
  #apt-get install libgl-mesa:i386 libgl-mesa ia32-libs ia32-libs:i386 lib32z1 lib32ncurses5 lib32z2-1.0  
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
  apt-get install samba
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
  apt-get install xsysinfo dos2unix xfe
}
user=paul
pass=safe
install_dev
#samba_add
#install_qt_dep
#install_WiringPi
#install_tools


#apt-get install <package name> -y
#etc.
echo "Install script completed."