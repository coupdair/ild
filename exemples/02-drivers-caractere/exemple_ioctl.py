#! /usr/bin/python

import fcntl, os;

f=os.open("/dev/cdrom", os.O_NONBLOCK);
fcntl.ioctl(f, 0x5309, 0); #CDROMEJECT
#fcntl.ioctl(f, 0x5319, 0); #CLOSE TRAY

