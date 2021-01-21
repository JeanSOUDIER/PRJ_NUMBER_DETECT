#include "neuralnetwork.h"

namespace MNN {

NeuralNetwork::NeuralNetwork(const uint64_t &layers){
    _layers.reserve(layers);
}

NeuralNetwork::NeuralNetwork(const std::vector<Layer*> &layers){
    _layers = layers;
}

#ifdef MNN_TRAIN
NeuralNetwork::NeuralNetwork(const scalar &learning_rate , const uint64_t &layers){

    _learning_rate = learning_rate;

    _layers.reserve(layers);

     _save_path = "D:/new_MNN.xml";

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


NeuralNetwork::NeuralNetwork(const uint64_t &layers , const std::string &save_path){
    _layers.reserve(layers);
     _save_path = save_path;
}
#endif
NeuralNetwork::~NeuralNetwork(){

    clear_layers();

#ifdef MNN_TRAIN
    delete dataset_trainer;
    delete dataset_tester;
#endif

}


long long int NeuralNetwork::predict(const ste::Matrix<scalar> &input) const{

    ste::Matrix<scalar> result = input;

    for(const Layer* layer : _layers){
        MNN::elu_r((result = (layer->weights() * result))+= layer->biases()); ///This syntax limits the amount of deep copy, and thus fastens the propagation. It is recommended to use it for your network too (adapted and associated with loop unrolling and eventual multi-threading).
        //result = MNN::elu((layer->weights() * result) += layer->biases());
    }

    return Utility::indexOf(result.toVector1D() , result.max());

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


std::vector<Layer*>& NeuralNetwork::layers() {return _layers;}


#ifdef MNN_TRAIN
void NeuralNetwork::create(const std::vector<uint64_t> &layers_neurons){

    //Xavier initialization
    //https://www.deeplearning.ai/ai-notes/initialization/
    std::mt19937 gen(time(0));

    for(size_t index = 1 ; index < layers_neurons.size() ; index++){

        const scalar standard_deviation =  std::sqrt(2./(layers_neurons.at(index-1) + (layers_neurons.at(index))));

        _layers.push_back(
                    new Layer(
                        ste::Matrix<scalar>::randn(layers_neurons.at(index) , layers_neurons.at(index-1) , 0 , standard_deviation) , //weights
                        ste::Matrix<scalar>(layers_neurons.at(index) , 1 , 0)) //biases
                    );

    }

}



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
        if(method_parameters.size() != 1){throw std::invalid_argument("NeuralNetwork::train\nInvalid method parameters.\nExpected 1 for momentum method, but received "
                                                                      + std::to_string(method_parameters.size()) + ".");}

        train_MOMENTUM(epochs , endCondition , batch_size  ,method_parameters[0]);
        break;
    }

    case(MNN::Train::RMS_PROP):{

        if(method_parameters.size() != 2){throw std::invalid_argument("NeuralNetwork::train\nInvalid method parameters.\nExpected 2 for RMSprop method, but received "
                                                                      + std::to_string(method_parameters.size()) + ".");}

        train_RMS(epochs , endCondition , batch_size , method_parameters[0] , method_parameters[1]);
        break;
    }

    case(MNN::Train::ADAM):{


        if(method_parameters.size() != 3){throw std::invalid_argument("NeuralNetwork::train\nInvalid method parameters.\nExpected 3 for Adam method, but received "
                                                                      + std::to_string(method_parameters.size()) + ".");}

        train_ADAM(epochs , endCondition, batch_size , method_parameters[0] , method_parameters[1] , method_parameters[2]);
        break;
    }

    default:{throw std::invalid_argument("NeuralNetwork::train\nInvalid method. This error is often due to a cast.");}

    }

