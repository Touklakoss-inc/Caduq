#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>

#include "Geometry/Geo.h"
#include "XPBD/PhyXManager.h"

namespace Caduq 
{
    class Entity;
    class Point;
    class Spline;
    class Patch;

    class EntityManager
    {
    private:

        std::shared_ptr<XPBD::PhyXManager> m_PhyXManager;

        std::vector<std::weak_ptr<Point>> m_PointList;
        std::vector<std::weak_ptr<Spline>> m_SplineList;
        std::vector<std::weak_ptr<Patch>> m_PatchList;

        std::vector<std::shared_ptr<Entity>> m_EntityList;
        std::vector<std::shared_ptr<Entity>> m_EntityToDelete;

        std::shared_ptr<Entity> m_CurEntity { nullptr };

        /* ImGui */
        // Point
        float m_GuiPointPopupCoord[3] { 0.0f, 0.0f, 0.0f };

        // Spline
        int m_GuiStartPointID = 0; // Here we store our selection data as an index.
        float m_GuiStartTangent[3] = { 0.0f, 0.0f, 0.0f };
        float m_GuiStartTension[1] = { 1.0f };

        int m_GuiEndPointID = 0; // Here we store our selection data as an index.
        float m_GuiEndTangent[3] = { 0.0f, 0.0f, 0.0f };
        float m_GuiEndTension[1] = { 1.0f };

        // Patch
        int m_GuiSpline1ID = 0; // Here we store our selection data as an index.
        int m_GuiSpline2ID = 0; // Here we store our selection data as an index.
        int m_GuiSpline3ID = 0; // Here we store our selection data as an index.
        int m_GuiSpline4ID = 0; // Here we store our selection data as an index.

        void PointPopup();
        void SplinePopup();
        void PatchPopup();

        template<typename T> 
        void CleanWeakPtrList(std::vector<std::weak_ptr<T>>& list);

    public:
        EntityManager();
        const auto& GetPhyXManager() { return m_PhyXManager; };

        void RenderImGui();

        void CreateEntity(const std::shared_ptr<Entity>& entity);

        void DeleteEntity(const std::shared_ptr<Entity>& entity);
        void ClearEntityToDelete();
        auto GetEntityToDelete() { return m_EntityToDelete; };
        void SetCurEntity(const std::shared_ptr<Entity>& curEntity) { m_CurEntity = curEntity; };

        // should it be returned by reference ?
        const std::vector<std::shared_ptr<Entity>>& GetEntityList() { return m_EntityList; };

        std::shared_ptr<Entity>& GetEntity(int index) { return m_EntityList.at(index); };
        std::weak_ptr<Point>& GetPoint(int index) { return m_PointList.at(index); };
        std::weak_ptr<Spline>& GetSpline(int index) { return m_SplineList.at(index); };
        std::weak_ptr<Patch>& GetPatch(int index) { return m_PatchList.at(index); };

        /* ImGui */
        void SetPointPopupParam(Eigen::Vector3f coord) {
            for (int i = 0; i < 3; i++) m_GuiPointPopupCoord[i] = coord[i]; };
        void SetSplinePopupParam(Geometry::SplinePoint startPoint, int startPointID, Geometry::SplinePoint endPoint, int endPointID);
        void SetPatchPopupParam(int spline1ID, int spline2ID, int spline3ID, int spline4ID);
    };
}

#endif
