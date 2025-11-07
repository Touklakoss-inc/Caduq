#ifndef FILEWRITTING_H
#define FILEWRITTING_H

#include <Eigen/Dense>

namespace Geometry::Tools
{
    std::string RJust(std::string str, int chr_nb);

    void WriteMesh(std::string filename, Eigen::MatrixXd nodes);
}

#endif
