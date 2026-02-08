#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>

#include "Entity.h"
#include "Geometry/Geo.h"
#include "XPBD/PhyXManager.h"
#include "Point.h"
#include "Spline.h"
#include "Patch.h"

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

        std::vector<std::shared_ptr<Point>> m_Point_List;
        std::vector<std::shared_ptr<Spline>> m_Spline_List;
        std::vector<std::shared_ptr<Patch>> m_Patch_List;

        std::vector<std::shared_ptr<Entity>> m_EntityToDelete;

        std::shared_ptr<Entity> m_CurEntity { nullptr };

        void DeletePoint(const std::shared_ptr<Point>& point);
        void DeleteSpline(const std::shared_ptr<Spline>& spline);
        void DeletePatch(const std::shared_ptr<Patch>& patch);

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

    public:
        EntityManager();
        const auto& GetPhyXManager() { return m_PhyXManager; };

        void RenderImGui();

        void CreatePoint(const std::shared_ptr<Point>& point);
        void CreateSpline(const std::shared_ptr<Spline>& spline);
        void CreatePatch(const std::shared_ptr<Patch>& patch);

        void DeleteEntity(const std::shared_ptr<Entity>& entity);
        void ClearEntityToDelete();
        auto GetEntityToDelete() { return m_EntityToDelete; };
        void SetCurEntity(const std::shared_ptr<Entity>& curEntity) { m_CurEntity = curEntity; };

        // should it be returned by reference ?
        const std::vector<std::shared_ptr<Point>>& GetPointList() const { return m_Point_List; };
        const std::vector<std::shared_ptr<Spline>>& GetSplineList() { return m_Spline_List; };
        const std::vector<std::shared_ptr<Patch>>& GetPatchList() { return m_Patch_List; };

        std::shared_ptr<Point>& GetPoint(int index) { return m_Point_List.at(index); };
        std::shared_ptr<Spline>& GetSpline(int index) { return m_Spline_List.at(index); };
        std::shared_ptr<Patch>& GetPatch(int index) { return m_Patch_List.at(index); };

        /* ImGui */
        void SetPointPopupParam(Eigen::Vector3f coord) {
            for (int i = 0; i < 3; i++) m_GuiPointPopupCoord[i] = coord[i]; };
        void SetSplinePopupParam(Geometry::SplinePoint startPoint, int startPointID, Geometry::SplinePoint endPoint, int endPointID);
        void SetPatchPopupParam(int spline1ID, int spline2ID, int spline3ID, int spline4ID);
    };
}

#endif
