#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>

#include "Point.h"
#include "Spline.h"
#include "Patch.h"
#include "Entity.h"

namespace Caduq 
{
    class EntityManager
    {
    private:
        std::vector<std::shared_ptr<Point>> m_Point_List;
        std::vector<std::shared_ptr<Spline>> m_Spline_List;
        std::vector<std::shared_ptr<Patch>> m_Patch_List;

        void DeletePoint(const std::shared_ptr<Point>& point);
        void DeleteSpline(const std::shared_ptr<Spline>& spline);
        void DeletePatch(const std::shared_ptr<Patch>& patch);

    public:
        EntityManager() = default;

        void RenderImGui();

        void CreatePoint(const std::shared_ptr<Point>& point);
        void CreateSpline(const std::shared_ptr<Spline>& spline);
        void CreatePatch(const std::shared_ptr<Patch>& patch);

        void DeleteEntity(const std::shared_ptr<Entity>& entity);

        // should it be returned by reference ?
        const std::vector<std::shared_ptr<Point>>& GetPointList() { return m_Point_List; };
        std::vector<std::shared_ptr<Spline>> GetSplineList() { return m_Spline_List; };
        std::vector<std::shared_ptr<Patch>> GetPatchList() { return m_Patch_List; };

        std::shared_ptr<Point>& GetPoint(int index);
        std::shared_ptr<Spline>& GetSpline(int index);
        std::shared_ptr<Patch>& GetPatch(int index);
    };
}

#endif
