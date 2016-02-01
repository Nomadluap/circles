#ifndef PACKINGVIEW_HPP
#define PACKINGVIEW_HPP

#include <QGraphicsScene>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <memory>

#include "packing/Packing.hpp"
#include "packing./PackingCoordinate.hpp"
#include "ui/GraphicCircle.hpp"

using namespace Circles;

namespace Circles{
    namespace Ui{

    /**
     * An extension of QGraphicsScene which visualizes a Packing of either type.
     *
     * The PackingView is repsonsible for actually rendering the image of the packing, as well as
     * drawing node connectors, indicies, center marks, and colourings of the displayed packing.
     */
    class PackingView: public QGraphicsScene
    {
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

    signals:
        /**
         * Emitted when the packing that the PackingView is displaying has changed
         * @param p The new packing that the view is now displaying
         */
        void packingChanged(std::shared_ptr<Packing::Packing> p);

    private:
        std::shared_ptr<Packing::Packing> packing_;
        QList<std::shared_ptr<Ui::GraphicCircle> > graphicCircles_;

        bool _drawCircles    = false;
        bool _drawCenters    = false;
        bool _drawIndices    = false;
        bool _drawConnectors = false;
        bool _drawColor      = false; // because everyone loves american spelling I guess.


    };

    } // namespace UI
} // namespace Circles



#endif // PACKINGVIEW_HPP
