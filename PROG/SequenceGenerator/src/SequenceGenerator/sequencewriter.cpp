#include "sequencewriter.h"

SequenceWriter::SequenceWriter(const QVector<Movement*> _movements, const QString _filePath){

    movements = _movements;
    filePath = _filePath;

}


bool SequenceWriter::generate(const Format format){

    if(movements.isEmpty()){return false;}

    switch(format){

        case(Format::XML):{return generate_XML();}
        case(Format::CSV):{return generate_CSV();}
        case(Format::ALL):{return generate_XML() && generate_CSV();}
        default:{return false;}

    }




}


bool SequenceWriter::generate_XML(){


    QFile target_file(filePath +".xml");

    const MovementMode movements_mode_format = movements.at(0)->getMode();

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
                        writer.writeTextElement("DIRECTION" , QString::number(static_cast<int>(movements.at(index)->getDirection())));
                        break;
                        }
                        case(MovementMode::COORDINATES):{
                        writer.writeTextElement("X" , QString::number(movements.at(index)->getX()));
                        writer.writeTextElement("Y" , QString::number(movements.at(index)->getY()));
                        writer.writeTextElement("Z" , QString::number(movements.at(index)->getZ()));
                        break;
                        }
                        default:{break;}

                    }

                    writer.writeTextElement("DURATION" , QString::number(movements.at(index)->getDuration()));


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


bool SequenceWriter::generate_CSV(){

    QFile target_file(filePath +".csv");

    const MovementMode movements_mode_format = movements.at(0)->getMode();

    if(target_file.open(QIODevice::WriteOnly)){


        QTextStream writer(&target_file);


        for(int index = 0 ; index < movements.size() ; index++){

            switch(movements_mode_format){

                case(MovementMode::DIRECTION):{
                writer<< static_cast<int>(movements.at(index)->getDirection()) <<",";
                break;
                }
                case(MovementMode::COORDINATES):{
                writer << QString::number(movements.at(index)->getX()) << "," << QString::number(movements.at(index)->getY()) << "," << QString::number(movements.at(index)->getZ()) <<",";
                break;
                }
                default:{break;}

            }
            writer << QString::number(movements.at(index)->getDuration()) << endl;
        }





    }else{return false;}

    target_file.close();

 return true;


}
