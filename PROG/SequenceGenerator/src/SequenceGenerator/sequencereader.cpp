#include "sequencereader.h"

#include <QDebug>

SequenceReader::SequenceReader(const QString _path){

    path = _path;
}

/**************************************************************/

bool SequenceReader::isValid(const QString path){

    return (isValid_XML(path) || isValid_CSV(path));


}


bool SequenceReader::isValid_XML(const QString path){

    bool success = true;

    QFile target_file(path);

    if(target_file.open(QIODevice::ReadOnly)){

        QXmlStreamReader reader(&target_file);

        reader.readNext();

        //<SEQUENCE>
        reader.readNextStartElement();
        qDebug() << reader.name() << "SEQUENCE";
        if(reader.name() != "SEQUENCE"){success = false;}

        //<FILE_FORMAT>
        reader.readNextStartElement();
        qDebug() << reader.name() << "FILE_FORMAT";
        const MovementMode movements_mode_format = static_cast<MovementMode>(reader.readElementText().toInt());
        if(movements_mode_format!= MovementMode::DIRECTION && movements_mode_format != MovementMode::COORDINATES){success = false;}

        //<MOVEMENTS>
         reader.readNextStartElement();
         qDebug() << reader.name() << "MOVEMENTS";
         if(reader.name() != "MOVEMENTS"){success = false;}

           //<MOVEMENT>
            while(reader.readNextStartElement() && success){

                qDebug() << reader.name() << "MOVEMENT";
                if(reader.name() != "MOVEMENT"){success = false; break;}


                switch(movements_mode_format){
                    case(MovementMode::DIRECTION):{
                    //<DIRECTION>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "DIRECTION";
                    if(reader.name()!= "DIRECTION"){success = false; break;}
                    const Direction dir = static_cast<Direction>(reader.readElementText().toInt());
                    if(dir != Direction::NONE &&
                            dir != Direction::FORWARD &&
                            dir != Direction::BARCKWARDS &&
                            dir != Direction::RIGHT &&
                            dir != Direction::LEFT){success = false; break;}
                    //</DIRECTION>

                    //<DURATION>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "DURATION";
                    if(reader.name() != "DURATION"){success = false ; break;}
                    //</DURATION>

                    break;
                    }
                    case(MovementMode::COORDINATES):{

                    //<X>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "X";
                    if(reader.name() != "X"){success = false ; break;}
                    reader.readElementText();
                    //</X>

                    //<Y>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "Y";
                    if(reader.name() != "Y"){success = false ; break;}
                    reader.readElementText();
                    //</Y>

                    //<Z>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "Z";
                    if(reader.name() != "Z"){success = false ; break;}
                    reader.readElementText();
                    //</Z>

                    //<DURATION>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "DURATION";
                    if(reader.name() != "DURATION"){success = false ; break;}
                    reader.readElementText();
                    //</DURATION>
                    break;
                    }
                    default:{success = false;} //Not good news if this is reached
                }

                reader.readNextStartElement();

            }
            //</MOVEMENT>
         //</MOVEMENTS>



        //</FILE_FORMAT>

    }
    else{success = false;}

    target_file.close();


    return success;
}



bool SequenceReader::isValid_CSV(const QString path){

    bool success = true;
    QFile target_file(path);

    if(target_file.open(QIODevice::ReadOnly)){

        QTextStream reader(&target_file);

        while(!reader.atEnd()){

            const int data_size = reader.readLine().split(',').size();
            success = (data_size == 2 || data_size == 4);
            if(!success){break;}

        }

    }

    target_file.close();

    return success;
}




Sequence* SequenceReader::readSequence(const QString path){

    if(isValid_XML(path)){return readSequence_XML(path);}
    if(isValid_CSV(path)){return readSequence_CSV(path);}
    qDebug() << "not valid";
    return new Sequence();


}

Sequence* SequenceReader::readSequence(){return readSequence(path);}

