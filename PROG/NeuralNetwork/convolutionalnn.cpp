#include "convolutionalnn.h"

#include <QDebug>

namespace BasicNN {
/*----------------------------------------------------------------------------*/

ConvolutionalNN::ConvolutionalNN(const BasicNN::scalar &learning_rate , const uint64_t &layer_number)
    : NeuralNetwork(learning_rate , layer_number){}

ConvolutionalNN::ConvolutionalNN(const scalar &learning_rate, const uint64_t &layer_number, const std::string &save_path)
    : NeuralNetwork(learning_rate , layer_number , save_path ){}

ConvolutionalNN::ConvolutionalNN(const BasicNN::scalar &learning_rate , const  std::vector<Layer> &layers , const std::string &save_path)
    : NeuralNetwork(learning_rate , layers , save_path) {}

ConvolutionalNN::ConvolutionalNN(const BasicNN::scalar &learning_rate , const std::string &save_path)
    : NeuralNetwork(learning_rate , save_path){}


ConvolutionalNN::~ConvolutionalNN(){}



void ConvolutionalNN::create(const uint64_t &layer_number , const uint64_t &layers_cells , const BasicNN::scalar &neuron_threshold , const BasicNN::scalar &neuron_offset , const std::vector<BasicNN::scalar> &neuron_weights){





}

void ConvolutionalNN::create(const uint64_t &layer_number , const uint64_t &layers_cells){


}




void ConvolutionalNN::train(const uint64_t &cycles){


    for(uint64_t cycle = 0 ; cycle < cycles ; cycle++){

        //<PLACEHOLDER FOR CUDA CODE>


    }


  //Safety, ensures the results to not be lost if it is impossible de write the results in the path
  if(!save(_save_path , true)){

      std::string new_path = _save_path;

      while(!save(new_path , true)){
          std::cout << "Could not save training results in " << new_path <<".\n";
          std::cout << "Please enter a new path : ";
          std:: cin >> new_path;
      }

      _save_path = new_path; //Updates the save path


  }
}
void ConvolutionalNN::train(const bool &endCondition){

    while(!endCondition){


        //<PLACEHOLDER FOR CUDA CODE>

    }

    //Safety, ensures the results to not be lost if it is impossible de write the results in the path
    if(!save(_save_path , true)){

        std::string new_path = _save_path;

        while(!save(new_path , true)){
            std::cout << "Could not save training results in " << new_path <<".\n";
            std::cout << "Please enter a new path : ";
            std:: cin >> new_path;
        }

        _save_path = new_path; //Updates the save path

    }

}


void ConvolutionalNN::train(const uint64_t &cycles , const bool &endCondition){

    for(uint64_t cycle = 0 ; cycle < cycles ; cycle++){

        if(endCondition){break;}

        //<PLACEHOLDER FOR CUDA CODE>


    }


    //Safety, ensures the results to not be lost if it is impossible de write the results in the path
    if(!save(_save_path , true)){

        std::string new_path = _save_path;

        while(!save(new_path , true)){
            std::cout << "Could not save training results in " << new_path <<".\n";
            std::cout << "Please enter a new path : ";
            std:: cin >> new_path; //Updates the save path
        }

        _save_path = new_path;

    }



}

bool ConvolutionalNN::load(const std::string &path){return load(QString::fromStdString(path));}
bool ConvolutionalNN::load(const QString &path){

    if(!isFileValid(path)){return false;}

    QFile target_file(path.endsWith(".xml" , Qt::CaseInsensitive) ? path : path+".xml");

    if(target_file.open(QIODevice::ReadOnly)){

         _layers.clear(); //Remove old euristics only if it is possible to load data from the file

        QXmlStreamReader reader (&target_file);

        reader.readNext();
        //<CONVOLUTIONAL_NEURAL_NETWORK>
        reader.readNextStartElement();

            //<LAYER>
            while(reader.readNextStartElement()){

                std::vector<Neuron> cells;

                //<NEURON>
                while(reader.readNextStartElement()){

                    //<THRESHOLD>
                     reader.readNextStartElement();
                     const BasicNN::scalar threshold = reader.readElementText().toDouble();
                    //</THRESHOLD>

                     //<OFFSET>
                      reader.readNextStartElement();
                      const BasicNN::scalar offset = reader.readElementText().toDouble();
                     //</OFFSET>

                      std::vector<BasicNN::scalar> weights;

                     //<WEIGHTS>
                     while(reader.readNextStartElement()){

                         //<WEIGHT>
                         reader.readNextStartElement();
                         weights.push_back(reader.readElementText().toDouble());
                         //<WEIGHT>

                     }
                     //</WEIGHTS>


                     Neuron n(threshold , offset , weights);
                     cells.push_back(n);
                }
                //</NEURON>

                addLayer(Layer(cells));

            }
            //<LAYER>


        //</CONVOLUTIONAL_NEURAL_NETWORK>


    }

    else{return false;}

    return true;


}



bool ConvolutionalNN::isFileValid(const std::string &path){return isFileValid(QString::fromStdString(path));}
bool ConvolutionalNN::isFileValid(const QString &path){


    bool success = true;

    QFile target_file(path.endsWith(".xml" , Qt::CaseInsensitive) ? path : path+".xml");

    if(!target_file.exists()){return false;}

    if(target_file.open(QIODevice::ReadOnly)){

        QXmlStreamReader reader(&target_file);

        reader.readNext();

        //<CONVOLUTIONAL_NEURAL_NETWORK>
          reader.readNextStartElement();
          qDebug() << reader.name() << "CONVOLUTIONAL_NEURAL_NETWORK";
          if(reader.name() != "CONVOLUTIONAL_NEURAL_NETWORK"){success = false;}


          //<LAYER>
          while(reader.readNextStartElement() && success){

              qDebug() << reader.name() << "LAYER";
              if(reader.name() != "LAYER"){success = false;}

              //<NEURON>
              while(reader.readNextStartElement() && success){

                  qDebug() << reader.name() << "NEURON";
                  if(reader.name() != "NEURON"){success = false;}


                  //<THRESHOLD>
                   reader.readNextStartElement();
                   qDebug() << reader.name() << "THRESHOLD";
                   if(reader.name() != "THRESHOLD"){success = false;}
                   reader.readElementText();
                  //</THRESHOLD>

                   //<OFFSET>
                    reader.readNextStartElement();
                    qDebug() << reader.name() << "OFFSET";
                    if(reader.name() != "OFFSET"){success = false;}
                    reader.readElementText();
                   //</OFFSET>


                  //<WEIGHTS>
                  while(reader.readNextStartElement() && success){

                      //<WEIGHT>
                      reader.readNextStartElement();
                      qDebug() << reader.name() << "WEIGHT";
                      if(reader.name() != "WEIGHT"){success = false;}
                      reader.readElementText();
                      //<WEIGHT>

                  }
                  //</WEIGHTS>
              }
              //</<NEURON>
          }
          //</LAYER>
       //</CONVOLUTIONAL_NEURAL_NETWORK>



    }
    else{return false;}

    return success;

}



bool ConvolutionalNN::save(const std::string &path , const bool &override_old ){return save(QString::fromStdString(path) , override_old);}
bool ConvolutionalNN::save(const QString &path , const bool &override_old){


    QFile target_file(path.endsWith(".xml" , Qt::CaseInsensitive) ? path : path+".xml");

    if(target_file.exists() && override_old){target_file.remove();}


    if(target_file.open(QFile::WriteOnly)){

        QXmlStreamWriter writer(&target_file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();

        //<CONVOLUTIONAL_NEURAL_NETWORK>
        writer.writeStartElement("CONVOLUTIONAL_NEURAL_NETWORK");
                for(auto &layer:_layers){
                    //<LAYER>
                    writer.writeStartElement("LAYER");

                    for(auto &neuron:layer.cells()){
                            //<NEURON>
                            writer.writeStartElement("NEURON");
                                writer.writeTextElement("THRESHOLD" , QString::number(neuron.threshold()));
                                writer.writeTextElement("OFFSET" , QString::number(neuron.offset()));

                                    //<WEIGHTS>
                                    writer.writeStartElement("WEIGHTS");
                                    for(auto &weight:neuron.weights()){
                                        writer.writeTextElement("WEIGHT" , QString::number(weight));
                                    }
                                    writer.writeEndElement();
                                    //<WEIGHTS>


                                writer.writeEndElement();
                            //</NEURON>
                    }
                    writer.writeEndElement();
                    //</LAYER>

                }


        writer.writeEndElement();
        //</CONVOLUTIONAL_NEURAL_NETWORK>

        writer.writeEndDocument();


    }
    else{return false;}


    target_file.close();
    return true;


}

void ConvolutionalNN::addLayer(const Layer &layer){_layers.push_back(Layer(layer));}
void ConvolutionalNN::addLayer(const std::vector<Neuron> &cells){_layers.push_back(Layer(cells));}
void ConvolutionalNN::addLayer(const uint64_t &cell_number){_layers.push_back(Layer(cell_number));}

unsigned char ConvolutionalNN::predict() {


    //<PLACEHOLDER FOR CUDA CODE>

    return 'a';

}


void ConvolutionalNN::error(const bool &recompute){

    if(recompute){
        for(Layer &layer:_layers){layer.compute();}
    }


                    /*-------*/


     for(Layer &layer:_layers){
         for(Neuron &neuron:layer.cells()){
             //<PLACEHOLDER FOR CUDA CODE>
             (void)neuron; //Disables compiler warning
            }
        }




}

std::vector<Layer> ConvolutionalNN::layers() {return _layers;}
/*----------------------------------------------------------------------------*/
}
