#include "sequencewriter.h"

SequenceWriter::SequenceWriter(const QVector<Movement> &_movements, const QString _filePath){

    movements = _movements;
    filePath = _filePath;

}
SequenceWriter::SequenceWriter(const std::vector<Movement> &_movements, const std::string _filePath){

    movements = QVector<Movement>::fromStdVector(_movements);
    filePath = QString::fromStdString(_filePath);

}

/**************************************************************/

void SequenceWriter::remove(const QString &path){

    QFile file(path);
    if(file.exists()){file.remove();}

}

/**************************************************************/

bool SequenceWriter::generate(const Format format, const bool withOverride){

    if(movements.isEmpty()){return false;}

    switch(format){

        case(Format::XML):{return generate_XML(withOverride);}
        case(Format::CSV):{return generate_CSV(withOverride);}
        case(Format::ALL):{return generate_XML(withOverride) && generate_CSV(withOverride);}
        default:{return false;}

    }

}

/**************************************************************/

bool SequenceWriter::generate_XML(const bool withOverride){


    QFile target_file(filePath +".xml");

    if(withOverride){
        if(target_file.exists()){target_file.remove();}
    }

    const MovementMode movements_mode_format = movements.at(0).getMode();

    if(target_file.open(QIODevice::WriteOnly)){

        QXmlStreamWriter writer(&target_file);

        writer.setAutoFormatting(true);
        writer.writeStartDocument();

        //<SEQUENCE>
        writer.writeStartElement("SEQUENCE");
            writer.writeTextElement("FILE_FORMAT" , QString::number(static_cast<int>(movements_mode_format)));

            //<MOVEMENTS>
            writer.writeStartElement("MOVEMENTS");

                for(int index = 0 ; index < movements.size() ; index++){

                    //<MOVEMENT>
                    writer.writeStartElement("MOVEMENT");

                    switch(movements_mode_format){

                        case(MovementMode::DIRECTION):{
                        writer.writeTextElement("DIRECTION" , QString::number(static_cast<int>(movements.at(index).getDirection())));
                        break;
                        }
                        case(MovementMode::COORDINATES):{
                        writer.writeTextElement("X" , QString::number(movements.at(index).getX()));
                        writer.writeTextElement("Y" , QString::number(movements.at(index).getY()));
                        writer.writeTextElement("Z" , QString::number(movements.at(index).getZ()));
                        break;
                        }
                        default:{break;}

                    }

                    writer.writeTextElement("DURATION" , QString::number(movements.at(index).getDuration()));


                    writer.writeEndElement();
                    //</MOVEMENT>
                }

            writer.writeEndElement();
            //<MOVEMENTS>

        writer.writeEndElement();
        //</SEQUENCE>

    }else{return false;}

    target_file.close();

 return true;

}

/**************************************************************/

bool SequenceWriter::generate_CSV(const bool withOverride){

    QFile target_file(filePath +".csv");

    if(withOverride){
        if(target_file.exists()){target_file.remove();}
    }

    const MovementMode movements_mode_format = movements.at(0).getMode();

    if(target_file.open(QIODevice::WriteOnly)){


        QTextStream writer(&target_file);


        for(int index = 0 ; index < movements.size() ; index++){

            switch(movements_mode_format){

                case(MovementMode::DIRECTION):{
                writer<< static_cast<int>(movements.at(index).getDirection()) <<",";
                break;
                }
                case(MovementMode::COORDINATES):{
                writer << QString::number(movements.at(index).getX()) << "," << QString::number(movements.at(index).getY()) << "," << QString::number(movements.at(index).getZ()) <<",";
                break;
                }
                default:{break;}

            }
            writer << QString::number(movements.at(index).getDuration()) << endl;
        }





    }else{return false;}

    target_file.close();

 return true;


}
/**************************************************************/

void SequenceWriter::setFile(const QString &path){filePath = path;}
