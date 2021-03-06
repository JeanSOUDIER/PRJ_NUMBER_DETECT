#include "project_network.h"

Project_network::Project_network(const uint64_t &layer_number) : MNN::NeuralNetwork(layer_number){}

#ifdef MNN_TRAIN
Project_network::Project_network(const MNN::scalar &learning_rate , const uint64_t &layer_number ,
                                 const std::string &save_path ,
                                 const std::string &path_src_TRAIN, const std::string &delimiter_TRAIN ,
                                 const std::string &path_src_VALIDATION , const std::string &delimiter_VALIDATION)

    : MNN::NeuralNetwork(learning_rate , layer_number , save_path){



    dataset_trainer = new MNIST_Handler(path_src_TRAIN , delimiter_TRAIN , MNIST_Handler::SET::TRAIN);
    dataset_tester = new MNIST_Handler(path_src_VALIDATION , delimiter_VALIDATION , MNIST_Handler::SET::TEST);

}
#endif

Project_network::~Project_network(){
    clear_layers();
}

#ifdef MNN_TRAIN
ste::Matrix<MNN::scalar> Project_network::expected(const long long int &label) const{

    switch(label){

        case(0):{return ste::Matrix<MNN::scalar>({1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} , 10 , 1);}
        case(1):{return ste::Matrix<MNN::scalar>({0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0} , 10 , 1);}
        case(2):{return ste::Matrix<MNN::scalar>({0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0} , 10 , 1);}
        case(3):{return ste::Matrix<MNN::scalar>({0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0} , 10 , 1);}
        case(4):{return ste::Matrix<MNN::scalar>({0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0} , 10 , 1);}
        case(5):{return ste::Matrix<MNN::scalar>({0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0} , 10 , 1);}
        case(6):{return ste::Matrix<MNN::scalar>({0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0} , 10 , 1);}
        case(7):{return ste::Matrix<MNN::scalar>({0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0} , 10 , 1);}
        case(8):{return ste::Matrix<MNN::scalar>({0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0} , 10 , 1);}
        case(9):{return ste::Matrix<MNN::scalar>({0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1} , 10 , 1);}

        default:{throw std::invalid_argument("Project_network::expected_vector\nExpected a label between 0 and 9 but received " + std::to_string(label) + ".");}

    }
}

//void Project_network::train_SGD(const uint64_t &epochs , const bool &endCondition , const uint64_t &batch_size){


//    NeuralNetwork::train_SGD(epochs , endCondition, batch_size);

//    for(uint64_t epoch = 1 ; (epoch <= epochs) && !endCondition ; epoch++){

//        for(uint64_t batch_item = 1 ; batch_item <= batch_size ; batch_item++){}

//    }

//}

//void Project_network::train_MOMENTUM(const uint64_t &epochs , const bool &endCondition , const uint64_t &batch_size  , const MNN::scalar &beta){

//    (void)beta;

//    for(uint64_t epoch = 1 ; (epoch <= epochs) && !endCondition ; epoch++){

//        for(uint64_t batch_item = 1 ; batch_item <= batch_size ; batch_item++){}

//    }

//}

//void Project_network::train_RMS(const uint64_t &epochs , const bool &endCondition , const uint64_t &batch_size , const MNN::scalar &beta , const MNN::scalar &epsilon){

//    (void)beta;
//    (void)epsilon;

//    for(uint64_t epoch = 1 ; (epoch <= epochs) && !endCondition ; epoch++){

//        for(uint64_t batch_item = 1 ; batch_item <= batch_size ; batch_item++){}

//    }

//}

//void Project_network::train_ADAM(const uint64_t &epochs , const bool &endCondition , const uint64_t &batch_size , const MNN::scalar &beta1 , const MNN::scalar &beta2, const MNN::scalar &epsilon){

//    (void)beta1;
//    (void)beta2;
//    (void)epsilon;

//    for(uint64_t epoch = 1 ; (epoch <= epochs) && !endCondition ; epoch++){

//        for(uint64_t batch_item = 1 ; batch_item <= batch_size ; batch_item++){}

//    }

//}


MNN::scalar Project_network::cost(const ste::Matrix<MNN::scalar> &expected_output , const ste::Matrix<MNN::scalar> &obtained_output) const{

    (void)expected_output;
    (void)obtained_output;
    return 0;
}

uint64_t Project_network::test(const uint64_t &size){

    //std::cout << "   Proceeding to testing..." << std::endl;

    uint64_t correct = 0;
    //auto t1 = std::chrono::high_resolution_clock::now();

    //Shuffle if not necessary to go through all the validation set
    if(size < 10000){

        std::mt19937 gen(time(0));
        std::uniform_int_distribution<uint64_t> index_data_shuffler(0 , 9999);

        const uint64_t random_value = index_data_shuffler(gen);
        const uint64_t begin_index = (random_value + size > 9999) ? 9999-size-1 : random_value ;

        //t1 = std::chrono::high_resolution_clock::now();
        for(uint64_t data = begin_index ; data < begin_index + size ; data++){

            //const long long int prediction = predict(dataset_tester->data(data));
            //const long long int label = dataset_tester->label(data);

            //std::cout << "Label: " << label << " Prediction: " << prediction << std::endl;
            //correct += (prediction == label);

           correct +=(predict(dataset_tester->data(data)) == dataset_tester->label(data));

        }

    }
    else{


        //t1 = std::chrono::high_resolution_clock::now();
        for(uint64_t data = 0 ; data < size ; data++){

            //std::cout << "Item " << data << "...\n";
//            correct +=(predict(dataset_tester->data(data)) == dataset_tester->label(data));
            //std::cout << "Done." << std::endl;

            const long long int prediction = predict(dataset_tester->data(data));
            const long long int label = dataset_tester->label(data);

            std::cout << "Label: " << label << " Prediction: " << prediction << std::endl;
            correct += (prediction == label);

        }

    }

//    const auto t2 = std::chrono::high_resolution_clock::now();
//    std::cout << "Done." << std::endl;

//    std::cout << "Results: " << correct << "/" << size << std::endl;
//    std::cout << "Time (us): " << std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count() << " us" << std::endl;
//    std::cout << "Time (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count() << " ms" << std::endl;



    std::cout << "   Done." << std::endl;


    return correct;

}
#endif
