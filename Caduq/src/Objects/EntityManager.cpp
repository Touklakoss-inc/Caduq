#include "EntityManager.h"

#include "Point.h"
#include "Vizir/Logging/Log.h"
#include <memory>

#include <vector>
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

    void EntityManager::DeletePoint(const std::shared_ptr<Point>& point)
    {
        bool canDelete = point->Delete();
    }
    void EntityManager::DeleteSpline(const std::shared_ptr<Spline>& spline)
    {
        bool canDelete = spline->Delete();
        if (canDelete)
        {
            auto it = std::find(m_Spline_List.begin(),m_Spline_List.end(), spline);
            if (it != m_Spline_List.end()) 
            {
                m_Spline_List.erase(it);
            }
        }
        else
            VZ_ERROR("Cannot delete object");
    }
    void EntityManager::DeletePatch(const std::shared_ptr<Patch>& patch)
    {
        // m_Patch_List.erase(patch);
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
