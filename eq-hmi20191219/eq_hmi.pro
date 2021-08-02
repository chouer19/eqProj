#-------------------------------------------------
#
# Project created by QtCreator 2019-08-05T15:12:06
#
#-------------------------------------------------

QT       += core gui
QT += sql
QMAKE_LFLAGS += -no-pie

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eq_hmi
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    sources/system/sys_abstract.cpp \
    sources/system/sys_task.cpp \
    sources/system/pub_types.cpp \
    sources/system/main_p0.cpp \
    sources/system/main_p1.cpp \
    sources/system/main_p2.cpp \
    sources/drive/dri_abstract.cpp \
    sources/drive/dri_serialport.cpp \
    sources/drive/pub_serialport.cpp \
    sources/protocol/pro_vehicle.cpp \
    sources/protocol/pro_dgps.cpp \
    sources/protocol/pro_abstract.cpp \
    sources/algorithm/alg_abstract.cpp \
    sources/algorithm/alg_latctrl.cpp \
    sources/algorithm/alg_lonctrl.cpp \
    sources/algorithm/alg_speed_plan.cpp \
    sources/algorithm/alg_path_plan.cpp \
    mywidget.cpp \
    dialog_drive_login.cpp \
    dialog_error_declare.cpp \
    dialog_delay_request.cpp \
    dialog_enter_code.cpp \
    dialog_confirm.cpp \
    dialog_system_config.cpp \
    sources/drive/dri_file_txt.cpp \
    sources/system/sys_telecom.cpp \
    qdisplaywidget2.cpp \
    qdisplaywidget1.cpp \
    sources/protocol/pro_gnss_receiver.cpp \
    sources/system/ser_outside_xc/eqDriving/sensor/GpsManager.cpp \
    sources/system/ser_outside_xc/eqDriving/sensor/Nmea.cpp \
    sources/system/ser_outside_xc/eqDriving/util/Logger.cpp \
    sources/system/ser_outside_xc/eqDriving/util/Strlib.cpp \
    sources/system/ser_outside_xc/eqDriving/v2x/eqCloudPackets.cpp \
    sources/system/ser_outside_xc/eqDriving/v2x/eqPacketProtocol.cpp \
    sources/system/ser_outside_xc/eqDriving/v2x/eqSingletonClient.cpp \
    sources/system/ser_outside_xc/eqDriving/v2x/SingletonAuxiliaryClient.cpp \
    sources/system/ser_outside_xc/thirdParty/geographicLib/Geocentric.cpp \
    sources/system/ser_outside_xc/thirdParty/geographicLib/Geodesic.cpp \
    sources/system/ser_outside_xc/thirdParty/geographicLib/GeodesicLine.cpp \
    sources/system/ser_outside_xc/thirdParty/geographicLib/LocalCartesian.cpp \
    sources/system/ser_outside_xc/thirdParty/geographicLib/Math.cpp \
    sources/system/ser_outside_xc/thirdParty/simdjson/simdjson.cpp \
    sources/system/ser_outside_xc/thirdParty/sqlite/sqlite3.c \
    sources/system/ser_outside_xc/thirdParty/zlib/adler32.c \
    sources/system/ser_outside_xc/thirdParty/zlib/compress.c \
    sources/system/ser_outside_xc/thirdParty/zlib/crc32.c \
    sources/system/ser_outside_xc/thirdParty/zlib/deflate.c \
    sources/system/ser_outside_xc/thirdParty/zlib/gzclose.c \
    sources/system/ser_outside_xc/thirdParty/zlib/gzlib.c \
    sources/system/ser_outside_xc/thirdParty/zlib/gzread.c \
    sources/system/ser_outside_xc/thirdParty/zlib/gzwrite.c \
    sources/system/ser_outside_xc/thirdParty/zlib/infback.c \
    sources/system/ser_outside_xc/thirdParty/zlib/inffast.c \
    sources/system/ser_outside_xc/thirdParty/zlib/inflate.c \
    sources/system/ser_outside_xc/thirdParty/zlib/inftrees.c \
    sources/system/ser_outside_xc/thirdParty/zlib/trees.c \
    sources/system/ser_outside_xc/thirdParty/zlib/uncompr.c \
    sources/system/ser_outside_xc/thirdParty/zlib/zutil.c \
    sources/system/ser_outside_xc/eqDriving/sensor/Bestposa.cpp \
    sources/system/ser_outside_xc/eqDriving/sensor/Bestvela.cpp \
    sources/system/ser_outside_xc/eqDriving/sensor/Heading2a.cpp \
    sources/system/ser_outside_xc/eqDriving/sensor/Time.cpp \
    dialog_unload_place_scan.cpp \
    dialog_confirm_load.cpp \
    dialog_choose_load_mode.cpp \
    dialog_truck_info.cpp

