
// This focuses primarily on specific values of alpha, beta, v and r
// It goes on to show the effects of altering the values of alpha and beta
// Here you can also explore various values of gamma, the ratio of v and r
//    Gamma (γ) = speed of particles (v) / radius of neighbourhoods (r)
// Particle behaviour is only dependent on values of alpha, beta and gamma
// Any particular combination of alpha, beta and gamma is called a species
// Differences in particle density will affect a species overall behaviour
// The simulation is scale invariant and works the same at any screen size
// You can also increase or decrease particle density by zooming in or out

// Global variables
const pi=Math.PI, tau=2*pi;                     // Used extensively
var canvas, context;                            // HTML canvas DOM elements
var p, a, b, g, caption, demo=0, cs=1, ps=1;    // Particle parameters
var vw, vw2, vh, vh2, z, dpr, cw, ch;           // Viewport and HTML canvas
var r, r2, v, sr, sr2, vwsr4, vhsr4, srdrp;     // Used extensively
var count, elapsed, time, dist=0, paused=true;  // Simulation parameters
var hist=new Array(), hl=100, analyse=false;    // Analytics
var hues=new Array();                           // Look-up table

// Default settings
var n=600;  // Number of particles (100 to 10,000)
var d=8.0;  // Particle density at 100% zoom (1.0 to 10.0)
var s=0.1;  // Visible radius of particles relative to r (0.01 to 1.00)
var o=1.0;  // Background opacity (1.0 to 0.1)
var f=25;   // Maximum frames rate (10 to 100)

// Preset values of alpha, beta and gamma
var preset=[[180,17,0.134,n,d,s,o,15,0,'Cell-like behaviour'],
  [5,31,0.166,n,2,s,0.1,f,0,'Sprites'],
  [-23,2,0.166,n,10,s,0.6,f,0,'Donuts'],
  [114,-4,0.166,800,d,s,0.4,f,0,'Goblins'],
  [173,-8,0.200,1000,d,s,0.1,f,0,'Puff balls'],
  [22,-29,0.125,400,d,s,0.1,f,0,'Lava lamp'],
  [174,15,0.200,1000,10,s,0.6,f,0,'Thick cell walls'],
  [-90,-90,0.333,1000,2,s,0.1,f,0,'Liquid crystal'],
  [-20,-25,0.143,1000,d,s,0.6,f,0,'Gradual evolution'],
  [146,8,0.125,1000,20,0.075,0.6,f,0,'Thermophiles'],
  [117,-4,0.143,1000,18,0.075,0.6,f,0,'Amoebas merging and evolving'],
  [35,-5,0.134,1000,32,0.075,0.6,f,0,'Extreme density'],
  [-70,5,0.13,1600,d,0.075,0.5,f,1,'Nascent organelles'],
  [-177,3,0.27,1600,10,0.075,0.6,f,1,'Shockwave'],
  [-69,-1,0.205,2000,d,0.05,0.3,f,1,'Rainbow juice'],
  [42,9,0.275,1500,5,0.075,0.6,f,1,'Cell nucleus'],
  [0,13,0.2,2000,10,0.075,0.2,f,0,'Exchanging particles'],
  [139,-28,0.1,100,2,0.05,0.1,100,1,'Silly string']];
  
  // Run simulation
function initialize() {
  // Load values of alpha, beta and gamma
  let url=window.location.search.substring(1).split(",");                           // Check URL for settings
  if (!url[0]) species(0);                                                          // Default species
  else if (url.length==1) { demo=url; species(url); }                               // Load species presets
  else if (url.length>=3) species(url[0],url[1],url[2]);                            // Set species parameters
  if (url.length>3) n=url[3], d=url[4], s=url[5], o=url[6], f=url[7], dist=url[8];  // Environmental controls
  
  // Setup HTML canvas
  canvas=document.getElementById("canvas");
  context=canvas.getContext("2d");
  refresh();
  
  // Enable keyboard inputs
  document.addEventListener('keydown',keyboard);
}

  // Set values of alpha, beta and gamma
  function species(abg) {
    caption='&emsp;';
    if (arguments.length==0) {
      // Randomize values (3600 × 1800 × 500 = 3.24 billion possible values!)
      a=Math.round(Math.random()*3600)/10-180;  // Alpha (-180.0° to +180.0°)
      b=Math.round(Math.random()*1800)/10-90;   // Beta (-90.0° to +90.0°)
      g=Math.ceil(Math.random()*500)/1000;      // Gamma (0.001 to 0.500)
    } else if (arguments.length==1) {
      // Use preset values
      let pre=preset[abg];
      a=pre[0], b=pre[1], g=pre[2];
      n=pre[3], d=pre[4], s=pre[5], o=pre[6], f=pre[7];
      dist=pre[8], caption=pre[9];
    } else {
      // Use selected values
      a=arguments[0], b=arguments[1], g=arguments[2];
    }
    
    // Convert values to radians!
    a=(a/360)*tau, b=(b/360)*tau;
    
    // Update user interface
    showabg(); sliders();
  }

    // Display current settings
    function showabg() {
      document.getElementById("info").innerHTML='<h1>'+caption+'</h1>';
      let sHTML='α&thinsp;'+Math.round((a/pi)*1800)/10+'°&emsp;β&thinsp;'+Math.round((b/pi)*1800)/10+'°&emsp;γ&thinsp;'+Math.round(g*1000)/1000;
      document.getElementById("settings").innerHTML=sHTML;
    }

      // Apply settings to control sliders
  function sliders() {
    document.getElementById("alpha").value=Math.round((a/pi)*1800)/10;
    document.getElementById("beta").value=Math.round((b/pi)*1800)/10;
    document.getElementById("gamma").value=g;
    document.getElementById("number").value=n;
    document.getElementById("num").innerHTML=n;
    document.getElementById("density").value=d;
    document.getElementById("den").innerHTML=Math.round(d*z*z*100)/100;
    document.getElementById("size").value=s;
    document.getElementById("trails").value=1-o;
    document.getElementById("framerate").value=f;
    document.getElementById("rate").innerHTML=f;
  }
  
  // Begin new simulation
  function refresh(rd) {
    if (!paused) playPause();
    if (rd!==undefined) dist=rd;
    count=0, elapsed=0;
    scale(); setvr(); sliders();
    if (n>10000) p=new Array(eval(n));
    else p=new Array(10000);
    for (i=0; i<p.length; i++) p[i]=new Particle();
    playPause();
  }
  
  // Play/pause simulation
  function playPause() {
    if (paused) {
      // Play simulation
      paused=false;
      time=new Date().getTime();
      document.getElementById("controls").style.right='-44vh';
      run=setInterval(step,1000/f);
    } else {
      // Pause simulation
      paused=true;
      elapsed+=(new Date().getTime()-time);
      document.getElementById("controls").style.right='0';
      window.clearTimeout(run);
    }
  }
  
