#include "FileWriting.h"

#include "Geometry/Patch.h"
#include <fstream>
#include <iostream>

#include <vector>
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

    void WriteMesh(std::string filename, std::vector<Geometry::Spline> splines)
    {
        int node_offset{ 0 };
        std::ofstream outfile{ filename };

        outfile << "*NODE\n";

        for (Geometry::Spline s : splines)
        {
            auto [nodes, elts] = s.GetFemMesh();

            for (int i = 0; i < nodes.cols(); i++)
            {
                outfile << RJust(std::to_string(i+1 + node_offset), 8); 
                outfile << RJust(std::to_string(nodes(0, i)), 16);
                outfile << RJust(std::to_string(nodes(1, i)), 16);
                outfile << RJust(std::to_string(nodes(2, i)), 16);
                outfile << '\n';
            }
            node_offset += nodes.cols();
        }

        outfile << "*ELEMENT_BEAM\n";

        node_offset = 0;
        for (Geometry::Spline s : splines)
        {
            auto [nodes, elts] = s.GetFemMesh();
            for (int i = 0; i < elts.rows()/2-1; i++)
            {
                outfile << RJust(std::to_string(i+1 + node_offset), 8);
                outfile << RJust(std::to_string(1), 8);
                outfile << RJust(std::to_string(elts(i*2)+1 + node_offset), 8);
                outfile << RJust(std::to_string(elts(i*2+1)+1 + node_offset), 8);
                outfile << RJust(std::to_string(0), 8);
                outfile << '\n';
            }
            node_offset += nodes.cols();
        }

        outfile << "*END\n";
        outfile.close();
    }

    void WriteNodes(std::string filename, Eigen::MatrixXd nodes)
    {
        std::ofstream outfile{ filename };

        outfile << "*NODE\n";

        for (int i = 0; i < nodes.rows(); i+=3)
        {
            for (int j = 0; j < nodes.cols(); j++)
            {
                outfile << RJust(std::to_string(j+1 + i/3*nodes.cols()), 8); 
                outfile << RJust(std::to_string(nodes(i, j)), 16);
                outfile << RJust(std::to_string(nodes(i+1, j)), 16);
                outfile << RJust(std::to_string(nodes(i+2, j)), 16);
                outfile << '\n';
            }
        }

        outfile << "*END\n";
        outfile.close();
    }

    void WriteSurface(std::string filename, Geometry::Patch patch)
    {
        std::ofstream outfile{ filename };

        auto [nodes, elts] = patch.GetFemMesh();

        outfile << "*NODE\n";

        for (int j = 0; j < nodes.cols(); j+=1)
        {
            for (int i = 0; i < nodes.rows(); i+=3)
            {
                outfile << RJust(std::to_string(j+1 + i/3*nodes.cols()), 8); 
                outfile << RJust(std::to_string(nodes(i, j)), 16);
                outfile << RJust(std::to_string(nodes(i+1, j)), 16);
                outfile << RJust(std::to_string(nodes(i+2, j)), 16);
                outfile << '\n';
            }
        }

        outfile << "*ELEMENT_SHELL\n";

        for (int i = 0; i < elts.rows()/4; i++)
        {
            outfile << RJust(std::to_string(i+1), 8);
            outfile << RJust(std::to_string(1), 8);
            outfile << RJust(std::to_string(elts(i*4)+1), 8);
            outfile << RJust(std::to_string(elts(i*4+1)+1), 8);
            outfile << RJust(std::to_string(elts(i*4+2)+1), 8);
            outfile << RJust(std::to_string(elts(i*4+3)+1), 8);
            outfile << '\n';
        }

        outfile << "*END\n";
        outfile.close();
    }
    void WriteGfxSurface(std::string filename, Geometry::Patch patch)
    {
        std::ofstream outfile{ filename };

        Geometry::Mesh m = patch.GetGfxMesh();

        outfile << "*NODE\n";

        for (int j = 0; j < m.nodes.cols(); j+=1)
        {
            for (int i = 0; i < m.nodes.rows(); i+=3)
            {
                outfile << RJust(std::to_string(j+1 + i/3*m.nodes.cols()), 8); 
                outfile << RJust(std::to_string(m.nodes(i, j)), 16);
                outfile << RJust(std::to_string(m.nodes(i+1, j)), 16);
                outfile << RJust(std::to_string(m.nodes(i+2, j)), 16);
                outfile << '\n';
            }
        }

        outfile << "*ELEMENT_SHELL\n";

        for (int i = 0; i < m.elts.rows()/3; i++)
        {
            outfile << RJust(std::to_string(i+1), 8);
            outfile << RJust(std::to_string(1), 8);
            outfile << RJust(std::to_string(m.elts(i*3)+1), 8);
            outfile << RJust(std::to_string(m.elts(i*3+1)+1), 8);
            outfile << RJust(std::to_string(m.elts(i*3+2)+1), 8);
            outfile << '\n';
        }

        outfile << "*END\n";
        outfile.close();
    }
}