    std::cout << "Training completed." << std::endl;

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




void NeuralNetwork::train_SGD(const uint64_t &epochs , const bool &endCondition , const uint64_t &batch_size){

    std::clog << "WARNING ! Call to base function MNN::NeuralNetwork::train_SGD.\n This function IS NOT MEANT TO BE CALLED TO TRAIN YOUR NEURAL NETWORK it does not all to customize anything, including the activation functions. It should only serve as a reference for the algorithm." << std::endl;
    std::cout << "*************************************************MNN::NeuralNetwork::train_SGD*************************************************" << std::endl;

    for(uint64_t epoch = 1 ; (epoch <= epochs) && !endCondition ; epoch++){
        std::cout << "Epoch " << epoch << std::endl;
        for(uint64_t item = 0 ; item < dataset_trainer->size()/batch_size ; item += batch_size){

            //Vectors of the errors, total errors and gradients.
            std::vector<ste::Matrix<scalar>> errors;
            errors.reserve(layers().size());

            std::vector<ste::Matrix<scalar>> total_errors;
            total_errors.reserve(layers().size());

            std::vector<ste::Matrix<scalar>> gradients;
            gradients.reserve(layers().size());

            //Initialize the vectors.
            for(Layer* layer : _layers){
                errors.push_back(ste::Matrix<scalar>(layer->cell_number() , 1 , 0));
                total_errors.push_back(ste::Matrix<scalar>(layer->cell_number() , 1 , 0));
                gradients.push_back(ste::Matrix<scalar>(layer->weights().rows() , layer->weights().columns()  , 0));
            }


            for(uint64_t batch_item = item ; batch_item <= item+batch_size ; batch_item++){

                std::vector<ste::Matrix<scalar>> raw_outputs;
                raw_outputs.reserve(layers().size());

                std::vector<ste::Matrix<scalar>> activated_outputs;
                activated_outputs.reserve(layers().size());


                ///Forward propagation
                activated_outputs.push_back(dataset_trainer->data(batch_item));
                for(const Layer *layer : _layers){
                    raw_outputs.push_back((layer->weights() * activated_outputs.back()) += layer->biases());
                    activated_outputs.push_back(MNN::elu(raw_outputs.back()));
                }


                ///Back propagation

                ///Output layer
                errors.back() = ste::Matrix<scalar>::hadamard(activated_outputs.back() - expected(dataset_trainer->label(batch_item)) ,
                                                                   MNN::elu_derivative_r(raw_outputs.back()));


               for(uint64_t layer = _layers.size()-2 ; layer > 0 ; layer--){

                    errors.at(layer) = ste::Matrix<scalar>::hadamard(_layers.at(layer+1)->weights().transpose()*errors.at(layer+1) , //Check indexes here
                                                                     MNN::elu_derivative_r(raw_outputs.at(layer))); //No need to keep the old value, so it is passed by reference to avoid copy         //--------------------------------------------------------------------------------

                    total_errors.at(layer) += errors.at(layer);

                    gradients.at(layer) += errors.at(layer) * activated_outputs.at(layer).transpose();

                }

                ///First layer
                errors.at(0) = ste::Matrix<scalar>::hadamard(_layers.at(0+1)->weights().transpose()*errors.at(0+1) , //Check indexes here
                                                             MNN::elu_derivative_r(raw_outputs.at(0))); //No need to keep the old value, so it is passed by reference to avoid copy         //--------------------------------------------------------------------------------

                total_errors.at(0) += errors.at(0);

                gradients.at(0) += errors.at(0) * activated_outputs.at(0).transpose();

            }

            std::for_each(gradients.begin() , gradients.end() , [=](ste::Matrix<scalar> &matrix){matrix *=(_learning_rate/batch_size);});
            std::for_each(total_errors.begin() , total_errors.end() , [=](ste::Matrix<scalar> &matrix){matrix*=(_learning_rate/batch_size);});

            ///Gradient descent
            for(uint64_t index = 0 ; index < _layers.size() ; index++){
                _layers.at(index)->weights() -= gradients.at(index);
                _layers.at(index)->biases() -= total_errors.at(index);
            }
        }

    }

}

void NeuralNetwork::train_MOMENTUM(const uint64_t &epochs , const bool &endCondition , const uint64_t &batch_size  , const MNN::scalar &beta){

    std::clog << "WARNING ! Call to base function MNN::NeuralNetwork::train_MOMENTUM.\n This function IS NOT MEANT TO BE CALLED TO TRAIN YOUR NEURAL NETWORK it does not all to customize anything, including the activation functions. It should only serve as a reference for the algorithm." << std::endl;
    std::cout << "*************************************************MNN::NeuralNetwork::train_MOMENTUM*************************************************" << std::endl;

    (void)beta;

    for(uint64_t epoch = 1 ; (epoch <= epochs) && !endCondition ; epoch++){


        for(uint64_t item = 0 ; item < dataset_trainer->size()/batch_size ; item++){

            for(uint64_t batch_item = 1 ; batch_item <= batch_size ; batch_item++){}

        }

    }


}

void NeuralNetwork::train_RMS(const uint64_t &epochs , const bool &endCondition , const uint64_t &batch_size , const MNN::scalar &beta , const MNN::scalar &epsilon){

    std::clog << "WARNING ! Call to base function MNN::NeuralNetwork::train_RMS.\n This function IS NOT MEANT TO BE CALLED TO TRAIN YOUR NEURAL NETWORK it does not all to customize anything, including the activation functions. It should only serve as a reference for the algorithm." << std::endl;
    std::cout << "*************************************************MNN::NeuralNetwork::train_RMS*************************************************" << std::endl;

    (void)beta;
    (void)epsilon;

    for(uint64_t epoch = 1 ; (epoch <= epochs) && !endCondition ; epoch++){


        for(uint64_t item = 0 ; item < dataset_trainer->size()/batch_size ; item++){

            for(uint64_t batch_item = 1 ; batch_item <= batch_size ; batch_item++){}

        }

    }


}

void NeuralNetwork::train_ADAM(const uint64_t &epochs , const bool &endCondition , const uint64_t &batch_size , const MNN::scalar &beta1 , const MNN::scalar &beta2, const MNN::scalar &epsilon){

    std::clog << "WARNING ! Call to base function MNN::NeuralNetwork::train_ADAM.\n This function IS NOT MEANT TO BE CALLED TO TRAIN YOUR NEURAL NETWORK it does not all to customize anything, including the activation functions. It should only serve as a reference for the algorithm." << std::endl;
    std::cout << "*************************************************MNN::NeuralNetwork::train_ADAM*************************************************" << std::endl;

    (void)beta1;
    (void)beta2;
    (void)epsilon;

    for(uint64_t epoch = 1 ; (epoch <= epochs) && !endCondition ; epoch++){


        for(uint64_t item = 0 ; item < dataset_trainer->size()/batch_size ; item++){

            for(uint64_t batch_item = 1 ; batch_item <= batch_size ; batch_item++){}

        }

    }


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

                for(uint64_t layer = 0 ; layer < layers().size() ; layer++){
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

scalar NeuralNetwork::loss(const ste::Matrix<scalar> &expected_output , const ste::Matrix<scalar> &obtained_output) const{
    return cost(expected_output , obtained_output);
}
#endif



}


