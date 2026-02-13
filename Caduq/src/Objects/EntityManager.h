#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>

#include "Geometry/Geo.h"
#include "XPBD/PhyXManager.h"
#include "Entity.h"

namespace Caduq 
{
    class Entity;
    class Point;
    class Spline;
    class Patch;
    class Frame;

    class EntityManager
    {
    private:
        std::shared_ptr<Frame> m_MainFrame;
        std::shared_ptr<XPBD::PhyXManager> m_PhyXManager;

        std::vector<std::weak_ptr<Point>> m_PointList{ };
        std::vector<std::weak_ptr<Spline>> m_SplineList{ };
        std::vector<std::weak_ptr<Patch>> m_PatchList{ };

        std::vector<std::shared_ptr<Entity>> m_EntityList{ };
        std::vector<std::shared_ptr<Entity>> m_EntityToDelete{ };

        std::shared_ptr<Entity> m_CurEntity { nullptr };

        template<typename T> 
        void CleanWeakPtrList(std::vector<std::weak_ptr<T>>& list)
        {
            list.erase(std::remove_if(list.begin(), list.end(), [](const std::weak_ptr<T>& l) { return l.expired(); }), list.end());
        }

    public:
        EntityManager(const std::shared_ptr<Frame>& frame);
        virtual ~EntityManager() = default;
        const std::shared_ptr<Frame>& GetMainFrame() { return m_MainFrame; };
        const std::shared_ptr<XPBD::PhyXManager>& GetPhyXManager() { return m_PhyXManager; };

        template<typename T> 
        void CreateEntity(const std::shared_ptr<T>& entity);

        void DeleteEntity(const std::shared_ptr<Entity>& entity);
        void ClearEntityToDelete(); // Call at the begining of RenderImGui()

        void RenderImGui();

        const std::vector<std::shared_ptr<Entity>>& GetEntityToDelete() { return m_EntityToDelete; };

        void SetCurEntity(const std::shared_ptr<Entity>& curEntity) { m_CurEntity = curEntity; };
        const std::shared_ptr<Entity>& GetCurEntity() { return m_CurEntity; };

        const std::vector<std::shared_ptr<Entity>>& GetEntityList() { return m_EntityList; };
        const std::vector<std::weak_ptr<Point>>& GetPointList() { return m_PointList; };
        const std::vector<std::weak_ptr<Spline>>& GetSplineList() { return m_SplineList; };
        const std::vector<std::weak_ptr<Patch>>& GetPatchList() { return m_PatchList; };

        std::shared_ptr<Entity>& GetEntity(int index) { return m_EntityList.at(index); };
        std::weak_ptr<Point>& GetPoint(int index) { return m_PointList.at(index); };
        std::weak_ptr<Spline>& GetSpline(int index) { return m_SplineList.at(index); };
        std::weak_ptr<Patch>& GetPatch(int index) { return m_PatchList.at(index); };
    };

    template<typename T> 
    void EntityManager::CreateEntity(const std::shared_ptr<T>& entity)
    {
        m_EntityList.push_back(entity);
        if constexpr (std::is_same_v<T, Point>) 
        {
            m_PhyXManager->AddPhyXPointToList(entity->GetPhyXPoint());
            m_PointList.push_back(entity);
        }
        else if constexpr (std::is_same_v<T, Spline>) 
            m_SplineList.push_back(entity);
        else if constexpr (std::is_same_v<T, Patch>) 
            m_PatchList.push_back(entity);

        entity->Init();
        VZ_INFO(entity->GetName() + " created");
    }
}

#endif
