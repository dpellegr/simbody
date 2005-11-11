#ifndef SIMTK_PLACEMENT_H_
#define SIMTK_PLACEMENT_H_

/* Copyright (c) 2005-6 Stanford University and Michael Sherman.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including 
 * without limitation the rights to use, copy, modify, merge, publish, 
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/** @file
 * Placements definitions.
 */

#include "SimbodyCommon.h"

#include <iostream>

namespace simtk {

class Feature;
class Station;
class Direction;
class Orientation;
class Frame;
class RealParameter;
class StationParameter;
class RealMeasure;
class StationMeasure;

// Declared below.
class Placement;
class FeaturePlacement;
class RealPlacement;
class RealConstantPlacement;
class StationPlacement;
class StationConstantPlacement;
class DirectionPlacement;
class OrientationPlacement;
class FramePlacement;


// 
// <placement>     ::= constant | parameter | feature.placement
//                     | func(<placement> ...)
//
// Features provide a list of the placement types they need. Each of
// these list elements must be associated with a <placementExpr> eventually.
// Then these are evaluated at the appropriate runtime stage.
//
// <placement>'s have value types scalar, station, direction,
// orientation, or frame.
//                     

class Placement {
public:
    Placement() : rep(0) { }
    Placement(const Placement&);
    Placement& operator=(const Placement&);
    ~Placement();

    bool           hasOwner() const;
    const Feature& getOwner() const;
    int            getIndexInOwner() const;

    String toString(const String& linePrefix="") const;

protected:
    class PlacementRep* rep;
    friend class PlacementRep;
};
std::ostream& operator<<(std::ostream& o, const Placement&);


/**
 * Create a Placement which is evaluated by returning the
 * value of the indicated Feature's Placement. This is
 * deferred until runtime; the Feature may not even have
 * a placement yet.
 */
class FeaturePlacement : public Placement {
public:
    explicit FeaturePlacement(const Feature&);

private:
};

class RealPlacement : public Placement {
public:
    RealPlacement(const Real&);
    RealPlacement(const RealParameter&);
    static RealPlacement plus  (const RealPlacement& l,
                                const RealPlacement& r);
    static RealPlacement minus (const RealPlacement& l,
                                const RealPlacement& r);
    static RealPlacement times (const RealPlacement& l,
                                const RealPlacement& r);
    static RealPlacement divide(const RealPlacement& l,
                                const RealPlacement& r);
    static RealPlacement length(const DirectionPlacement&);
private:
};

class RealConstantPlacement : public RealPlacement {
public:
    RealConstantPlacement(const Real&);
};

class StationPlacement : public Placement {
public:
    StationPlacement(const Vec3&);    // implicit conversion
    StationPlacement(const Station&); //   "
    StationPlacement(const Frame&);   //   "

    StationPlacement(const Direction&   v,
                     const RealMeasure& scale);

    StationPlacement(const RealMeasure& x,
                     const RealMeasure& y,
                     const RealMeasure& z);

    static StationPlacement   plus(const StationPlacement&,
                                   const DirectionPlacement&);
private:
};

class DirectionPlacement : public Placement {
public:
    DirectionPlacement(const Vec3&);      // implicit conversion
    DirectionPlacement(const Direction&); //   "

    static DirectionPlacement plus (const DirectionPlacement& l,
                                    const DirectionPlacement& r);
    static DirectionPlacement minus(const StationPlacement& head,
                                    const StationPlacement& tail);
    static DirectionPlacement minus(const DirectionPlacement& l,
                                    const DirectionPlacement& r);
private:
};

// Three, mutually orthogonal, right handed directions.
class OrientationPlacement : public Placement {
public:
    OrientationPlacement(); // identity
    OrientationPlacement(const Mat33&);      // implicit conversion
private:
};

class FramePlacement : public Placement {
public:
    FramePlacement(const Frame&);   // implicit conversions
    FramePlacement(const Station&); //   orientation inherited from 
                                    //   Station's parent
    FramePlacement(const Orientation&); // origin inherited from
                                        // Orientation's parent
    FramePlacement(const Orientation&, const Station&);
private:
};

} // namespace simtk

#endif // SIMTK_PLACEMENT_H_
