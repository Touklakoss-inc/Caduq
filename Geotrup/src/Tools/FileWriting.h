#ifndef FILEWRITTING_H
#define FILEWRITTING_H

#include <Eigen/Dense>
#include "Geometry/Spline.h"
#include "Geometry/Patch.h"

namespace Geometry::Tools
{
    std::string RJust(std::string str, int chr_nb);

    void WriteMesh(std::string filename, std::vector<Geometry::Spline> nodes);
    void WriteNodes(std::string filename, Eigen::MatrixXd nodes);
    void WriteSurface(std::string filename, Geometry::Patch patch);
}

#endif
