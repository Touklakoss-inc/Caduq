#include "EntityManager.h"

#include <memory>

namespace Caduq 
{
    void EntityManager::CreatePoint(const std::shared_ptr<Point>& point)
    {
        m_Point_List.push_back(point); // push_back make a copy of the shared pointer
        point->Init();
    }

    void EntityManager::CreateSpline(const std::shared_ptr<Spline>& spline)
    {
        m_Spline_List.push_back(spline);
        spline->Init();
    }

    void EntityManager::CreatePatch(const std::shared_ptr<Patch>& patch)
    {
        m_Patch_List.push_back(patch);
        patch->Init();
    }

    std::shared_ptr<Point>& EntityManager::GetPoint(int index)
    {
        return m_Point_List.at(index);
    }

    std::shared_ptr<Spline>& EntityManager::GetSpline(int index)
    {
        return m_Spline_List.at(index);
    }

    std::shared_ptr<Patch>& EntityManager::GetPatch(int index)
    {
        return m_Patch_List.at(index);
    }
}
