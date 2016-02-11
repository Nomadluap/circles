#ifndef PACKINGVIEW_HPP
#define PACKINGVIEW_HPP

#include <QGraphicsScene>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QWheelEvent>
#include <memory>
#include <QColor>
#include <QMap>

#include "packing/Packing.hpp"
#include "packing./PackingCoordinate.hpp"
#include "View/GraphicCircle.hpp"
#include "view/Text.hpp"


using namespace Circles;

namespace Circles{
    namespace View{

    /**
     * An extension of QGraphicsScene which visualizes a Packing of either type.
     *
     * The PackingView is repsonsible for actually rendering the image of the packing, as well as
     * drawing node connectors, indicies, center marks, and colourings of the displayed packing.
     */
    class PackingView: public QGraphicsScene
    {
        Q_OBJECT
    public:
        /**
         * Construct a PackingView with no internal Packing. In this case, an empty
         * EuclideanPacking will be assumed.
         */
        PackingView();

        /**
         * Construct a PackingView that views a specified packing.
         * @param p the packing to visualize.
         */
        PackingView(std::shared_ptr<Packing::Packing> p);

        PackingView(const PackingView& other);

        PackingView(PackingView&& other);

        PackingView& operator=(const PackingView& other);

        PackingView& operator=(PackingView&& other);

        /**
         * Set the color of a specified triangle to be displayed.
         * @param t Triangle to set.
         * @param c Color to set the triangle.
         */
        void setTriangleColor(Triangle t, QColor c);

        void clearTriangleColor();

        Packing::Packing& packing();

    public slots:
        void setDrawCircles     (bool state);
        void setDrawCenters     (bool state);
        void setDrawIndices     (bool state);
        void setDrawConnectors  (bool state);
        void setDrawColor       (bool state);

        /**
         * Set the packing that the PackingView is to display.
         * @param p the packing for the scene to display
         */
        void setPacking(std::shared_ptr<Packing::Packing> p);

        /**
         * Re-generate the graphics objects to represent the packing.
         *
         * this should be done after the packing is re-packed or re-laid.
         */
        void rebuildGraphics();

    signals:
        /**
         * Emitted when the packing that the PackingView is displaying has changed
         * @param p The new packing that the view is now displaying
         */
        void packingChanged(std::shared_ptr<Packing::Packing> p);

        /**
         * Passed when the view recieves a wheel event. Meant to be intercepted
         * by the containing widget to zoom the view.
         * @param e the event.
         */
        void gotMouseWheelEvent(int delta);

        void gotMousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);

    private:
        std::shared_ptr<Packing::Packing> packing_;
        QList<std::shared_ptr<View::GraphicCircle> > graphicCircles_;
        QList<std::shared_ptr<QGraphicsPolygonItem> > colorPolygons_;
        QList<std::shared_ptr<Text> > indices_;


        /// for storing triangle colors
        QMap<Graph::Triangle, QColor> triangleColors_;
        //TODO: make the other graphics item classes

        bool drawCircles_    = true;
        bool drawCenters_    = false;
        bool drawIndices_    = false;
        bool drawConnectors_ = false;
        bool drawColor_      = false; // because everyone loves american spelling I guess.

        /// Re-create the graphics objects for the packingView
        void rebuildCircles();
        void rebuildCenters();
        void rebuildIndices();
        void rebuildConnectors();
        void rebuildColor();
    protected:
        /**
         * override of default wheelEvent to emit the signal.
         * @param e
         */
        void wheelEvent(QGraphicsSceneWheelEvent* e) override;

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    };

    } // namespace UI
} // namespace Circles



#endif // PACKINGVIEW_HPP
