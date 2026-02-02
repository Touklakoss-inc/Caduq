#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>

#include "Entity.h"
#include "Geometry/Geo.h"
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
        std::vector<std::shared_ptr<Point>> m_Point_List;
        std::vector<std::shared_ptr<Spline>> m_Spline_List;
        std::vector<std::shared_ptr<Patch>> m_Patch_List;

        std::vector<std::shared_ptr<Entity>> m_EntityToDelete;

        std::shared_ptr<Entity> m_CurEntity { nullptr };

        bool m_PointPopupOpened { false };
        bool m_SplinePopupOpened { false };
        bool m_PatchPopupOpened { false };
        bool m_FirstPopupOpening { false };

        bool m_CreatePatch4 { true }; // if false => create patch 3

        void DeletePoint(const std::shared_ptr<Point>& point);
        void DeleteSpline(const std::shared_ptr<Spline>& spline);
        void DeletePatch(const std::shared_ptr<Patch>& patch);

        /* ImGui */
        // Point
        float m_PointPopupCoord[3] { 0.0f, 0.0f, 0.0f };

        // Spline
        int start_point_idx = 0; // Here we store our selection data as an index.
        float start_tangency[3] = { 0.0f, 0.0f, 0.0f };
        float start_tension[1] = { 1.0f };

        int end_point_idx = 0; // Here we store our selection data as an index.
        float end_tangency[3] = { 0.0f, 0.0f, 0.0f };
        float end_tension[1] = { 1.0f };

        // Patch
        int spline_1_idx = 0; // Here we store our selection data as an index.
        int spline_2_idx = 0; // Here we store our selection data as an index.
        int spline_3_idx = 0; // Here we store our selection data as an index.
        int spline_4_idx = 0; // Here we store our selection data as an index.

        void PointPopup();
        void SplinePopup();
        void PatchPopup();

    public:
        EntityManager();

        void RenderImGui();

        void CreatePoint(const std::shared_ptr<Point>& point);
        void CreateSpline(const std::shared_ptr<Spline>& spline);
        void CreatePatch(const std::shared_ptr<Patch>& patch);

        void DeleteEntity(const std::shared_ptr<Entity>& entity);
        void ClearEntityToDelete();
        auto GetEntityToDelete() { return m_EntityToDelete; };
        void SetCurEntity(const std::shared_ptr<Entity>& curEntity) { m_CurEntity = curEntity; };
        void PointPopupOpened() { m_PointPopupOpened = true; };
        void SplinePopupOpened() { m_SplinePopupOpened = true; };
        void PatchPopupOpened() { m_PatchPopupOpened = true; };
        void FirstPopupOpening() { m_FirstPopupOpening = true; };

        // should it be returned by reference ?
        const std::vector<std::shared_ptr<Point>>& GetPointList() { return m_Point_List; };
        const std::vector<std::shared_ptr<Spline>>& GetSplineList() { return m_Spline_List; };
        const std::vector<std::shared_ptr<Patch>>& GetPatchList() { return m_Patch_List; };

        std::shared_ptr<Point>& GetPoint(int index);
        std::shared_ptr<Spline>& GetSpline(int index);
        std::shared_ptr<Patch>& GetPatch(int index);

        /* ImGui */
        void SetPointPopupCoord(Eigen::Vector3f coord) {
            for (int i = 0; i < 3; i++) m_PointPopupCoord[i] = coord[i]; };
        void SetSplinePopupParam(Geometry::SplinePoint startPoint, int startPointID, Geometry::SplinePoint endPoint, int endPointID);
        void SetPatchPopupParam(int spline1ID, int spline2ID, int spline3ID, int spline4ID);
    };
}

#endif
