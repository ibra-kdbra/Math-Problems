#include "triangle_wrapper.hpp"

#include <assert.h>

/* #define SINGLE // also needs to be changed in triangle.c */
#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */
#include "triangle.h"

#include "util.hpp"

#define ATTRIB_SIZE 0

e_buffer_t triangulate_xy_plane(const samples_t& vertices) {
    assert(vertices.size() >= 3);

    e_buffer_t elements;

    struct triangulateio in, out;
    in.numberofpoints = vertices.size();
    in.numberofpointattributes = ATTRIB_SIZE;
    in.numberofsegments = vertices.size();
    in.numberofholes = 0;
    in.numberofregions = 0;

    in.pointlist = (REAL *)malloc(in.numberofpoints * 2 * sizeof(REAL));
    in.pointmarkerlist = (int *)NULL;
    in.pointattributelist = (REAL *)NULL;
    in.segmentlist = (int *)malloc(in.numberofsegments * 2 * sizeof(int));
    in.segmentmarkerlist = (int *)NULL;

    for (unsigned i = 0; i < in.numberofpoints; ++i) {
        in.pointlist[i*2 + 0] = vertices.at(i).x;
        in.pointlist[i*2 + 1] = vertices.at(i).y;
    }
    for (unsigned i = 0; i < in.numberofsegments; ++i) {
        in.segmentlist[i*2 + 0] = i;
        in.segmentlist[i*2 + 1] = (i+1) % in.numberofpoints;
    }

    out.pointlist = (REAL *)NULL;
    out.pointmarkerlist = (int *)NULL;
    out.pointattributelist = (REAL *)NULL;
    out.trianglelist = (int *)NULL;
    out.segmentlist = (int *)NULL;
    out.segmentmarkerlist = (int *)NULL;

    triangulate("zpQ", &in, &out, (struct triangulateio *)NULL);

    for (unsigned i = 0; i < out.numberoftriangles; ++i) {
        elements.push_back(out.trianglelist[i * out.numberofcorners + 0]);
        elements.push_back(out.trianglelist[i * out.numberofcorners + 1]);
        elements.push_back(out.trianglelist[i * out.numberofcorners + 2]);
    }

    free(in.pointlist);
    free(in.pointmarkerlist);
    free(in.pointattributelist);
    free(in.segmentlist);
    free(in.segmentmarkerlist);
    free(out.pointlist);
    free(out.pointmarkerlist);
    free(out.pointattributelist);
    free(out.segmentlist);
    free(out.segmentmarkerlist);
    free(out.trianglelist);

    return elements;
}