Sequence* SequenceReader::readSequence_XML(const QString path){

    Sequence *seq = new Sequence();

    QFile target_file(path);

    if(target_file.open(QIODevice::ReadOnly)){

        QXmlStreamReader reader(&target_file);

        reader.readNext();

        //<SEQUENCE>
        reader.readNextStartElement();
        qDebug() << reader.name() << "SEQUENCE";

        //<FILE_FORMAT>
        reader.readNextStartElement();
        qDebug() << reader.name() << "FILE_FORMAT";
        const MovementMode movements_mode_format = static_cast<MovementMode>(reader.readElementText().toInt());

        //<MOVEMENTS>
         reader.readNextStartElement();
         qDebug() << reader.name() << "MOVEMENTS";

           //<MOVEMENT>
            while(reader.readNextStartElement()){

                qDebug() << reader.name() << "MOVEMENT";

                Movement * mov = new Movement();
                mov->setMode(movements_mode_format , false);

                switch(movements_mode_format){
                    case(MovementMode::DIRECTION):{
                    //<DIRECTION>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "DIRECTION";
                    const Direction dir = static_cast<Direction>(reader.readElementText().toInt());
                    mov->setDirection(dir);
                    //</DIRECTION>

                    //<DURATION>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "DURATION";
                    const uint64_t duration = reader.readElementText().toInt();
                    mov->setDuration(duration);
                    //</DURATION>

                    break;
                    }
                    case(MovementMode::COORDINATES):{

                    //<X>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "X";
                    const int x = reader.readElementText().toInt();
                    mov->setX(x);
                    //</X>

                    //<Y>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "Y";
                    const int y = reader.readElementText().toInt();
                    mov->setY(y);
                    //</Y>

                    //<Z>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "Z";
                    const int z = reader.readElementText().toInt();
                    mov->setZ(z);
                    //</Z>

                    //<DURATION>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "DURATION";
                    const uint64_t duration = reader.readElementText().toInt();
                    mov->setDuration(duration);
                    //</DURATION>
                    break;
                    }
                    default:{mov->setMode(MovementMode::COORDINATES , true);} //Not good news if this is reached
                }

                seq->addMovement(mov);
                reader.readNextStartElement();

            }
            //</MOVEMENT>
         //</MOVEMENTS>



        //</FILE_FORMAT>
    }

    target_file.close();

    return seq;


}

Sequence* SequenceReader::readSequence_CSV(const QString path){

    qDebug() << "readSequence_CSV";

    Sequence *seq = new Sequence();


    QFile target_file(path);

    if(target_file.open(QIODevice::ReadOnly)){

        qDebug() << "open";

        QTextStream reader(&target_file);

        qDebug() << "while begin";
        while(!reader.atEnd()){

            const QVector<QString> item = reader.readLine().split(',').toVector();
             seq->addMovement(constructMovement_CSV(item));

        }

        qDebug() << "while end";


    }else{qDebug() << "no open";}


    target_file.close();

    return seq;
}

Movement* SequenceReader::constructMovement_CSV(const QVector<QString>& data ){

    Movement *mov = new Movement();
    switch(data.size()){

        case(2):{
            mov->setMode(MovementMode::DIRECTION , false);
            mov->setDirection(static_cast<Direction>(data.at(0).toInt()));
            break;
        }

        case(4):{
        qDebug() << "mov_coor";;
            mov->setMode(MovementMode::COORDINATES , false);
            mov->setX(data.at(0).toInt());
            mov->setY(data.at(1).toInt());
            mov->setZ(data.at(2).toInt());
            mov->setDuration(static_cast<uint64_t>(data.at(3).toInt()));
            /*qDebug() << mov->getX();
            qDebug() << mov->getY();
            qDebug() << mov->getZ();
            qDebug() << mov->getDuration();*/
            break;
        }

        default:{break;}

    }

    return mov;


}

