#ifndef JATTACH_H
#define JATTACH_H

#include "Joint.h"
#include "Point.h"

#include "XPBD/PhyXManager.h"
#include <memory>
namespace XPBD
{
    class JAttach : public Joint
    {
    private:
        std::shared_ptr<Point> m_P1;
        std::shared_ptr<Point> m_P2;
        double m_DRest;
        double m_Alpha;

    public:
        JAttach(const std::shared_ptr<Point>& p1, const std::shared_ptr<Point>& p2, double dRest, double alpha);

        void Init() override;
        void Delete() override;
        void ApplyConstraints(double dts) override;

        void RenderImGui(const PhyXManager& phyXManager) override;
    };
}

#endif
