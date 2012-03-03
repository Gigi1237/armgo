AUTOMAKE_OPTIONS = foreign  
  

SUBDIRS = common
SUBDIRS += window
SUBDIRS += boot
SUBDIRS += cpu
SUBDIRS += elf
SUBDIRS += memory
SUBDIRS += s3c2410
SUBDIRS += s3c2410/device/gpio
SUBDIRS += s3c2410/device/uart
SUBDIRS += s3c2410/device/watchdog
SUBDIRS += s3c2410/device/sdram
SUBDIRS += thread
SUBDIRS += armgo
#above the sequence is very very imortant!!!



INCLUDES = $(top_srcdir)/armgo -I$(top_srcdir)/boot -I$(top_srcdir)/common -I$(top_srcdir)/cpu -I$(top_srcdir)/elf -I$(top_srcdir)/memory -I$(top_srcdir)/s3c2410 -I$(top_srcdir)/thread -I$(top_srcdir)/window


export INCLUDES
