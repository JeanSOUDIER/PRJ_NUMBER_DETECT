#ifndef CONVOLUTIONALNN_H
#define CONVOLUTIONALNN_H

#include "neuralnetwork.h"


#include <iostream>

#include <QFile>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>



namespace BasicNN{
/*----------------------------------------------------------------------------*/
    class ConvolutionalNN : public NeuralNetwork{

    public:

         ConvolutionalNN(const BasicNN::scalar &learning_rate , const uint64_t &layer_number);
         ConvolutionalNN(const BasicNN::scalar &learning_rate , const uint64_t &layer_number , const std::string &save_path);
         ConvolutionalNN(const BasicNN::scalar &learning_rate , const  std::vector<Layer> &layers , const std::string &save_path);
         ConvolutionalNN(const BasicNN::scalar &learning_rate , const std::string &save_path);

         ~ConvolutionalNN() override;


    protected:

         void create(const uint64_t &layer_number , const uint64_t &layers_cells , const BasicNN::scalar &neuron_threshold , const BasicNN::scalar &neuron_offset , const std::vector<BasicNN::scalar> &neuron_weights) override;
         void create(const uint64_t &layer_number , const uint64_t &layers_cells) override;


         std::vector<Layer> layers() override;
         void train(const uint64_t &cycles) override;
         void train(const bool &endCondition) override;
         void train(const uint64_t &cycles , const bool &endCondition) override;

         bool load(const std::string &path) override;
         bool load(const QString &path) override;

         bool isFileValid(const std::string &path) override;
         bool isFileValid(const QString &path) override;

         bool save(const std::string &path , const bool &override_old) override;
         bool save(const QString &path , const bool &override_old) override;

         void addLayer(const Layer&) override;
         void addLayer(const std::vector<Neuron> &cells) override;
         void addLayer(const uint64_t &cell_number) override;

         void error(const bool &recompute) override;
         unsigned char predict() override;


    };
/*----------------------------------------------------------------------------*/

}



#endif // CONVOLUTIONALNN_H
