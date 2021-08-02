#include "header/drive/dri_file_txt.h"



IFILE_TXT::IFILE_TXT()
{

}

IFILE_TXT::~IFILE_TXT()
{

}

QByteArray IFILE_TXT::read(string _filename)
{
    QByteArray data;
    if(_filename!="")
    {
        QString openfilename=QString::fromStdString(_filename);
        QFile file(openfilename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            data = file.readAll();

        }
        file.close();
    }
    return data;
}

bool IFILE_TXT::write(string _filename,QByteArray _data)
{
    QString fileName=QString::fromStdString(_filename);
    if(fileName!="")
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(_data);
        file.close();
        return true;
    }

    return false;
}
