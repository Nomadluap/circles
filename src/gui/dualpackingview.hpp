#ifndef DUALPACKINGVIEW_HPP
#define DUALPACKINGVIEW_HPP

#include <QWidget>
#include <memory>
#include <cmath>

#include "packing/Packing.hpp"
#include "graph/Graph.hpp"
#include "packing/HyperPacking.hpp"
#include "packing/EuclidPacking.hpp"
#include "view/PackingView.hpp"

using namespace Circles;

namespace Ui {
class DualPackingView;
}

namespace Circles{
    namespace GUI{

    class DualPackingView : public QWidget
    {
        Q_OBJECT

    public:
        explicit DualPackingView(QWidget *parent = 0);
        ~DualPackingView();

    public slots:
        /**
         * Generate packingViews for the packings and send them to the GraphicsViews.
         * Also handles setting up any signals that may be required between the views and
         * the parent.
         */
        void generateViews();

        /**
         * Instruct the PackingView to display an existing Euclidean packing. The hyperbolic packing will be
         * generated from this packing.
         * @param p The packing to display.
         */
        void setPacking(std::shared_ptr<Packing::EuclidPacking> p);

        /**
         * Instruct the PackingView to display an existing Hyperbolic packing. The corresponding EuclideanPacking will
         * be generated from this packing.
         * @param p The HyperPacking to display.
         */
        void setPacking(std::shared_ptr<Packing::HyperPacking> p);

        /**
         * Set the view to display both the euclidean and hyperbolic packings generated from the selected graph.
         * @param g The graph to generate the packings from.
         * @param centerCircle The index of the node to be placed at the center
         * @param firstNeighbour The index of the second circle to be placed.
         * @param angle The angle from from the positive horizontal to place the firstNeighbour.
         */
        void setPacking(std::shared_ptr<Graph::Graph> g, int centerCircle, int firstNeighbour, qreal angle);

        /**
         * Perform a repack and layout of the hyperbolic packing.
         * @param epsilon The epsilon value to use for the repack.
         */
        void hyperRepackAndLayout();

        /**
         * Perform a repack and layout of the euclidean packing.
         * @param epsilon The epsilon value to use for the repack.
         */
        void euclidRepackAndLayout();

        /**
         * Set the magnitude of the epsilon value for repacking, as a power of 10.
         * @param mag Power of 10 to use as epsilon value;
         */
        void setEpsilonMagnitude(qreal mag);

        /**
         * Set the radius to use for the outer circles in the hyperbolic packing.
         * @param radius radius to use.
         */
        void setHyperRadius(qreal radius);

        /**
         * Zoom the euclidean view
         * @param steps
         */
        void zoomEuclideanView(int delta);

        /**
         * Zoom the hyper view.
         * @param steps
         */
        void zoomHyperView(int delta);

    private:
        Ui::DualPackingView *ui;

        /// holds the common graph to both packings
        std::shared_ptr<Graph::Graph> graph_;

        //reference to the hyperPacking currently being displayed.
        std::shared_ptr<Packing::HyperPacking> hyperPacking_;

        //reference to the EuclidPacking currently being displayed.
        std::shared_ptr<Packing::EuclidPacking> euclidPacking_;

        /// Views that actually display the packings.
        std::shared_ptr<View::PackingView> euclidView_;
        std::shared_ptr<View::PackingView> hyperView_;

        /// epsilon value for repacks.
        qreal epsilon_ = 10.0e-3;

        qreal hyperRadius_ = 5.0;

        /// default zoom rates
        const qreal EUCLID_ZOOM_DEFAULT = 33.0;
        const qreal HYPER_ZOOM_DEFAULT = 100.0;

        /// current zoom levels
        qreal euclidZoom_ = EUCLID_ZOOM_DEFAULT;
        qreal hyperZoom_ = HYPER_ZOOM_DEFAULT;

        const qreal ZOOM_PER_MOUSECOUNT = 1.1;


        /**
         * Generate the colors for the packing based on the unit disc coloring defined in PackingView
         * @param p Packing to generate for
         */
        void generateHyperColors(View::PackingView& p);

    };
    }
}

#endif // DUALPACKINGVIEW_HPP
