/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef OOX_DRAWINGML_CHART_PLOTAREACONVERTER_HXX
#define OOX_DRAWINGML_CHART_PLOTAREACONVERTER_HXX

#include "oox/drawingml/chart/converterbase.hxx"

namespace com { namespace sun { namespace star {
    namespace chart2 { class XDiagram; }
} } }

namespace oox {
namespace drawingml {
namespace chart {

// ============================================================================

struct View3DModel;
class TypeGroupConverter;

class View3DConverter : public ConverterBase< View3DModel >
{
public:
    explicit            View3DConverter( const ConverterRoot& rParent, View3DModel& rModel );
    virtual             ~View3DConverter();

    /** Converts the OOXML plot area model to a chart2 diagram. */
    void                convertFromModel(
                            const ::com::sun::star::uno::Reference< ::com::sun::star::chart2::XDiagram >& rxDiagram,
                            TypeGroupConverter& rTypeGroup );
};

// ============================================================================

struct WallFloorModel;

class WallFloorConverter : public ConverterBase< WallFloorModel >
{
public:
    explicit            WallFloorConverter( const ConverterRoot& rParent, WallFloorModel& rModel );
    virtual             ~WallFloorConverter();

    /** Converts the OOXML wall/floor model to a chart2 diagram. */
    void                convertFromModel(
                            const ::com::sun::star::uno::Reference< ::com::sun::star::chart2::XDiagram >& rxDiagram,
                            ObjectType eObjType );
};

// ============================================================================

struct PlotAreaModel;

class PlotAreaConverter : public ConverterBase< PlotAreaModel >
{
public:
    explicit            PlotAreaConverter( const ConverterRoot& rParent, PlotAreaModel& rModel );
    virtual             ~PlotAreaConverter();

    /** Converts the OOXML plot area model to a chart2 diagram. */
    void                convertFromModel( View3DModel& rView3DModel );
    /** Converts the manual plot area position and size, if set. */
    void                convertPositionFromModel();

    /** Returns the automatic chart title if the chart contains only one series. */
    inline const ::rtl::OUString& getAutomaticTitle() const { return maAutoTitle; }
    /** Returns true, if the chart is three-dimensional. */
    inline bool         is3dChart() const { return mb3dChart; }
    /** Returns true, if chart type supports wall and floor format in 3D mode. */
    inline bool         isWall3dChart() const { return mbWall3dChart; }

private:
    ::rtl::OUString     maAutoTitle;
    bool                mb3dChart;
    bool                mbWall3dChart;
    bool                mbPieChart;
};

// ============================================================================

} // namespace chart
} // namespace drawingml
} // namespace oox

#endif
