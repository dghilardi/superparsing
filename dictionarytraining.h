#ifndef DICTIONARYTRAINING_H
#define DICTIONARYTRAINING_H

#include "quantizedsift.h"
#include <json/json.h>
#include <string>
#include <fstream>
#include "globalSettings.h"
#include "queryimage.h"
#include "superpixel.h"
#include "Utils/printutils.h"

#define ERROR_PARSE_JSON 1
#define MSG_ERROR_PARSE_JSON "ERROR! The file doesn't exists or is malformed "

using namespace std;

class DictionaryTraining //Static class
{
public:
    DictionaryTraining();
    static void train(string jsonPath, QuantizedSift &dictionary);
};

#endif // DICTIONARYTRAINING_H
