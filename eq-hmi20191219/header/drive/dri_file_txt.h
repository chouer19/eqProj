#ifndef DRI_FILE_TXT_H
#define DRI_FILE_TXT_H

#include"header/system/pub_types.h"
#include"dri_interface.hpp"
#include<QFile>


class IFILE_TXT:implements IFILE
{
public:
    IFILE_TXT();
    virtual ~IFILE_TXT();
    virtual QByteArray read(string _filename);
    virtual bool write(string _filename,QByteArray _data);
};

#endif // DRI_FILE_TXT_H