HEADERS += \
        mainwindow.h \
    pub_globals.h \
    header/system/pub_types.h \
    header/system/pub_globals.h \
    header/system/rtwtypes.h \
    header/system/sys_abstract.hpp \
    header/system/sys_task.hpp \
    header/system/sys_interface.hpp \
    header/system/typedefs.h \
    header/system/main_linux.h \
    header/drive/dri_interface.hpp \
    header/drive/dri_abstract.hpp \
    header/drive/dri_serialport.hpp \
    header/drive/pub_serialport.hpp \
    header/protocol/pro_interface.h \
    header/protocol/pro_vehicle.h \
    header/protocol/pro_dgps.hpp \
    header/protocol/pro_abstract.hpp \
    header/algorithm/alg_abstract.hpp \
    header/algorithm/alg_interface.hpp \
    header/algorithm/alg_latctrl.hpp \
    header/algorithm/alg_lonctrl.hpp \
    header/algorithm/alg_speed_plan.h \
    header/algorithm/alg_path_plan.hpp \
    mywidget.h \
    dialog_drive_login.h \
    dialog_error_declare.h \
    dialog_delay_request.h \
    dialog_enter_code.h \
    dialog_confirm.h \
    dialog_system_config.h \
    header/drive/dri_file_txt.h \
    header/system/sys_telecom.h \
    qdisplaywidget2.h \
    qdisplaywidget1.h \
    header/protocol/pro_gnss_receiver.h \
    header/system/ser_outside_xc/eqDriving/common/SingletonT.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/GpsManager.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/Nmea.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/NmeaPrivate.hpp \
    header/system/ser_outside_xc/eqDriving/serial/SimpleSerial.hpp \
    header/system/ser_outside_xc/eqDriving/util/Logger.hpp \
    header/system/ser_outside_xc/eqDriving/util/Strlib.hpp \
    header/system/ser_outside_xc/eqDriving/v2x/eqCloudPackets.hpp \
    header/system/ser_outside_xc/eqDriving/v2x/eqPacketProtocol.hpp \
    header/system/ser_outside_xc/eqDriving/v2x/eqSingletonClient.hpp \
    header/system/ser_outside_xc/eqDriving/v2x/SingletonAuxiliaryClient.hpp \
    header/system/ser_outside_xc/thirdParty/geographicLib/Config.h \
    header/system/ser_outside_xc/thirdParty/geographicLib/Constants.hpp \
    header/system/ser_outside_xc/thirdParty/geographicLib/Geocentric.hpp \
    header/system/ser_outside_xc/thirdParty/geographicLib/Geodesic.hpp \
    header/system/ser_outside_xc/thirdParty/geographicLib/GeodesicLine.hpp \
    header/system/ser_outside_xc/thirdParty/geographicLib/LocalCartesian.hpp \
    header/system/ser_outside_xc/thirdParty/geographicLib/Math.hpp \
    header/system/ser_outside_xc/thirdParty/gzip/compress.hpp \
    header/system/ser_outside_xc/thirdParty/gzip/config.hpp \
    header/system/ser_outside_xc/thirdParty/gzip/decompress.hpp \
    header/system/ser_outside_xc/thirdParty/gzip/utils.hpp \
    header/system/ser_outside_xc/thirdParty/gzip/version.hpp \
    header/system/ser_outside_xc/thirdParty/simdjson/simdjson.hpp \
    header/system/ser_outside_xc/thirdParty/sqlite/sqlite3.h \
    header/system/ser_outside_xc/thirdParty/zlib/crc32.h \
    header/system/ser_outside_xc/thirdParty/zlib/deflate.h \
    header/system/ser_outside_xc/thirdParty/zlib/gzguts.h \
    header/system/ser_outside_xc/thirdParty/zlib/inffast.h \
    header/system/ser_outside_xc/thirdParty/zlib/inffixed.h \
    header/system/ser_outside_xc/thirdParty/zlib/inflate.h \
    header/system/ser_outside_xc/thirdParty/zlib/inftrees.h \
    header/system/ser_outside_xc/thirdParty/zlib/trees.h \
    header/system/ser_outside_xc/thirdParty/zlib/zconf.h \
    header/system/ser_outside_xc/thirdParty/zlib/zlib.h \
    header/system/ser_outside_xc/thirdParty/zlib/zutil.h \
    header/system/ser_outside_xc/eqDriving/sensor/Bestposa.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/BestposaPrivate.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/Bestvela.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/BestvelaPrivate.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/Heading2a.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/Heading2aPrivate.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/Time.hpp \
    header/system/ser_outside_xc/eqDriving/sensor/TimePrivate.hpp \
    dialog_unload_place_scan.h \
    dialog_confirm_load.h \
    dialog_choose_load_mode.h \
    dialog_truck_info.h

FORMS += \
        mainwindow.ui \
    dialog_drive_login.ui \
    dialog_error_declare.ui \
    dialog_delay_request.ui \
    dialog_enter_code.ui \
    dialog_confirm.ui \
    dialog_system_config.ui \
    dialog_unload_place_scan.ui \
    dialog_confirm_load.ui \
    dialog_choose_load_mode.ui \
    dialog_truck_info.ui


DISTFILES += \
    resource/logo.png

RESOURCES += \
    resource/resource.qrc


INCLUDEPATH += -L./usr/local/include
DEPENDPATH += -L./usr/local/include

LIBS += /usr/local/lib/libboost_date_time.a
LIBS += /usr/local/lib/libboost_chrono.a
LIBS += /usr/local/lib/libboost_thread.a
LIBS += /usr/local/lib/libboost_system.a
LIBS += /usr/local/lib/libboost_chrono.a
LIBS += -L./usr/local/lib/ -lcppnetlib-client-connections
LIBS += -L./usr/local/lib/ -lcppnetlib-network-uri

INCLUDEPATH += -L./usr/lib/x86_64-linux-gnu
DEPENDPATH += -L./usr/lib/x86_64-linux-gnu
LIBS += -L./usr/lib/x86_64-linux-gnu -ldl
LIBS += -L./usr/lib/x86_64-linux-gnu -lssl3
