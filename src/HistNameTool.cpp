#include "HistNameTool.h"

#include <string>

namespace HistNameTool
{
    std::string transformFloat(float KLambda){
        // input: any float KLambda
        // output: somethin like (string) 1p0

        // A kindly remind here: dure to the limit of std:to_string, this program cant handle
        // input more than 5 digit decimal
        // Transform input KLambda to string form, such as 1p00000
        std::string string_KLambda = std::to_string(KLambda);
        auto index_of_point = string_KLambda.find(".");
        if(index_of_point != -1)
            string_KLambda.replace(index_of_point, 1, "p");
        // 2 digit decimal
        //string_KLambda = string_KLambda.substr(0, string_KLambda.length() - 4);
        // 1 digit decimal
        string_KLambda = string_KLambda.substr(0, string_KLambda.length() - 5);
        //if KLambda<0, we need to substitute the "-" with "n"
        auto index_of_minus = string_KLambda.find("-");
        if (index_of_minus != -1)
            string_KLambda.replace(index_of_minus, 1, "n");

        return string_KLambda;

    }
}
