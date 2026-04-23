#ifndef JOINTSBB_H
#define JOINTSBB_H

#include "Eigen/Core"
#include <optional>
#include <Eigen/Dense>

namespace XPBD
{
    struct JAttach
    {
        int p1;
        std::optional<int> p2;
        double dRest;
        double alpha;
        Eigen::Vector3d pos1;
        Eigen::Vector3d pos2;
    };

    struct JRestrictAxis
    {
        Eigen::Vector3d axis;
        Eigen::Vector3d pos1;
        Eigen::Vector3d pos2;
        double posMin;
        double posMax;
        double alpha;
        std::optional<int> p2;
        int p1;
    };

    struct JAlignTwoAxes
    {
        Eigen::Vector3d a1;
        Eigen::Vector3d a2;
        double alpha;
        std::optional<int> p2;
        int p1;
    };
}
#endif
