
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
  
  // Create particle
  function Particle() {
    if (dist==0) {
      // Uniform distribution
      this.x=Math.random()*vw;
      this.y=Math.random()*vh;
    } else if (dist==1) {
      // Centre-weighted distribution
      let mx=Math.max(0,(vw-vh)/2), my=Math.max(0,(vh-vw)/2), min=Math.min(vw,vh);
      this.x=mx+((Math.random()+Math.random()+Math.random())/3)*min;
      this.y=my+((Math.random()+Math.random()+Math.random())/3)*min;
    }
    this.phi=Math.random()*tau;    // Set random orientation
    this.pSin=Math.sin(this.phi);  // Sine of phi
    this.pCos=Math.cos(this.phi);  // Cosine of phi
    this.N=0;                      // Number of neighbours
    this.L=0;                      // Neighbours on left
    this.R=0;                      // Neighbours on right
  }

  // Set scale of viewport, canvas and particles
  function scale() {
    // Screen dimensions
    vw=window.innerWidth, vw2=vw/2;                  // Viewport width (CSS pixels)
    vh=window.innerHeight, vh2=vh/2;                 // Viewport height (CSS pixels)
    z=Math.round(((window.outerWidth-8)/vw)*20)/20;  // Browser zoom level (desktop)
    dpr=Math.round(window.devicePixelRatio*20)/20;   // Device Pixel Ratio (mobile)
    cw=(vw*dpr), ch=(vh*dpr);                        // HTML canvas dimensions
    
    // Set display size (CSS pixels)
    canvas.style.width=vw+'px';
    canvas.style.height=vh+'px';
    
    // Set actual size in memory (scaled to DPR)
    canvas.width=cw;
    canvas.height=ch;
    
    // Normalize coordinate system to use CSS pixels
    context.scale(dpr,dpr);
    
    console.log('Viewport: vw='+vw+' vh='+vh+'\nScaling: z='+z+' dpr='+dpr+'\nCanvas: cw='+cw+' ch='+ch);
  }

    // Calculate r and v based on viewport dimensions, density and gamma
    function setvr() {
      r=Math.sqrt((vw*vh*z*z*d)/(n*pi)), r2=r*r;                                   // Radius of neighbourhoods (r)
      v=g*r;                                                                       // Speed of particles
      sr=s*r, sr2=sr*2, sr4=sr*4, vwsr4=(sr*4)+vw, vhsr4=(sr*4)+vh, srdpr=sr*dpr;  // Scale visible size of particles
      console.log('r='+r+'\nv='+v+'\ns='+sr+'\ng='+g);                             // Display values in the console
    }
  
        
  // Advance simulation by one step (this function is called f times per second)
  function step() {
    count++;
    if (analyse) {
      hl-=0.005*(hl-100);   // Gradually shorten histograms
      hist[0]=new Array();  // Number of neighbours (N)
      hist[1]=new Array();  // Neighbours on left (L)
      hist[2]=new Array();  // Neighbours on right (R)
      hist[3]=new Array();  // Grid cell densities
    }
    
    // Create new grid
    let xCells=Math.floor(vw/r), xSize=vw/xCells;
    let yCells=Math.floor(vh/r), ySize=vh/yCells;
    let grid=new Array(xCells);
    for (i=0;i<xCells;i++) {
      grid[i]=new Array(yCells);
      for (j=0;j<yCells;j++) grid[i][j]=new Array();
    }
    
    for (i=0; i<n; i++) {
      // Insert particle index into a grid cell
      let xg=Math.floor(p[i].x/xSize);
      let yg=Math.floor(p[i].y/ySize);
      grid[xg][yg].push(i);
      // Calculate sine and cosine of phi
      p[i].pSin=Math.sin(p[i].phi);
      p[i].pCos=Math.cos(p[i].phi);
      // Reset values of N, L and R
      p[i].N=0, p[i].L=0, p[i].R=0;
    }
    
    // Count neighbours (N, L and R values) for each particle
    for (xg=0; xg<xCells; xg++) {							                  // Step through the grid cells (left to right)
      for (yg=0; yg<yCells; yg++) {                             // Step through the grid cells (top to bottom)
        for (gp=0; gp<grid[xg][yg].length; gp++) {		          // Step through the particles in each grid
          let i=grid[xg][yg][gp];                               // Now we have a particle index and we're already in the right grid
          for (xc=xg-1; xc<=xg+1; xc++) {                       // Iterate through the 9 neighbour cells (left to right)
            for (yc=yg-1; yc<=yg+1; yc++) {                     // Iterate through the 9 neighbour cells (top to bottom)
              let sxc=scope(xc,xCells), syc=scope(yc,yCells);   // Wrap screen edges, Pac-Man style
              for (jp=0; jp<grid[sxc][syc].length; jp++) {      // Iterate through the particles in each cell			
                let j=grid[sxc][syc][jp];                       // Finally we have both i & j
                if (i<j) {                                      // Still not gonna check it twice
                  let sX=scope(p[j].x-p[i].x+vw2,vw)-vw2;       // Calculate x-axis separation
                  let sY=scope(p[j].y-p[i].y+vh2,vh)-vh2;       // Calculate y-axis separation
                  let sD=(sX*sX)+(sY*sY);                       // Calculate the square of the separation distance
                  if (sD<=r2) {                                 // If separation distance is less than r
                    p[i].N++, p[j].N++;                         // Increase total neighbour counts
                    if (sX*p[i].pSin-sY*p[i].pCos<0) p[i].R++;  // Particle j is to the right of i
                    else p[i].L++;                              // Particle j is to the left of i
                    if (sX*p[j].pSin-sY*p[j].pCos>0) p[j].R++;  // Particle i is to the right of j
                    else p[j].L++;                              // Particle i is to the left of j
                  }
                }
              }
            }
          }
        }
      }
    }
    
    // Clear HTML canvas (or leave trails)
    context.fillStyle='rgba(0,0,0,'+o+')';
    context.fillRect(0,0,vw,vh);
    
    // Draw particles on HTML canvas then apply formula
    for (i=0; i<n; i++) {
      if (ps==0) {
        // Draw vectors (arrows)
        context.beginPath();
        context.moveTo(p[i].x,p[i].y);
        context.lineTo(p[i].x+(v*p[i].pCos),p[i].y+(v*p[i].pSin));
        context.moveTo(p[i].x+(v*p[i].pCos)+((v*0.25+4)*Math.cos(p[i].phi+3.5)),p[i].y+(v*p[i].pSin)+((v*0.25+4)*Math.sin(p[i].phi+3.5)));
        context.lineTo(p[i].x+(v*p[i].pCos),p[i].y+(v*p[i].pSin));
        context.lineTo(p[i].x+(v*p[i].pCos)+((v*0.25+4)*Math.cos(p[i].phi-3.5)),p[i].y+(v*p[i].pSin)+((v*0.25+4)*Math.sin(p[i].phi-3.5)));
        context.strokeStyle='hsl('+hue(p[i].N)+',100%,50%)';
        context.stroke();
        context.fillStyle='#FFF';
        context.fillRect(p[i].x-1,p[i].y-1,2,2);
        context.closePath();
      } else {
        if (srdpr>2) {
          // Draw a circle
          context.beginPath();
          context.arc(p[i].x,p[i].y,sr,0,tau);
          context.fillStyle='hsl('+hue(p[i].N)+',100%,50%)';
          context.fill();
          context.closePath();
        } else {
          // Draw a small square
          context.fillStyle='hsl('+hue(p[i].N)+',100%,50%)';
          context.fillRect(p[i].x-sr,p[i].y-sr,sr2,sr2);
        }
        
        // Draw radius, orientation and gamma (r, phi and g)
        if ((n<=500) && (f<=15)) {
          context.beginPath();
          context.moveTo(p[i].x,p[i].y);
          context.lineTo(p[i].x+(v*Math.cos(p[i].phi)),p[i].y+(v*Math.sin(p[i].phi)));
          context.strokeStyle='#666';
          context.stroke();
          context.closePath();
          context.beginPath();
          context.arc(p[i].x,p[i].y,r,0,tau);
          context.strokeStyle='rgba(255,255,255,'+(50/(n*f))+')';
          context.stroke();
          context.closePath();
        }
      }
      
      // Append histogram data
      if (analyse) {
        if (hist[0][p[i].N]!==undefined) hist[0][p[i].N]++; else hist[0][p[i].N]=1;
        if (hist[1][p[i].L]!==undefined) hist[1][p[i].L]++; else hist[1][p[i].L]=1;
        if (hist[2][p[i].R]!==undefined) hist[2][p[i].R]++; else hist[2][p[i].R]=1;
      }
          
      // Apply changes in orientation
      let delta_phi=a+(b*p[i].N*Math.sign(p[i].R-p[i].L));  // delta_phi = alpha + beta × N × sign(R - L)
      p[i].phi=scope(p[i].phi+delta_phi,tau);               // Turn right delta_phi
      
      // Move forward v
      p[i].x+=(v*p[i].pCos);
      p[i].y+=(v*p[i].pSin);
      
      // Wrap screen edges, Pac-Man style
      p[i].x=scope(p[i].x,vw);
      p[i].y=scope(p[i].y,vh);
    }
    
    // Analytics
    if (analyse) {
      // framesPerSecond();
      // orbitals();
      histogram();
    }
  }