// Global variables
const pi=Math.PI, tau=2*pi;                          // Used extensively
var simulation, overlay, mediacontrols;              // HTML DOM elements
var canvas, context;                                 // Canvas elements
var cw=1000, ch=1000, cw2, ch2;                      // Canvas dimensions
var demo=0, demoactive=false;                        // Demo settings
var count, elapsed, time, run, paused=true;          // Simulation parameters
var species=new Array(), particle=new Array();       // Species and particle parameters
var number, density, distribution;                   // Experimental setup
var size, trails, framerate, strobe, tint, colours;  // Graphics settings
var rMax, shape;                                     // Used extensively
var lut=new Array();      