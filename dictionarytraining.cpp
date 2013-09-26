#include "dictionarytraining.h"

DictionaryTraining::DictionaryTraining()
{
    //Static class
}

void DictionaryTraining::train(string jsonPath, QuantizedSift &dictionary){
    ifstream ifs(jsonPath.c_str());
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(content, root, false);

    GlobalSettings::imgPath = root["imgPath"].asString();
    GlobalSettings::labelsPath = root["labelsPath"].asString();
    Json::Value trainingSet = root["dataset"];

    if(!parsedSuccess){
        cerr << MSG_ERROR_PARSE_JSON << jsonPath << endl;
        throw ERROR_PARSE_JSON;
    }
    cout<<"Computing dictionary:"<<endl;

    cv::Mat wholeDescriptors;
    for(int i=0; i<trainingSet.size(); ++i){
        //get i-image
        QueryImage queryIm(trainingSet[i].asString(), false); //cout << "get image" << endl;
        //get superpixels
        vector<SuperPixel*>* querySP = queryIm.getSuperPixels(); //cout << "get superpixels" << endl;
        //get SIFT descriptor for each superpixel
        /*
        Image img(trainingSet[i].asString());
        cv::Mat mask(img.getImage()->size(),CV_8UC1,cv::Scalar(255));
        SuperPixel *SP = new SuperPixel(*(img.getImage()), mask , false);
            cv::Mat descriptor;
            SP->computeSiftFeature(descriptor, false); cout << descriptor.size() << endl;
            wholeDescriptors.push_back(descriptor); //cout << "push back descriptor" << endl;
        PrintUtils::printPercentage(i,trainingSet.size());
        */
        for(int j=0; j<querySP->size(); j++){
            cv::Mat descriptor;
            (*querySP)[j]->computeSiftFeature(descriptor, false); //cout << "compute descriptor" << endl;
            wholeDescriptors.push_back(descriptor); //cout << "push back descriptor" << endl;
        }
        PrintUtils::printPercentage(i,trainingSet.size());
    }

    dictionary.computeKmean(wholeDescriptors); cout << "Kmean" << endl;
}
