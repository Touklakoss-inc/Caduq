#include "FileWriting.h"

#include <fstream>
#include <iostream>

namespace Geometry::Tools
{
    std::string RJust(std::string str, int chr_nb)
    {
        int strLength{ static_cast<int>(str.length()) };
        if (strLength < chr_nb)
        {
            for (int i = 0; i < chr_nb-strLength; i++) 
            {
                str = " " + str;
            }
        }

        return str;
    }

    void WriteMesh(std::string filename, Eigen::MatrixXd nodes)
    {
        std::ofstream outfile{ filename };

        outfile << "*NODE\n";

        for (int i = 0; i < nodes.cols(); i++)
        {
            outfile << RJust(std::to_string(i+1), 8); 
            outfile << RJust(std::to_string(nodes(0, i)), 16);
            outfile << RJust(std::to_string(nodes(1, i)), 16);
            outfile << RJust(std::to_string(nodes(2, i)), 16);
            outfile << '\n';
        }

        outfile << "*ELEMENT_BEAM\n";


        for (int i = 0; i < nodes.cols()-1; i++)
        {
            outfile << RJust(std::to_string(i+1), 8); 
            outfile << RJust(std::to_string(1), 8); 
            outfile << RJust(std::to_string(i+1), 8);
            outfile << RJust(std::to_string(i+2), 8);
            outfile << RJust(std::to_string(0), 8);
            outfile << '\n';
        }

        outfile << "*END\n";
        outfile.close();
    }
}
