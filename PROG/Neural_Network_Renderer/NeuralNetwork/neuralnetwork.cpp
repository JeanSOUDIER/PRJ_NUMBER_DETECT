#include "neuralnetwork.h"

#include <QDebug>
namespace MNN {

NeuralNetwork::NeuralNetwork(const uint64_t &layers){

    _learning_rate = 0;

    _layers.reserve(layers);

    _save_path = "D:/";


}

NeuralNetwork::NeuralNetwork(const std::vector<Layer*> &layers){

    _learning_rate = 0;

    _layers = layers;

    _save_path = "D:/";

}


NeuralNetwork::NeuralNetwork(const scalar &learning_rate , const uint64_t &layers){

    _learning_rate = learning_rate;

    _layers.reserve(layers);

     _save_path = "D:/";

}
NeuralNetwork::NeuralNetwork(const scalar &learning_rate , const uint64_t &layers , const std::string &save_path){

    _learning_rate = learning_rate;

    _layers.reserve(layers);

     _save_path = save_path;


}

NeuralNetwork::NeuralNetwork(const scalar &learning_rate , const std::vector<Layer *> &layers , const std::string &save_path){

    _learning_rate = learning_rate;

    _layers = layers;

     _save_path = save_path;

}

NeuralNetwork::NeuralNetwork(const scalar &learning_rate , const std::string &save_path){

    _learning_rate = learning_rate;

    _layers.reserve(2);

     _save_path = save_path;


}

NeuralNetwork::~NeuralNetwork(){clear_layers();}



void NeuralNetwork::create(const std::vector<uint64_t> &layers_neurons){



        std::mt19937 gen(time(0));

    /*


        std::vector<Neuron*> neurons;
        neurons.reserve(layers_neurons.at(layer));

        const scalar variance =  2./(layers_neurons.at(layer-1) + (layers_neurons.at(layer)));
        //const scalar variance = 1;

        std::normal_distribution<scalar> xavier_initializer(0, std::sqrt(variance));

        for(uint64_t neuron_index = 0 ; neuron_index < layers_neurons.at(layer) ; neuron_index++){

            //Xavier initialization
            //https://www.deeplearning.ai/ai-notes/initialization/

            std::vector<scalar> weights;
            weights.resize(layers_neurons.at(layer-1) , 0);

            for(scalar &weight : weights){weight = xavier_initializer(gen);}

            AdvancedNeuron *neuron = new AdvancedNeuron(weights , 0);
            //neuron->setActivationFunction(AdvancedNeuron::sigmoid() , AdvancedNeuron::sigmoid_derivative());

            neurons.push_back(neuron);


        }

        _layers.push_back(new ConvolutionalLayer(neurons));
    */



    for(size_t index = 1 ; index < layers_neurons.size() ; index++){

        ste::Matrix<scalar> weights(layers_neurons.at(index) , layers_neurons.at(index-1));






        _layers.push_back(
                    new Layer(
                        weights ,
                        ste::Matrix<scalar>(layers_neurons.at(index) , 1))
                    );

    }

}


#ifdef MNN_TRAIN
void NeuralNetwork::train(const uint64_t &epochs , const uint64_t &batch_size , const Train &method, const std::vector<scalar> &method_parameters){
    train(epochs , false , batch_size , method , method_parameters);
}

void NeuralNetwork::train(const uint64_t &epochs , const bool &endCondition , const uint64_t &batch_size , const Train &method , const std::vector<scalar> &method_parameters){


    switch(method){

    case(MNN::Train::GRADIENT_DESCENT):{

        train_SGD(epochs , endCondition , batch_size);
        break;
    }

    case(MNN::Train::MOMENTUM):{
        if(method_parameters.size() != 1){throw std::invalid_argument("Project_network::train\nInvalid method parameters.\nExpected 1 for momentum method, but received "
                                                                      + std::to_string(method_parameters.size()) + ".");}

        train_MOMENTUM(epochs , endCondition , batch_size  ,method_parameters[0]);
        break;
    }

    case(MNN::Train::RMS_PROP):{

        if(method_parameters.size() != 2){throw std::invalid_argument("Project_network::train\nInvalid method parameters.\nExpected 2 for RMSprop method, but received "
                                                                      + std::to_string(method_parameters.size()) + ".");}

        train_RMS(epochs , endCondition , batch_size , method_parameters[0] , method_parameters[1]);
        break;
    }

    case(MNN::Train::ADAM):{


        if(method_parameters.size() != 3){throw std::invalid_argument("Project_network::train\nInvalid method parameters.\nExpected 3 for Adam method, but received "
                                                                      + std::to_string(method_parameters.size()) + ".");}

        train_ADAM(epochs , endCondition, batch_size , method_parameters[0] , method_parameters[1] , method_parameters[2]);
        break;
    }

    default:{throw std::invalid_argument("Project_network::train\nInvalid method. This error is often due to a cast.");}

    }

    //Safety, ensures the results to not be lost if it is impossible de write the results in the path
    if(!save(_save_path , true)){

        std::string new_path = _save_path;

        while(!save(new_path , true)){
            std::cout << "Could not save training results in " << new_path <<".\n";
            std::cout << "Please enter a new path : ";
            std::cin >> new_path; //Updates the save path
        }

        _save_path = new_path;

    }

}

void NeuralNetwork::train_SGD(const uint64_t &epoch , const uint64_t &batch_size){
    train_SGD(epoch , false , batch_size);
}

void NeuralNetwork::train_MOMENTUM(const uint64_t &epoch , const uint64_t &batch_size  , const scalar &beta){
    train_MOMENTUM(epoch , false , batch_size , beta);
}

void NeuralNetwork::train_RMS(const uint64_t &epoch , const uint64_t &batch_size , const scalar &beta , const scalar &epsilon){
    train_RMS(epoch , false , batch_size , beta , epsilon);
}

void NeuralNetwork::train_ADAM(const uint64_t &epoch , const uint64_t &batch_size , const scalar &beta1, const scalar &beta2, const scalar &epsilon){
    train_ADAM(epoch , false , batch_size , beta1 , beta2 , epsilon);
}
#endif
long long int NeuralNetwork::predict(const ste::Matrix<scalar> &input) const{

    ste::Matrix<scalar> result = input;

    for(Layer* layer : _layers){
        result = MNN::elu((layer->weights() * result) + layer->biases());
    }

    return Utility::indexOf(result.toVector1D() , result.max());

}



bool NeuralNetwork::save(const bool &override_old){return save(_save_path , override_old);}

bool NeuralNetwork::save(const std::string &path , const bool &override_old){

    const QString q_path = QString::fromStdString(path);

    QFile target_file(q_path.endsWith(".xml" , Qt::CaseInsensitive) ? q_path : q_path+".xml");

    if(target_file.exists() && override_old){target_file.remove();}
    if(target_file.exists() && !override_old){return false;}

    if(target_file.open(QFile::WriteOnly)){

        QXmlStreamWriter writer(&target_file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();

        //<MNN_NETWORK>
        writer.writeStartElement("MNN_NETWORK");

                for(uint64_t layer = 1 ; layer < layers().size() ; layer++){
                    //<LAYER>
                    writer.writeStartElement("LAYER");

                    for(uint64_t neuron = 0 ; neuron < layers().at(layer)->weights().rows() ; neuron++){
                            //<NEURON>
                            writer.writeStartElement("NEURON");
                                writer.writeTextElement("BIAS" , QString::number(layers().at(layer)->biases().at(neuron)));

                                    //<WEIGHTS>
                                    writer.writeStartElement("WEIGHTS");
                                    for(const scalar weight : layers().at(layer)->weights().rowAt(neuron)){
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
        //</MNN_NETWORK>

        writer.writeEndDocument();


    }
    else{return false;}

    target_file.close();

    return true;


}


bool NeuralNetwork::load(const std::string &path){

    const QString q_path = QString::fromStdString(path);

    if(!isFileValid(path)){return false;}

    QFile target_file(q_path.endsWith(".xml" , Qt::CaseInsensitive) ? q_path : q_path+".xml");

    if(target_file.open(QIODevice::ReadOnly)){

       _layers.clear(); //Remove old euristics only if it is possible to load data from the file

        QXmlStreamReader reader (&target_file);

        reader.readNext();
        //<MNN_NETWORK>
        reader.readNextStartElement();

            //<LAYER>
            while(reader.readNextStartElement()){

                std::vector<std::vector<scalar>> layer_weights;
                std::vector<scalar> layer_biases;

                //<NEURON>
                while(reader.readNextStartElement()){

                    //<BIAS>
                     reader.readNextStartElement();
                     //qDebug() << reader.name() << "BIAS";
                     const scalar bias = reader.readElementText().toDouble();
                     layer_biases.push_back(bias);
                    //</BIAS>
                    reader.readNextStartElement();
                      std::vector<scalar> weights;

                     //<WEIGHTS>
                     while(reader.readNextStartElement()){

                         //<WEIGHT>
                         weights.push_back(reader.readElementText().toDouble());
                         //qDebug() << reader.name() << "WEIGHT";
                         //<WEIGHT>

                     }
                     //</WEIGHTS>

                     layer_weights.push_back(weights);
                    reader.readNextStartElement();

                }
                //</NEURON>

                _layers.push_back(
                                 new Layer( ste::Matrix<scalar>(layer_weights)
                                            ,
                                            ste::Matrix<scalar>(layer_biases , layer_weights.size() , 1))
                                  );


            }
            //<LAYER>

        //</MNN_NETWORK>


    }

    else{return false;}

    return true;

}

bool NeuralNetwork::isFileValid(const std::string &path){


    const QString q_path = QString::fromStdString(path);

    bool success = true;

    QFile target_file(q_path.endsWith(".xml" , Qt::CaseInsensitive) ? q_path : q_path+".xml");

    if(!target_file.exists()){return false;}

    if(target_file.open(QIODevice::ReadOnly)){

        QXmlStreamReader reader(&target_file);

        reader.readNext();

        //<MNN_NETWORK>
          reader.readNextStartElement();
          //qDebug() << reader.name() << "MNN_NETWORK";
          if(reader.name() != "MNN_NETWORK"){success = false;}

          //<LAYER>

          while(reader.readNextStartElement() && success){

              //qDebug() << reader.name() << "LAYER";
              if(reader.name() != "LAYER"){success = false;}

              //<NEURON>
              while(reader.readNextStartElement() && success){


                  //qDebug() << reader.name() << "NEURON";
                  if(reader.name() != "NEURON"){success = false;}

                  //<BIAS>
                   reader.readNextStartElement();
                   //qDebug() << reader.name() << "BIAS";
                   if(reader.name() != "BIAS"){success = false;}
                   reader.readElementText();
                  //</BIAS>

                   reader.readNextStartElement();
                   //qDebug() << reader.name() << "WEIGHTS";
                   if(reader.name() != "WEIGHTS"){success = false;}

                  //<WEIGHTS>
                  while(reader.readNextStartElement() && success){

                      //<WEIGHT>
                      reader.readNextStartElement();
                      //qDebug() << reader.name() << "WEIGHT";
                      if(reader.name() != "WEIGHT"){success = false;}
                      reader.readElementText();
                      //<WEIGHT>
                  }


                  //</WEIGHTS>
                  reader.readNextStartElement();
              }

              //</<NEURON>


          }
          //</LAYER>

       //</MNN_NETWORK>

}

    return success;
}


void NeuralNetwork::clear_layers(){

    if(layers().size() > 0){

        auto st = _layers.begin();
        auto ed = _layers.end();

        while (st != ed) {
            delete *st;
            ++st;
        }

    }

    _layers.clear();


}

scalar NeuralNetwork::loss(const ste::Matrix<scalar> &expected_output , const ste::Matrix<scalar> &obtained_output) const{
    return cost(expected_output , obtained_output);
}


std::vector<Layer*>& NeuralNetwork::layers() {return _layers;}

}


