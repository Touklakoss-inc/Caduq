#include "STL.h"

#include "Eigen/Core"
#include "Geometry/Geo.h"
#include <fstream>
#include <iostream>

namespace Tools
{
    Geometry::Mesh ImportSTL(const std::string& filename)
    {
        Eigen::Matrix<double, 3, Eigen::Dynamic> nodes;
        Eigen::Matrix<uint32_t, Eigen::Dynamic, 1> elts;
        std::ifstream file(filename);
        if (file.is_open()) 
        {
            std::string line;
            int n = 0;
            while (std::getline(file, line)) 
            {
                // Takes only space separated C++ strings.
                std::stringstream ss(line);  
                std::string word;
                std::vector<std::string> linevector { };
                while (ss >> word) // Extract word from the stream.
                { 
                    linevector.push_back(word);
                }

                if (linevector[0] == "vertex")
                {
                    nodes.conservativeResize(nodes.rows(), nodes.cols()+1);
                    nodes << nodes.block(0, 0, nodes.rows(), nodes.cols()-1), Eigen::Vector3d{ std::stof(linevector[1]), std::stof(linevector[2]), std::stof(linevector[3]) };
                }
                if (linevector[0] == "facet")
                {
                    elts.conservativeResize(elts.rows()+3, 1);
                    elts << elts.block(0, 0, elts.rows()-3, 1), n, n+1, n+2;
                    n += 3;
                }
            }
        }

        if (nodes.cols() == 0)
        {
            std::cout << "ERROR, no nodes in stl file"  << '\n';
            return Geometry::Mesh{ Eigen::Vector3d::Zero(), Eigen::Vector3<uint32_t>::Zero() };
        }

        return Geometry::Mesh{ nodes, elts };
    }
}
