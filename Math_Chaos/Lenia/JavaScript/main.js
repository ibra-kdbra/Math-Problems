"use strict";

//*** util
const PRECISION = 1000000;
const EPSILON = 1/PRECISION;  // machine epsilon
const π = Math.PI;

//*** param
var LOG_SIZE = 0;
var F_SIZE = 1 << LOG_SIZE;  // field size in power of 2
var NS = 13;  // R = space resolution = kernel radius (neighborhood size) in cells
var TS = 10;  // T = time resolution = time step in steps
const TS_LIST = [1,2,4,5,10,20,50,100,200,500,1000];  // multiples of 2 and 5
			//[1,2,4,5,8,10,16,20,25,32,40,50,64,80,100,125,160,200,250,320,400,500,625,800,1000];
const B_SIMP = ["0", "1/12", "1/6", "1/4", "1/3", "5/12", "1/2", "7/12", "2/3", "3/4", "5/6", "11/12", "1", 
				"13/12", "7/6", "5/4", "4/3", "17/12", "3/2", "19/12", "5/3", "7/4", "11/6", "23/12", "2"];  // numerator & denominator of simplified fraction
const B_DIV = 12;  // denominator
var delta_c = 0.14;
var delta_w = 0.015;
var delta_adj = 0.001;
var kernel_A = 4;
var kernel_B = new Array(4).fill(0); kernel_B[0] = B_DIV;
var kernel_E = new Array(4).fill(0);
var gen = 0;
var time = 0;
var runGen = null;
var isLimitValue = true;

//*** running
const FPS_FREQ = 20;
var isRunning = true;
var isRunOnce = false;
var lastLoop = null;
var fps = 0;

//*** control
const ZIP_HEADER = "(zip)";
const ZIP2_HEADER = "(zip2)";
const ZIP_START = 192;
const FILL_HEADER = "(fill)";
const MISSING_MARK = "[x]";
const RAND_BORDER = 45/256;
var optAnimalX = 0;
var optCenter = 0;  const Center_OFF = 0, Center_POS = 1, Center_AM = 2, Center_TH = 3;
var optRepeat = 0;  const Repeat_OFF = 0, Repeat_ADD = 1, Repeat_MIRROR = 2, Repeat_COLLIDE = 3;
var lastAdd = 0;  const Add_RAND = 0, Add_PASTE = 1, Add_SOL = 2;
var lastMirror = 0; const Flip_H = 1, Flip_V = 2, Flip_HV = 3, Mirror_H = 4, Mirror_X = 5, Mirror_D = 6;
var collideStage = 0;  const Collide_1ST = 0, Collide_2ND = 1, Collide_ADJ = 2, Collide_ERR = 3;
var optCopy = 0;  const Copy_FIELD_S = 0, Copy_FIELD = 1, Copy_DELTA = 2, Copy_NEIGH = 3, Copy_SEED = 4; 
var optFormat = 0;  const Format_ZIP = 0, Format_TABLE = 1, Format_TEXT = 2, Format_MATLAB = 3, Format_ZIP2 = -2;
var optAid = 1;  const Aid_OFF = 0, Aid_RULER = 1, Aid_TRACE = 2;
var optCalc = 1;  const Calc_ALL = 0, Calc_STAT = 1, Calc_NONE = 2;
var optValue = 1;  const Value_VAL = 0, Value_AVG = 1, Value_PERIOD = 2;
var isShowArrow = false;
var transA = null;
var transS = null;
var lastStatTR = null;
var lastSeed = 0;
var lastFill = false;
var lastAnimalID = null;
var lastAnimalItem = null;
var lastRuleSt = "";
var lastCellSt = "";
var cubeList = [];
var lastZoom = false;
var lastZoomName = "";
var collideCellSt = "";
var collideZoom = false;
var collideNS = 0;
var accumX = 0;
var accumY = 0;
var accumA = 0;
var accumF = 0;
var isMac = false;

//*** gif
var gif = new GIFEncoder();
var optGIF = 0;  const GIF_OFF = 0, GIF_ON = 1;

//*** display
var LOG_PIXEL = 0;
var PIXEL = 1 << LOG_PIXEL;
var P_SIZE = F_SIZE * PIXEL;
var optWindows = 1;  const Windows_FIELD = 0, Windows_CALC = 1, Windows_STAT = 2;
var optPanel = 1;  const Panel_CTRL = 0, Panel_PLOT = 1, Panel_CALC = 2, Panel_CUBE = 3, Panel_STAT = 4, Panel_INFO = 5;
var isKernelDrawn = false;
var optColor = 1;  const GREY = 0, BLUE = 1, GREEN = 2, RAINBOW = 3; const LMARK = 11, MMARK = 12, DMARK = 13;
const NEG_THRES = -0.8; //-0.8
const NEG_COLORS = 4; //4
const COLOR_ZERO = [15,15,15];
const COLOR_NEG = [15,14,15];
const COLOR_DOT = [12,12,13];
const COLORS = [
[ [13,13,13], [12,12,12], [11,11,11], [10,10,10], [9,9,9], [8,8,8], [7,7,7], [6,6,6], [5,5,5], [4,4,4], [1,1,1], /**/ [0,15,15], [0,9,15], [7,11,15] ],  // B/W
[ [9,15,15], [6,13,15], [3,11,15], [0,9,15], [3,8,15], [6,7,15], [9,6,15], [11,4,15], [13,2,15], [15,0,15], [11,0,5], /**/ [15,15,0], [15,7,0], [15,11,7] ],  // blue
[ [9,15,9], [7,13,7], [5,11,5], [3,10,3], [5,10,3], [8,10,1], [11,8,0], [13,6,3], [15,4,6], [15,2,9], [9,0,7], /**/ [0,15,15], [0,9,15], [7,11,15] ] ];  // green
//[ [0,15,15], [0,7,15], [0,0,15], [0,7,7], [0,15,0], [7,15,0], [15,15,0], [15,7,0], [15,0,0], [15,0,7], [15,0,15], /**/ [15,15,0], [15,7,0], [15,11,7] ] ];  // rainbow

//*** objects
var field = null;
var fieldOld = null;
var fieldTx = null;
var fieldIm = null;
//var fieldAm = null;
//var fieldPh = null;
var kernel = null;
var kernelRe = null;
var kernelIm = null;
var neighRe = null;
var neighIm = null;
var delta = null;
var neigh = null;
//var neighMax = 0;
var neighLevel = 45;
var neighDiv = 60;
var plottingArr = null;
var plotStat = [];

var canvas1 = null;
var canvas2 = null;
var canvas3 = null;
var canvas4 = null;
var canvasT = null;
var mouseX = -1, mouseY = -1;

//*** keyboard
const ENTER = 13, SPACE = 32, LEFT = 37, RIGHT = 39, UP = 38, DOWN = 40, INS = 45, DEL = 46, HOME = 36, END = 35, PGUP = 33, PGDN = 34,
	PAD = 96, ADD = 107, SUB = 109, MUL = 106, DIV = 111, DEC = 110,
	GRAVE = 192, DASH = 189, EQUAL = 187, BKSPACE = 8, LBRA = 219, RBRA = 221, BKSLASH = 220, SEMICOLON = 186, QUOTE = 222, COMMA = 188, DOT = 190, SLASH = 191;
const ALT = "~~";
const INT = 3000;  // internal commands
var actionKey = null;
var actionShift = false;

//*** info
var mX, mY; // centroid
var gX, gY; // growth centroid
var axisA, axisB, th, ec, cp; // semi-major & semi-minor axes, orientation, eccentricity
var vol, volg, moi, rog; // volume, growth, mass moment of inertia, radius of gyration
var m00, m10, m01, m11, m20, m02, m30, m03, m21, m12, m22, m31, m13, m40, m04; // raw moments, mass = m00
var g00, g10, g01; // raw moments for growth
var μ11, μ20, μ02, μ30, μ03, μ21, μ12, μ22, μ31, μ13, μ40, μ04; // central moments (translation invariant)
var η11, η20, η02, η30, η03, η21, η12, η22, η31, η13, η40, η04; // normalized central moments (translation & scale invariant)
var φ1, φ2, φ3, φ4, φ5, φ6, φ7, φ8, φ9, φ10, φ11, φ12, φ13; // Hu's & Flusser's moment invariants (translation, scale & rotation invariant; φ2,φ3 not useful, φ7 skew invariant)
var oldth=0, dth=0, th180=0;
//var ths1=0, ths2=0, thc=0, thw=0, tht=0;
var shiftX, shiftY;
var arrowX, arrowY;
var oldmX, oldmY, dM, aM, oldaM, daM, dMG, aMG, oldaMG, daMG;
var status = "";
var debugSt = "";
var debug = [];

//*** stat
const STAT_W = 300; // = 33 for Paraptera arcus saliens ignis
const STAT_H = 30;
const STAT_S = 1;
const PERIOD_PRE = 50;//3;//10;
const PERIOD_W = 1000;//300 //1 << 8;//<< 9;
const PERIOD_COUNT_MIN = 2;
const PERIOD_ALL_COUNT_MIN = 20;
const PERIOD_SURE_LEVEL = 1.5;
var statVals = [];
var statIndex = 0;
var stat = null;
var period0 = null;

const STAT_N = 20;
const STAT_SPLIT = 11;
const sNORM = 0, sLOG = 1, sANGLE = 2;
const STAT_INFO = [ // [name, arcNo, type, max]
["m", 19, sLOG, 2], ["g", 18, sLOG, 2], ["V", 17, sLOG, 2], ["V_g", 16, sLOG, 2], ["ρ", 15, sNORM, 1], ["ρ_g", 14, sNORM, 1], //["k", 10, sNORM, 25], 
["s", 13, sNORM, 0.5], ["d_g", 12, sNORM, 0.5], ["ω", 11, sANGLE, 100], ["ω_g", 10, sANGLE, 100], ["ω_θ", 9, sANGLE, 100], 
["I", 0, sLOG, -2], ["γ", 1, sLOG, -10], ["φ_5", 2, sLOG, -20], ["φ_6", 3, sLOG, -20], ["φ_7", 4, sLOG, -40], 
["κ", 5, sNORM, 2], ["ψ_8", 6, sLOG, -20], ["ψ_9", 7, sLOG, -20], ["ψ_10", 8, sLOG, -40], //["φ_2", 8, sLOG, -10], ["φ_3", 9, sLOG, -10], ["φ_8", 8, sLOG, -20], ["ψ_11", 9, sLOG, -40], 
];
function GetStatVals() {
	return [m00/NS/NS, g00/NS/NS, vol/NS/NS, volg/NS/NS, m00/vol, g00/volg,  // rog/NS, 
		dM/NS*TS, dMG/NS, dM/NS*TS<0.01 ? 0 : daM*TS, dMG/NS<0.01 ? 0 : daMG*TS, dth*TS,
		φ1, φ4, φ5, φ6, φ7,
		φ9, φ10, φ11, φ12];  // φ2, φ3, φ7, φ8, φ13
}

//*** main
function InitAllArrays() {
	field = null;      field = InitArray();
	fieldOld = null;   fieldOld = InitArray();
	fieldTx = null;    fieldTx = InitArray();
	fieldIm = null;    fieldIm = InitArray();
	//fieldAm = null;  //fieldAm = InitArray();
	//fieldPh = null;  //fieldPh = InitArray();
	kernel = null;     kernel = InitArray();
	kernelRe = null;   kernelRe = InitArray();
	kernelIm = null;   kernelIm = InitArray();
	neighRe = null;    neighRe = InitArray();
	neighIm = null;    neighIm = InitArray();
	delta = null;      delta = InitArray();
	neigh = null;      neigh = InitArray();

	PlotSurface(null);
}

function InitAllCanvas() {
	canvas1 = InitCanvas("canvas1");
	canvas2 = InitCanvas("canvas2");
	canvas3 = InitCanvas("canvas3");
	canvas4 = InitCanvas("canvas4");
	canvasT = InitCanvas("canvasT");
}

function ResizeField(s, p, isZoom) {
	var isFirst = (LOG_SIZE == 0);
	var isSizeChanged = (LOG_SIZE != s);
	var d = s - LOG_SIZE;
	var d2 = Round(Math.pow(2, d));

	if (NS != 2 && isSizeChanged && !isFirst) {
		if (isZoom && d<0) TransformField(0, 0, d2, 0, 0); 
		lastCellSt = CopyField(field, F_SIZE, F_SIZE, Format_ZIP, true);
	}

	LOG_SIZE = s;
	F_SIZE = 1 << LOG_SIZE;
	LOG_PIXEL = p;
	PIXEL = (LOG_PIXEL>=0) ? 1 << LOG_PIXEL : Round(Math.pow(2, LOG_PIXEL));
	P_SIZE = Math.round(F_SIZE * PIXEL);
	if (isSizeChanged) optAnimalX = Bound(LOG_SIZE - 7, 0, 3);
	//if (isSizeChanged) NS = Math.round(13/256*F_SIZE);

	if (isSizeChanged) InitAllArrays();
	InitAllCanvas();
	CalcKernel();
	if (isSizeChanged) ResetCounters();

	if (NS != 2 && isSizeChanged && !isFirst) {
		AddCells(lastCellSt, false, false);
		if (isZoom && d>0) TransformField(0, 0, d2, 0, 0); 
	}
}

function ResizeAll(s, isZoom) {
	/*
	FS		PS		NS		AX
	5=32	3=8		13/2	0
	6=64	2=4		13		0
	7=128	1=2		13x2	1
	8=256	0=1		13x4	2
	9=512	-1=0.5	13x8	3
	10=1024	-2=0.125
	*/
	var p = Bound(8-s, -2, 3);
	ResizeField(s, p, isZoom);
}

function OnCopy(e, format) {
	/*
	//*** bulk convert all animals to zip
	var st = "";
	for (var n=0; n<animalArr.length; n++) {
		var a = animalArr[n];
		if (a.length == 4) {
			var s = a[3].split(";");
			var ruleSt = s.slice(0, -1).join(";");
			var cellSt = s[3].split("=")[1];
			cellSt = TextToZip(cellSt, Format_ZIP, true);
			a[3] = ruleSt + ";cells=" + cellSt;
			st += "[\"" + a[0] + "\", \"" + a[1] + "\", \"" + a[2] + "\", \"" + a[3] + "\"],\r\n";
		} else if (a.length == 3) {
			st += "[\"" + a[0] + "\", \"" + a[1] + "\", \"" + a[2] + "\"],\r\n";
		} else if (a.length == 1) {
			st += "[\"" + a[0] + "\"],\r\n";
		}
	}
	CopyToClipboard(st);
	return false;
	*/

	if (format==null) format = optFormat;

	if (optPanel==Panel_CUBE) {
		var foundList = [];
		var missingList = [];
		for (var id in cubeList) {
			var st = cubeList[id];
			if (st == MISSING_MARK)
				missingList.push(id);
			else
				foundList.push(st);
		}
		var st2 = foundList.join("\n") + "\n";
		if (missingList.length > 0) st2 = MISSING_MARK + missingList.join(",") + "\n" + st2;
		CopyToClipboard(st2);
		return false;
	} else {
		var arr = optCopy==Copy_FIELD_S ? field : optCopy==Copy_FIELD ? field : optCopy==Copy_DELTA ? delta : optCopy==Copy_NEIGH ? neigh : null;
		var name = optCopy==Copy_FIELD_S ? "cells" : optCopy==Copy_FIELD ? "cells" : optCopy==Copy_DELTA ? "delta" : optCopy==Copy_NEIGH ? "neigh" : optCopy==Copy_SEED ? "seed" : "??";
		var shrink = (optCopy==Copy_FIELD_S);
		switch (optCopy) {
			case Copy_FIELD_S: case Copy_FIELD: case Copy_DELTA: case Copy_NEIGH:
				var cellSt = CopyField(arr, F_SIZE, F_SIZE, format, shrink);
				/*
				if (optCopy==Copy_FIELD) {
					lastCellSt = cellSt;
					lastZoom = false;
					lastAdd = Add_PASTE;
				}
				*/
				var st = (format==Format_MATLAB)
					? GetMatlabRuleSt() + ";" + name + "=[" + cellSt + "];"
					: GetRuleSt() + ";" + name + "=" + (format==Format_TABLE?"\n":"") + cellSt;
				CopyToClipboard(st);
				return false;
			case Copy_SEED:
				var st = GetRuleSt() + ";" + name + "=" + (lastFill ? FILL_HEADER : "") + lastSeed;
				CopyToClipboard(st);
				return false;
			default:
				return true;
		}
	}
}

function OnPaste(e) {
	var st = e.clipboardData.getData("text/plain");
	st = st.replace(/\r\n/g, "\n");
	if (st.includes("\nR=") || st.includes("\n"+MISSING_MARK)) { // multi-line for beta cube
		var s = st.split("\n");
		var st0 = "";
		cubeList = [];
		for (var i=0; i<s.length; i++) {
			if (s[i].startsWith("R=")) {
				var r = ParseRule(s[i]);
				if (st0=="") {
					st0 = s[i];
					SetRuleAndAddCells(st0, false, false);
					status = lastZoomName = "Pasted cube<br>Rule: " + lastRuleSt.replace("<","&lt;").replace(">","&gt;");
					DrawAllPanels();
					lastAdd = Add_PASTE;
				}
				var id = GetCubeID(r.kernel_B);
				cubeList[id] = s[i];
			} else if (s[i].startsWith(MISSING_MARK)) {
				var missingList = s[i].substring(MISSING_MARK.length).split(",");
				for (var j=0; j<missingList.length; j++) {
					var id = missingList[j];
					if (!cubeList[id]) cubeList[id] = MISSING_MARK;
				}
			}
		}
		SwitchPanel(Panel_CUBE);
		RefreshCube();
		PositionCube();
	} else if (st.includes(";cells=")) {
		if (st.includes("\t")) { // from table
			st = st.replace(/\t/g, ",").replace(/\n/g, "/").replace(/cells=,*\//, "cells=");
			CopyToClipboard(st);
		}
		SetRuleAndAddCells(st, false, false);
		status = lastZoomName = "Pasted field<br>Rule: " + lastRuleSt.replace("<","&lt;").replace(">","&gt;");
		DrawAllPanels();
		lastAdd = Add_PASTE;
	} else if (st.includes("\t")) { // from table, without header
		st = st.replace(/\t/g, ",").replace(/\n/g, "/");
		AddCells(st, false, false);
		status = lastZoomName = "Pasted field";
		DrawAllPanels();
		lastAdd = Add_PASTE;
	} else if (st.includes(";seed=")) {
		var s = st.split(";");
		var seedSt = s[3].split("=")[1];
		lastFill = seedSt.startsWith(FILL_HEADER);
		if (lastFill) seedSt = seedSt.substring(FILL_HEADER.length);
		var ruleSt = s.slice(0, -1).join(";");
		SetRule(ruleSt);
		ClearField(0);
		UseSeed(seedSt);
		AddRandom(lastFill?0.5:1, lastFill);
		status = "Random" + (lastFill ? " filled" : " squares") + "<br>Pasted seed: " + lastSeed;
		lastAdd = Add_RAND;
	}
	return false;
}

function OnKeyDown(e) {
	actionKey = window.event ? e.keyCode : e.which;
	actionShift = e.shiftKey;

	var ch = String.fromCharCode(actionKey);
	var isAlpha = (ch>="A" && ch<="Z");
	var isNumeric = (ch>="0" && ch<="9");
	if ( (isAlpha || isNumeric)	&& !(e.ctrlKey||e.metaKey) ) {
		actionKey = ch;
	}
	if (e.altKey) actionKey = ALT+actionKey;

	if ( (!isMac && e.ctrlKey && ch=="C") || (!isMac && e.ctrlKey && actionKey==INS) || (isMac && e.metaKey && ch=="C") ) {
		if (!(window.getSelection && window.getSelection().toString()) && !(document.selection && document.selection.createRange().text)) { // copy text if HTML text selected
			OnCopy(e, optFormat);
			e.returnValue = false;
			e.cancel = true;
		}
	} else if ( (!isMac && e.ctrlKey && ch=="X") || (!isMac && e.shiftKey && actionKey==DEL) || (isMac && e.metaKey && ch=="X") ) {
		OnCopy(e, Format_TABLE);
		e.returnValue = false;
		e.cancel = true;
	//} else if ( (!isMac && e.ctrlKey && ch=="V") || (!isMac && e.shiftKey && actionKey==INS) || (isMac && e.metaKey && ch=="V") ) {
	//	OnPaste(e);
	//	e.returnValue = false;
	//	e.cancel = true;
	} else if ((isAlpha && !(e.ctrlKey||e.metaKey)) || isNumeric || [SPACE, ENTER, DASH, SUB, UP, DOWN, LEFT, RIGHT, PGUP, PGDN, HOME, END].includes(actionKey)) {
		e.returnValue = false;
		e.cancel = true;
	}
}

function SelectStatObj(obj) {
	SelectStat(obj.parentNode.parentNode.id.replace("s_tr", ""));
}

function SelectStat(id) {
	var t0 = document.getElementById("s_tr");
	var p0 = t0.parentNode;
	var t2 = document.getElementById("s_tr"+id);
	var p2 = t2.parentNode;
	if (lastStatTR != null) {
		var t1 = document.getElementById("s_tr"+lastStatTR);
		p0 = t1.parentNode;
		t1.title = "";
		p2.insertBefore(t1, t0);
	}
	t2.title = "Click a panel below to place on top";
	p2.insertBefore(t0, t2);
	p0.appendChild(t2);
	lastStatTR = id;
}

function BodyOnload() {
	isMac = window.navigator.platform.startsWith("Mac");
	InitAllStat();
	PopulateAnimalList();
	PopulateCatalogue();
	PopulateCube();
	SwitchPanel(Panel_CTRL);  // Panel_INFO
	SelectStat("0");
	DrawSwatches();

	DefaultGroups();
	PresetAnimal({fsize:7, animalCode:DEFAULT_ANIMAL});
	TransformField(0, 0, 2, 0, 0);

	//*** works in Windows, but OS X only allow running cut command from semi-trusted events (e.g. onkeydown but not oncopy) 
	//*** run OnCopy inside OnKeyDown instead
	//document.body.addEventListener("copy", OnCopy);
	document.body.addEventListener("paste", OnPaste);
	document.body.addEventListener("keydown", OnKeyDown);
	canvas1.can.addEventListener("mousemove", function (e) { mouseX = (e.clientX-canvas1.left)/PIXEL; mouseY = (e.clientY-canvas1.top)/PIXEL; if (!isRunning) DrawAllPanels(); });
	canvas2.can.addEventListener("mousemove", function (e) { mouseX = (e.clientX-canvas2.left)/PIXEL; mouseY = (e.clientY-canvas2.top)/PIXEL; if (!isRunning) DrawAllPanels(); });
	canvas3.can.addEventListener("mousemove", function (e) { mouseX = (e.clientX-canvas3.left)/PIXEL; mouseY = (e.clientY-canvas3.top)/PIXEL; if (!isRunning) DrawAllPanels(); });
	var OnMouseOut = function (e) { mouseX = mouseY = -1; if (!isRunning) DrawAllPanels(); };
	canvas1.can.addEventListener("mouseout", OnMouseOut);
	canvas2.can.addEventListener("mouseout", OnMouseOut);
	canvas3.can.addEventListener("mouseout", OnMouseOut);

	/*
	var request = new XMLHttpRequest();
	request.onreadystatechange = function() {
		if (this.readyState == 4) {
			alert("status[" + this.status + "] length[" + this.responseText.length + "]\n[" + this.responseText + "]");
		}
	};
	request.open("GET", "CS/dat/niche/Niche5/X5_cell-r13bc4g035,0048t01.js");
	request.setRequestHeader('Range', 'bytes=100-200');
	request.send();
	*/

	//setInterval(Step, 1);
	Step();
}

function Step() {
	if (actionKey != null)
		DoAction(actionKey, actionShift);
	actionKey = null;
	actionShift = false;

	if (isRunning || isRunOnce) {
		CalcFPS();
		NextGen();
		CalcMoments();
		CalcPeriods();
		CalcStats();
		DrawStatsBar();
		DrawAllPanels();
		ShowCounters();
		if (optGIF==GIF_ON)
			gif.addFrame(canvas1.ctx);
		if (runGen==0) {
			runGen = null;
			isRunning = false;
		}
	}
	isRunOnce = false;
	window.requestAnimationFrame(Step);
	//window.setTimeout(Step, 0);
}

function CalcFPS() {
	if (gen == 0) {
		lastLoop = Date.now();
		fps = 0;
	} else if (gen % FPS_FREQ == 0) {
		var thisLoop = Date.now();
		var s = (thisLoop - lastLoop) / 1000;
		fps = FPS_FREQ / s;
		lastLoop = thisLoop;
	}
}

//*** util
function Round(x) { return Math.round(x * PRECISION) / PRECISION; }
function Bound(x, min, max) { var v = Math.round(x * PRECISION) / PRECISION; return v<min ? min : v>max ? max : v; }
function Mod(x, n) { return ((x % n) + n) % n; }
function IsZero(x) { return (x < EPSILON && x > -EPSILON); }
function IsEq(x, y) { return (x-y < EPSILON && x-y > -EPSILON); }
function IsPos(x) { return (x > EPSILON); }
function IsNeg(x) { return (x < -EPSILON); }

function Num(x, n) {
	//return x<=-10||x>=10 ? x.toFixed(2) : x>=0 ? " "+("0000"+x.toFixed(2)).slice(-4) : "-"+("0000"+(-x).toFixed(2)).slice(-4);
	return n!=null ? x.toFixed(n) : x.toFixed(2);
}

function Num2(x) {
	return x==0 ? "-" : (x<0?"_":"^") + Math.log(Math.abs(x)).toFixed(1);
	//return (x>0?"+":"") + x.toExponential(2).replace("e","<sup>") + "</sup>";
}

function SortedKeys(list) {
	return Object.keys(list).sort(function(a,b){return list[b]-list[a]});
}

function RGBSt(S) {
	return "rgb("+(S[0]*0x11)+","+(S[1]*0x11)+","+(S[2]*0x11)+")";
}

/*
function LoadGif(img) {
	img.src = img.src.replace(".png", ".gif");
}

function UnloadGif(img) {
	img.src = img.src.replace(".gif", ".png");
}
*/

//**random
function GetNewSeed() { return (new Date).getTime(); }
function UseSeed(seed) { lastSeed = seed; init_genrand(lastSeed); }
function Random() { return genrand_real2(); }
function RandomInt(min, max) { return Math.floor(Random() * (max + 1 - min) + min); }
function RandomFloat(min, max) { return Random() * (max - min) + min; }

//*** calc
function CalcKernel() {
	var weight = 0.0;
	for (var i=0; i<F_SIZE; i++) {
		for (var j=0; j<F_SIZE; j++) {
			var ii = ((i + F_SIZE/2) % F_SIZE) - F_SIZE/2;
			var jj = ((j + F_SIZE/2) % F_SIZE) - F_SIZE/2;
			var r = Math.sqrt(ii*ii + jj*jj) / NS;
			var v = KernelFunc(r);
			weight += v;
			kernelRe[i][j] = v;
			ii = F_SIZE - ((i + F_SIZE/2) % F_SIZE) - 1;
			jj = ((j + F_SIZE/2) % F_SIZE);
			kernel[ii][jj] = v;
		}
	}

	FFT2D(1, kernelRe, kernelIm);
	for (var i=0; i<F_SIZE; i++) {
		for (var j=0; j<F_SIZE; j++) {
			kernelRe[i][j] /= weight;
			kernelIm[i][j] /= weight;
		}
	}
	isKernelDrawn = false;
}

function NextGen(isUpdate) {
	if (isUpdate==null) isUpdate = true;
	for (var i=0; i<F_SIZE; i++)
		for (var j=0; j<F_SIZE; j++)
			fieldOld[i][j] = field[i][j];
	for (var i=0; i<F_SIZE; i++)
		fieldIm[i].fill(0);

	// f * g = F-1( F(f) dot F(g) )
	FFT2D(1, field, fieldIm);
	MatrixDot(field, fieldIm, kernelRe, kernelIm, neighRe, neighIm);
	FFT2D(-1, neighRe, neighIm);

	shiftX = (optCenter==Center_OFF) ? 0 : Math.floor(mX-F_SIZE/2);
	shiftY = (optCenter==Center_OFF) ? 0 : Math.floor(mY-F_SIZE/2);

	var vmax = isLimitValue ? 1 : Number.POSITIVE_INFINITY;
	//neighMax = 0;
	for (var i=0; i<F_SIZE; i++) {
		for (var j=0; j<F_SIZE; j++) {
			var ii = (optCenter==Center_OFF) ? i : Mod(i-shiftY, F_SIZE);
			var jj = (optCenter==Center_OFF) ? j : Mod(j-shiftX, F_SIZE);
			/*
			if (optWindows==Windows_STAT) {
				var re = field[ii][jj];
				var im = fieldIm[ii][jj];
				fieldAm[ii][jj] = Math.sqrt(re*re + im*im);
				//fieldPh[ii][jj] = Math.atan2(im, re) / π;
			}
			*/

			var n = neigh[ii][jj] = neighRe[i][j];
			var d = delta[ii][jj] = DeltaFunc(n);
			var v = fieldOld[i][j] + d / TS;
			//if (v<0) v = 0; else if (isLimitValue && v>1) v = 1;
			if (v<0) v = 0; else if (v>vmax) v = vmax;
			if (isUpdate) field[ii][jj] = v; else field[i][j] = fieldOld[i][j];
			//if (n > neighMax) neighMax = n;
		}
	}
	if (isUpdate) {
		gen++;
		if (runGen != null) runGen--;
		time = Round(time + Round(1/TS));
		transA = transS = null;
	}
}

function TwoPoints(x1, y1, x2, y2, olda) {
	var dX = x1 - x2;
	var dY = y1 - y2;
	var d = Math.sqrt(dX*dX + dY*dY);
	var a = IsZero(d) ? 0 : Math.atan2(dY, dX) / π * 180;
	var da = !olda ? 0 : a - olda;
	da = (da + 540) % 360 - 180;
	//da = (da>180) ? 360-da : (da<-180) ? 360+da : da;
	//if (a*olda < 0 && da > 180) da = 360 - da;
	return {distance:d, angle:a, angleDiff:da};
}

function FindMax(dict, prev) {
	var max = 0, key = null;
	for (var k in dict) {
		var v = dict[k];
		if (v > max && (prev == null || k != prev)) {
			max = v;
			key = k;
		}
	}
	return {max:max, key:key};
}

function CalcMoments(isCalc) {
	vol = volg = m00 = m10 = m01 = m11 = m20 = m02 = m30 = m03 = m21 = m12 = m22 = m31 = m13 = m40 = m04 = 0; // m00 = mass
	g00 = g10 = g01 = 0;

	if (!isCalc && optCalc==Calc_NONE)
		return;

	oldmX = mX - shiftX;
	oldmY = mY - shiftY;
	oldaM = aM;
	oldaMG = aMG;
	for (var i=0; i<F_SIZE; i++) {
		for (var j=0; j<F_SIZE; j++) {
			var y = i<oldmY-F_SIZE/2 ? i+F_SIZE : i>oldmY+F_SIZE/2 ? i-F_SIZE : i;
			var x = j<oldmX-F_SIZE/2 ? j+F_SIZE : j>oldmX+F_SIZE/2 ? j-F_SIZE : j;
			var v = field[i][j];
			var g = Math.max(0, delta[i][j]);
			if (IsPos(v)) vol++;
			if (IsPos(g)) volg++;

			var vx = v*x, vy = v*y;
			var vxx = vx*x, vyy = vy*y, vxy = vx*y;
			var vxxx = vxx*x, vyyy = vyy*y, vxxy = vxx*y, vxyy = vyy*x;
			var gx = g*x, gy = g*y;

			m00 += v;
			m10 += vx;  m01 += vy;
			m11 += vxy;  m20 += vxx;  m02 += vyy;
			m30 += vxxx;  m03 += vyyy;  m21 += vxxy;  m12 += vxyy;
			m22 += vxxy*y;  m31 += vxxx*y;  m13 += vyyy*x;  m40 += vxxx*x;  m04 += vyyy*y;
			g00 += g;
			g10 += gx;  g01 += gy;
		}
	}
	mX = m00==0 ? F_SIZE/2 : m10/m00;
	mY = m00==0 ? F_SIZE/2 : m01/m00;
	gX = g00==0 ? F_SIZE/2 : g10/g00;
	gY = g00==0 ? F_SIZE/2 : g01/g00;

	var X2 = mX*mX, X3 = X2*mX;
	var Y2 = mY*mY, Y3 = Y2*mY;
	var XY = mX*mY;
	//moi = m20 + m02 - 2*mX*m10 - 2*mY*m01 + (X2+Y2)*m00;  // I = sum[vr2] = sum[v(x-mX)2+v(y-mY)2]
	//rog = Math.sqrt(moi / m00);
	//μ00 = m00 = mass
	//μ01 = 0 (centroid now at center)
	//μ10 = 0
	μ11 = m11 - mX*m01; // or = m11 - mY*m10
	μ20 = m20 - mX*m10;
	μ02 = m02 - mY*m01;
	μ30 = m30 - 3*mX*m20 + 2*X2*m10;
	μ03 = m03 - 3*mY*m02 + 2*Y2*m01;
	μ21 = m21 - 2*mX*m11 - mY*m20 + 2*X2*m01;
	μ12 = m12 - 2*mY*m11 - mX*m02 + 2*Y2*m10;
	μ22 = m22 - 2*mY*m21 + Y2*m20 - 2*mX*m12 + 4*XY*m11 - 2*mX*Y2*m10 + X2*m02 - 2*X2*mY*m01 + X2*Y2*m00;
	μ31 = m31 - mY*m30 + 3*mX*(mY*m20-m21) + 3*X2*(m11-mY*m10) + X3*(mY*m00-m01);
	μ13 = m13 - mX*m03 + 3*mY*(mX*m02-m12) + 3*Y2*(m11-mX*m01) + Y3*(mX*m00-m10);
	μ40 = m40 - 4*mX*m30 + 6*X2*m20 - 4*X3*m10 + X2*X2*m00;
	μ04 = m04 - 4*mY*m03 + 6*Y2*m02 - 4*Y3*m01 + Y2*Y2*m00;
	if (IsZero(m00)) {
		η11 = η20 = η02 = η30 = η03 = η21 = η12 = η22 = η31 = η13 = η40 = η04 = 0;
	} else {
		var m2 = m00*m00;
		var mA = m00*m00*Math.sqrt(m00);
		var m3 = m00*m00*m00;
		η11 = μ11/m2;  η20 = μ20/m2;  η02 = μ02/m2;
		η30 = μ30/mA;  η03 = μ03/mA;  η21 = μ21/mA;  η12 = μ12/mA;
		η22 = μ22/m3;  η31 = μ31/m3;  η13 = μ13/m3;  η40 = μ40/m3;  η04 = μ04/m3;
	}

	var Z = 2*η11;
	var A = η20 + η02;
	var B = η20 - η02;
	var C = η30 + η12;
	var D = η30 - η12;
	var E = η03 + η21;
	var F = η03 - η21;
	var G = η30 - 3*η12;
	var H = 3*η21 - η03;
	var Y = 2*η22;
	var I = η40 + η04;
	var J = η40 - η04;
	var K = η31 + η13;
	var L = η31 - η13;
	var CC = C*C;
	var EE = E*E;
	var CC_EE = CC - EE;
	var CC_EE3 = CC - 3*EE;
	var CC3_EE = 3*CC - EE;
	var CE = C*E;
	var DF = D*F;
	φ1 = A;
	φ2 = B*B + Z*Z;  //dependent on others
	φ3 = G*G + H*H;  //dependent on others
	φ4 = CC + EE;
	φ5 = G*C*CC_EE3 + H*E*CC3_EE;
	φ7 = H*C*CC_EE3 - G*E*CC3_EE;
	φ6 = B*CC_EE + 2*Z*CE;
	φ8 = Z*CC_EE/2 - B*CE;
	φ9 = I + Y;
	φ10 =    J*CC_EE + 4*L*DF;
	φ11 = -2*K*CC_EE - 2*J*DF;
	var M = I - 3*Y;
	var t1 = CC_EE*CC_EE - 4*CE*CE;
	var t2 = 4*CE*CC_EE;
	φ12 =  4*L*t2 + M*t1;
	φ13 = -4*L*t1 + M*t2;
	
	var t3 = φ1 / 2 * m00;
	var t4 = Math.sqrt(φ2) / 2 * m00;
	axisA = t3+t4;
	axisB = t3-t4;
	ec = Math.sqrt(1 - axisB/axisA);
	
	cp = m00 / (μ20 + μ02);

	th = Math.atan2(Z, B) / 2 /π*180;
	dth = 0;
	if (oldth > 30 && th < -30) {
		th180 = (th180 + 180) % 360;
		dth = 180;
	} else if (oldth < -30 && th > 30) {
		th180 = (th180 - 180) % 360;
		dth = -180;
	}
	dth += !oldth ? 0 : th - oldth;
	dth = (dth + 540) % 360 - 180;
	oldth = th;
	th += th180;

	var p2 = TwoPoints(mX, mY, oldmX, oldmY, oldaM);
	dM = p2.distance;  aM = p2.angle;  daM = p2.angleDiff;
	p2 = TwoPoints(mX, mY, gX, gY, oldaMG);
	dMG = p2.distance;  aMG = p2.angle;  daMG = p2.angleDiff;
	if (gen < 2)
		daM = daMG = 0;

	arrowX = mX;
	arrowY = mY;
	mX = Mod(mX, F_SIZE);
	mY = Mod(mY, F_SIZE);
}

function CalcStats() {
	if (optCalc==Calc_NONE)
		return;

	statVals = GetStatVals();
	statIndex = Mod(statIndex+1, STAT_W/STAT_S);

	for (var part=0; part<STAT_N; part++) {
		var info = STAT_INFO[part];
		var type = info[2], max = info[3];
		var s = stat[part];
		var v = statVals[part];
		//var x = ( type!=sLOG ? v : Math.log(Math.abs(v)) ) / max;
		var x = v;

		if (s.index < PERIOD_W-1) {
			s.index++;
			s.arr[s.index] = x;
		} else {
			s.arr.shift();
			s.arr.push(x);
		}
		if (isNaN(v)) v = statVals[part] = 0;
		s.avg = (gen>1) ? (s.avg * (gen-1) + v) / gen : v;
	}
}

function CalcPeriods() {
	if (optCalc!=Calc_ALL || gen < PERIOD_PRE)
		return;

	for (var part=0; part<STAT_N; part++) {
		var s = stat[part];
		if (s.index % 2 == 1) {
			// Autocorrelation
			var i = Round((s.index+1) / 2);
			var sd1 = 0, sd2 = 0;
			for (var k=0; k<i; k++) {
				var d = s.arr[k] - s.arr[k+i];
				sd1 += d*d;
			}
			sd1 /= i;
			/*
			if (type==sANGLE) {
				for (var k=0; k<i; k++) {
					var d = s.arr[k] + s.arr[k+i];
					sd2 += d*d;
				}
				sd2 /= i;
			}
			*/

			var MIN_PERIOD = Round(0.8 * TS);
			if (i >= MIN_PERIOD) {
				var p = null;
				if (sd1 > s.last1 && sd1 < s.max1 / 5) {
					p = (i==MIN_PERIOD) ? 0 : i-1;
				//} else if (type==sANGLE && sd2 > s.last2 && sd2 < s.max2 / 5) {
				//	p = (i==MIN_PERIOD) ? 0 : -(i-1);
				} else if (s.index >= PERIOD_W-1) {
					p = 0;
				}
				if (p != null) {
					s.period.curr = p;
					if (!s.period.arr[p]) s.period.arr[p] = 0;
					s.period.arr[p]++;
					if (p != 0) {
						var a = Math.abs(p);
						if (!period0.arr[a]) period0.arr[a] = 0;
						period0.arr[a]++; // += (a==0?1:a);
					}
					s.index = 0;
					s.arr[0] = x;
					s.max1 = 0;
					s.max2 = 0;
				}
			}
			s.last1 = sd1;
			if (sd1 > s.max1) s.max1 = sd1;
			//s.last2 = sd2;
			//if (sd2 > s.max2) s.max2 = sd2;
		}
	}

	for (var part=-1; part<STAT_N; part++) {
		var pp = part==-1 ? period0 : stat[part].period;
		var min = part==-1 ? PERIOD_ALL_COUNT_MIN : PERIOD_COUNT_MIN;
		var m1 = FindMax(pp.arr);
		var m2 = FindMax(pp.arr, m1.key);
		pp.star = pp.sure = null;
		if (m1.max > min && m1.max > m2.max) {
			pp.star = m1.key;
			pp.sure = (pp.star != 0) ? (m1.max >= m2.max * PERIOD_SURE_LEVEL) : null;
		}
	}

	/*
	debug = [];
	for (var part=-1; part<STAT_N; part++) {
		var st = part==-1 ? "[all] " : "[" + STAT_INFO[part][0].replace("_","<sub>") + "</sub>] ";
		var pp = part==-1 ? period0 : stat[part].period;
		var j = 1;
		SortedKeys(pp.arr).forEach(function(p, i) {
			if (j%12==0) st += "<br>"; j++;
			st += (p==pp.star&&pp.sure?"<u>":"") + "<b>" + Round(p/TS) + "</b></u>,<sub>" + pp.arr[p] + "</sub> ";
		});
		debug.push(st);
	}
	*/
}

//** maths
function FFT1D(dir, re1, im1) {
	/* Do the bit reversal */
	var S = re1.length, m = Round(Math.log2(S)), S2 = S >> 1, j1 = 0;
	for (var j=0; j<S-1; j++) {
		if (j < j1) {
			var tmp = re1[j]; re1[j] = re1[j1]; re1[j1] = tmp;
			tmp = im1[j]; im1[j] = im1[j1]; im1[j1] = tmp;
		}
		var j2 = S2;
		while (j2 <= j1) {
			j1 -= j2;
			j2 >>= 1;
		}
		j1 += j2;
	}
	
	/* Compute the FFT */
	var c1 = -1.0, c2 = 0.0, l2 = 1;
	for (var l=0; l<m; l++) {
		var l1 = l2;
		l2 <<= 1;
		var u1 = 1.0, u2 = 0.0;
		for (var i=0; i<l1; i++) {
			for (var j=i; j<S; j+=l2) {
				var j2 = j + l1;
				var t1 = u1 * re1[j2] - u2 * im1[j2];
				var t2 = u1 * im1[j2] + u2 * re1[j2];
				re1[j2] = re1[j] - t1;
				im1[j2] = im1[j] - t2;
				re1[j] += t1;
				im1[j] += t2;
			}
			var z = u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}
		c2 = Math.sqrt((1.0 - c1) / 2.0);
		if (dir == 1)
			c2 = -c2;
		c1 = Math.sqrt((1.0 + c1) / 2.0);
	}
	
	/* Scaling for forward transform */
	if (dir == -1) {
		var scale_f = 1.0 / S;		
		for (var j=0; j<S; j++) {
			re1[j] *= scale_f;
			im1[j] *= scale_f;
		}
	}
}

function FFT2D(dir, re2, im2) {
	var S = re2.length;
	for (var i=0; i<S; i++) {
		FFT1D(dir, re2[i], im2[i]);
	}
	for (var i=0; i<S; i++) {
		for (var j=0; j<i; j++) {
			var tmp = re2[i][j]; re2[i][j] = re2[j][i]; re2[j][i] = tmp;
		}
	}
	for (var i=0; i<S; i++) {
		for (var j=0; j<i; j++) {
			var tmp = im2[i][j]; im2[i][j] = im2[j][i]; im2[j][i] = tmp;
		}
	}
	for (var i=0; i<S; i++) {
		FFT1D(dir, re2[i], im2[i]);
	}
}

function MatrixDot(ar, ai, br, bi, cr, ci) {
	var S = ar.length;
	for (var i=0; i<S; i++) {
		var ar_i = ar[i], ai_i = ai[i];
		var br_i = br[i], bi_i = bi[i];
		var cr_i = cr[i], ci_i = ci[i];
		for (var j=0; j<S; j++) {
			var a = ar_i[j]; var b = ai_i[j];
			var c = br_i[j]; var d = bi_i[j];
			var t = a * (c + d);
			cr_i[j] = t - d*(a+b);
			ci_i[j] = t + c*(b-a);
		}
	}
}

//*** field
function InitArray() {
	var arr = [];
	for (var i=0; i<F_SIZE; i++)
		arr.push(new Array(F_SIZE).fill(0));
	return arr;
}

function ClearField(x) {
	for (var i=0; i<F_SIZE; i++) {
		for (var j=0; j<F_SIZE; j++) {
			field[i][j] = x;
		}
	}
	status = "";
	isShowArrow = false;
	ShowOptions();
	ResetCounters();

	NextGen(false);
	CalcMoments();
}

function AddRandom(intensity, isFill) {
	var border1 = Math.floor(RAND_BORDER * F_SIZE), border2 = F_SIZE - border1;
	if (isFill) {
		for (var i=border1; i<border2; i++) 
			for (var j=border1; j<border2; j++) 
				field[i][j] = RandomFloat(0, intensity);
	} else {
		var S = NS * 1.1;
		if (coreName[coreID] == "life") S = NS * 5;
		for (var k=0; k<20; k++) {
			var i0 = RandomInt(border1, border2 - S);
			var j0 = RandomInt(border1, border2 - S);
			var int0 = RandomFloat(0.3, intensity);
			for (var i=0; i<S; i++)
				for (var j=0; j<S; j++)
					field[i0+i][j0+j] = RandomFloat(0, int0);
		}
	}
	lastAdd = Add_RAND;
	lastFill = isFill;
	status = "Random" + (isFill ? " filled" : " squares") + "<br>Seed: " + lastSeed;
	isShowArrow = false;
	ShowOptions();
	ResetCounters();

	NextGen(false);
	CalcMoments();
}

function TransformField(dX, dY, dS, dA, flip) {
	//*** flip:1(horiz), 2(vert), 3(h+v), 4(mirror horiz), 5(mirror+flip horiz), 6(mirror diag)
	if (dS!=1) {
		NS = Math.round(Bound(NS * dS, 2, F_SIZE));
		CalcKernel();
	}

	if ((dA == 0 || transA == null) && (dS == 1 || transS == null)) {
		for (var i=0; i<F_SIZE; i++)
			for (var j=0; j<F_SIZE; j++)
				fieldTx[i][j] = field[i][j];
	}

	if (dA == 0) {
		transA = null;
	} else {
		if (transA == null) transA = 0;
		dA = transA = transA + dA;
	}
	if (dS == 1) {
		transS = null;
	} else {
		if (transS == null) transS = 1;
		transS = transS * dS;
		dS = Round(transS);
	}
	for (var i=0; i<F_SIZE; i++)
		field[i].fill(0);

	var sin = Math.sin(dA / 180 * π);
	var cos = Math.cos(dA / 180 * π);
	var S = (dS<1) ? Math.round(F_SIZE*dS) : F_SIZE;
	var D = (dS<1) ? Math.round(F_SIZE*(1-dS)/2) : 0;
	for (var i=D; i<S+D; i++) {
		for (var j=D; j<S+D; j++) {
			var ii = Math.round( (- (j-F_SIZE/2)*sin + (i-F_SIZE/2)*cos) / dS + F_SIZE/2 - dY);
			var jj = Math.round( (+ (j-F_SIZE/2)*cos + (i-F_SIZE/2)*sin) / dS + F_SIZE/2 - dX);
			ii = Mod(ii, F_SIZE);
			jj = Mod(jj, F_SIZE);
			if (flip==Flip_V||flip==Flip_HV||(flip==Mirror_X&&j>S/2)) ii = F_SIZE-1-ii;
			if (flip==Flip_H||flip==Flip_HV||(flip==Mirror_H&&j>S/2)||(flip==Mirror_X&&j>S/2)) jj = F_SIZE-1-jj;
			if (flip==Mirror_D&&ii+jj>F_SIZE) { var tmp = ii; ii = F_SIZE-1-jj; jj = F_SIZE-1-tmp; }
			field[i][j] = fieldTx[ii][jj];
		}
	}

	//var tmp = transA;
	//ResetCounters();
	//transA = tmp;
	if (flip==Mirror_H||flip==Mirror_X||flip==Mirror_D) lastMirror = flip;
	NextGen(false);
	CalcMoments();
}

function TransformFieldOrAdjust(dX, dY, dS, dA, flip) {
	var isColliderOn = optRepeat==Repeat_COLLIDE && collideStage==Collide_ADJ;
	var isMirrorOn = optRepeat==Repeat_MIRROR;
	if (isColliderOn || isMirrorOn) {
		accumX = Mod(accumX + dX, F_SIZE);
		accumY = Mod(accumY + dY, F_SIZE);
		accumA = Mod(accumA + dA, 360);
		accumF = accumF ^ flip;  // toggle x-flip or y-flip
	}
	if (isColliderOn) {
		ShowCollide();
	} else if (isMirrorOn) {
		AddCells(lastCellSt, lastZoom, false);
		TransformField(accumX, accumY, 1, accumA, accumF);
		TransformField(0, 0, 1, 0, lastMirror);
	} else {
		TransformField(dX, dY, dS, dA, flip);
	}
}

//*** param
function GetRuleSt() {
	var bList = (layerID==0) ? "" : [SimpFrac(kernel_B[0]), SimpFrac(kernel_B[1]), SimpFrac(kernel_B[2]), SimpFrac(kernel_B[3])].slice(0, layerID+1).join(",");
	var eList = (layerID==0) ? "" : [SimpFrac(kernel_E[0]), SimpFrac(kernel_E[1]), SimpFrac(kernel_E[2]), SimpFrac(kernel_E[3])].slice(0, layerID+1).join(",");
	var hasE = (kernel_E[0]>0 || kernel_E[1]>0 || kernel_E[2]>0 || kernel_E[3]>0);
	return "R=" + NS + ";k=" + coreName[coreID] + "("+bList+")" + (hasE?"("+eList+")":"") + ";d=" + deltaName[deltaID] + "(" + delta_c + "," + delta_w + ")*" + Round(1/TS) + (isLimitValue?"":"+");
}

function GetMatlabRuleSt() {
	var bList = (layerID==0) ? "1" : [SimpFrac(kernel_B[0]), SimpFrac(kernel_B[1]), SimpFrac(kernel_B[2]), SimpFrac(kernel_B[3])].slice(0, layerID+1).join(",");
	var eList = (layerID==0) ? "" : [SimpFrac(kernel_E[0]), SimpFrac(kernel_E[1]), SimpFrac(kernel_E[2]), SimpFrac(kernel_E[3])].slice(0, layerID+1).join(",");
	var hasE = (kernel_E[0]>0 || kernel_E[1]>0 || kernel_E[2]>0 || kernel_E[3]>0);
	return "R=" + NS + ";peaks=["+bList+"];" + (hasE?"valleys=["+eList+"];":"") + "mu=" + delta_c + ";sigma=" + delta_w + ";dt=" + Round(1/TS);
}

function DiluteTime() {
	var found = false;
	for (var n=0; n<=TS_LIST.length-1; n++) {
		if (TS_LIST[n] > TS) {
			TS = TS_LIST[n];
			found = true;
			break;
		}
	}
	if (!found) TS = TS_LIST[TS_LIST.length-1];
	ResetStat();
}

function ConcentrateTime() {
	var found = false;
	for (var n=TS_LIST.length-1; n>=0; n--) {
		if (TS_LIST[n] < TS) {
			TS = TS_LIST[n];
			found = true;
			break;
		}
	}
	if (!found) TS = TS_LIST[0];
	ResetStat();
}

function SetKernelSize(newNS, isResize) {
	if (newNS != NS) {
		if (isResize) TransformField(0, 0, newNS / NS, 0, 0);
		NS = newNS;
		CalcKernel();
		NextGen(false);
	}
}

function NextDeltaAdj(d) {
	// 10 3 1
	var lastD = delta_adj.toString().split('').pop();
	var s = d==+1 ? 
		(lastD==1 ? 3 : 10/3) : 
		(lastD==3 ? 1/3 : 3/10);
	return Bound(delta_adj * s, 0.00001, 0.01);
}

function SimpFrac(n) {
	if (n>=B_SIMP.length)
		return [n,B_DIV].join("/");
	else
		return B_SIMP[n];
}

function SimpFrac2(n) {
	if (n>=B_SIMP.length)
		return "<span class=frac><sup>"+n+"</sup><span>&frasl;</span><sub>"+B_DIV+"</sub></span>";
	else {
		var s = B_SIMP[n].split("/");
		if (s.length==2)
			return "<span class=frac><sup>"+s[0]+"</sup><span>&frasl;</span><sub>"+s[1]+"</sub></span>";
		else
			return s[0];
	}
}

function UnSimpFrac(st) {
	if (!st) return 0;
	if (st.endsWith("/"+B_DIV)) {
		return parseInt(st.split("/")[0]);
	} else {
		var i = B_SIMP.indexOf(st);
		return i!=-1 ? i : 0;
	}
}

//*** animals
function ToCellArray(cellSt) {
	var isZip1 = cellSt.startsWith(ZIP_HEADER);
	var isZip2 = cellSt.startsWith(ZIP2_HEADER);
	var isZip = isZip1 || isZip2;
	if (isZip1) cellSt = cellSt.substring(ZIP_HEADER.length);
	if (isZip2) cellSt = cellSt.substring(ZIP2_HEADER.length);
	var colSep = isZip ? "" : ",";

	var cells = cellSt.split("/");
	var h = cells.length;
	var w = 0;
	for (var i=0; i<h; i++) {
		var row = cells[i].trim();
		if (isZip) {
			var row2 = row.split("-");
			row = "";
			for (var j=0; j<row2.length; j++) {
				var row3 = row2[j].split(".");
				if (row3.length == 1) {
					row += row2[j];
				} else {
					row += ToZip(0).repeat(FromRepeatSt(row3[0])) + row3[1];
				}
			}
		}
		cells[i] = row = row.split(colSep);
		w = Math.max(w, row.length);
		if (isZip) {
			for (var j=0; j<row.length; j++) {
				row[j] = Round(FromZip(row[j]) / 100);
			}
		}
	}

	if (isZip2) {
		var cells2 = new Array(h);
		for (var i=0; i<h; i++) {
			var row = cells[i];
			cells2[i*2] = new Array(row.length*2);
			cells2[i*2+1] = new Array(row.length*2);
			for (var j=0; j<row.length; j++) {
				cells2[i*2][j*2] = cells2[i*2][j*2+1] = cells2[i*2+1][j*2] = cells2[i*2+1][j*2+1] = cells[i][j];
			}
		}
		h *= 2;
		w *= 2;
		cells = cells2;
	}

	return { arr:cells, w:w, h:h };
}

function AddCells(cellSt, isZoom, isRandom) {
	var scale = isZoom ? 1<<optAnimalX : 1;

	if (optRepeat!=Repeat_COLLIDE || (optRepeat==Repeat_COLLIDE && collideStage==Collide_1ST)) {
		lastCellSt = collideCellSt = cellSt;
		lastZoom = collideZoom = isZoom;

		if (isRandom) {
			isShowArrow = false;
			ShowOptions();
		} else {
			ClearField(0);
			isShowArrow = true;
			ShowOptions();
		}

		var cells = ToCellArray(cellSt);
		if (isRandom) {
			var border1 = Math.floor(RAND_BORDER * F_SIZE), border2 = F_SIZE - border1;
			var angle = (NS == 2) ? 0 : RandomFloat(0, 360);
			AddCellArray(cells, border1, border1, border2, border2, scale, angle, RandomInt(0, 3));
		} else {
			AddCellArray(cells, Math.floor(F_SIZE/2-cells.w/2*scale), Math.floor(F_SIZE/2-cells.h/2*scale), 0, 0, scale, 0, 0);
		}
		if (optRepeat==Repeat_COLLIDE) {
			collideStage = Collide_2ND;
			collideNS = NS;
		}
	} else {  // (optRepeat==Repeat_COLLIDE && collideStage!=Collide_1ST)
		collideCellSt = cellSt;
		collideZoom = isZoom;

		isShowArrow = true;
		ShowOptions();

		if (NS == collideNS) {
			collideStage = Collide_ADJ;
			accumX = 50; accumY = 50; accumA = 180; accumF = 0;
			ShowCollide();
		} else {
			collideStage = Collide_ERR;
		}
	}

	ShowOptions();
	ResetCounters();

	NextGen(false);
	CalcMoments();
}

function AddCellArray(cells, x1, y1, x2, y2, scale, angle, flip) {
	//*** flip:1(horiz), 2(vert), 3(h+v)
	//var angle = RandomFloat(0, 360);
	var sin = Math.sin(angle / 180 * π);
	var cos = Math.cos(angle / 180 * π);
	var fh = ( Math.abs(cells.h*cos) + Math.abs(cells.w*sin) + 1) * scale - 1;
	var fw = ( Math.abs(cells.w*cos) + Math.abs(cells.h*sin) + 1) * scale - 1;
	var fi0 = (y2<=0) ? y1 : RandomInt(y1, y2 - fh);
	var fj0 = (x2<=0) ? x1 : RandomInt(x1, x2 - fw);
	for (var fi=0; fi<fh; fi++) {
		for (var fj=0; fj<fw; fj++) {
			var i = Math.round( (- (fj-fw/2)*sin + (fi-fh/2)*cos) / scale + cells.h/2 );
			var j = Math.round( (+ (fj-fw/2)*cos + (fi-fh/2)*sin) / scale + cells.w/2 );
			var fii = Mod(fi+fi0, F_SIZE);
			var fjj = Mod(fj+fj0, F_SIZE);
			if (flip==2||flip==3) i = cells.h - 1 - i;
			if (flip==1||flip==3) j = cells.w - 1 - j;
			var c = (i>=0 && j>=0 && i<cells.h && j<cells.arr[i].length) ? cells.arr[i][j] : 0;
			var v = (c!="") ? parseFloat(c) : 0;
			if (v>0) field[fii][fjj] = v;
		}
	}
}

function ShowCollide() {
	ClearField(0);
	var cells1 = ToCellArray(lastCellSt);
	var cells2 = ToCellArray(collideCellSt);
	var scale1 = lastZoom ? 1<<optAnimalX : 1;
	var scale2 = collideZoom ? 1<<optAnimalX : 1;
	AddCellArray(cells1, Math.floor(F_SIZE/2-cells1.h/2), Math.floor(F_SIZE/2-cells1.w/2), 0, 0, scale1, 0, 0);
	AddCellArray(cells2, Math.floor(F_SIZE/2-cells2.h/2) + accumX, Math.floor(F_SIZE/2-cells2.w/2) + accumY, 0, 0, scale2, accumA, accumF);
	
	NextGen(false);
	CalcMoments();
}

function GetCubeID(arr, n) {
	var st = "";
	for (var i=0; i<arr.length; i++)
		st += arr[i].toString(16);
	return st.toUpperCase().substring(0, n ? n : layerID+1);
}

function PopulateCube() {
	const m = 12, m2 = m*2;
	const cubeW = m*2+1;
	const cubeH = m*4+2;
	var cube = document.getElementById("CubeTable");
	for (var i=0; i<cubeH; i++) {
		var tr = cube.appendChild(document.createElement("TR"));
		for (var j=0; j<cubeW; j++) {
			var td = tr.appendChild(document.createElement("TD"));
			var isEven = (i+j)%2==0;
			var s = Math.floor((i+j-m)/2);
			var d = Math.floor((i-j+m)/2);
			if (s>=0 && s<=m2 && d>=0 && d<=m2) {
				var b0 = (d>=m) ? Math.min(j, m) : Math.min(s, m);
				var b1 = (j>=m) ? Math.min(m2-s, m) : Math.min(m2-d, m);
				var b2 = (s>=m) ? Math.min(m2-j, m) : Math.min(d, m);
				var id = GetCubeID([b0, b1, b2], 3);
				td.classList.add("cubeCell");
				if (isEven) {
					td.innerHTML = id; //j+" "+s+" "+d;
					td.id = "cube"+id;
					td.classList.add("cubeTop");
				} else {
					td.innerHTML = "&nbsp;";
					td.id = "cube"+id+"u";
					td.classList.add("cubeBottom");
				}
				td.addEventListener("click", ClickCube);
				td.addEventListener("mouseover", HoverCube);
				td.addEventListener("mouseout", HoverCube);
				if (j==0 || j==m2 || (j==m&&s>=m) ||
					s==0 || s==m2 || (s==m&&j>=m) ||
					d==0 || d==m2 || (d==m&&j<=m)) {
					td.classList.add("cubeBorder");
				}
			} else {
				td.classList.add("cubeEmpty");
				const FLOAT = "<div style='position:absolute;' class='normal'>";
				const UNFLOAT = "</div>";
				if (j==1 && i==1) td.innerHTML = FLOAT+"&#8598; U"+UNFLOAT;
				if (j==3 && i==1) td.innerHTML = FLOAT+"&#8593; I"+UNFLOAT;
				if (j==5 && i==1) td.innerHTML = FLOAT+"&#8599; O"+UNFLOAT;
				if (j==1 && i==3) td.innerHTML = FLOAT+"&#8601; J"+UNFLOAT;
				if (j==3 && i==3) td.innerHTML = FLOAT+"&#8595; K"+UNFLOAT;
				if (j==5 && i==3) td.innerHTML = FLOAT+"&#8600; L"+UNFLOAT;
				if (j==1 && i==m*3+7) td.innerHTML = FLOAT+"&#8598; &beta;<sub>1</sub>"+UNFLOAT;
				if (j==2 && i==m*3+9) td.innerHTML = FLOAT+"&#8595; &beta;<sub>2</sub>"+UNFLOAT;
				if (j==3 && i==m*3+7) td.innerHTML = FLOAT+"&#8599; &beta;<sub>3</sub>"+UNFLOAT;
				if (j==m+5 && i==0) td.innerHTML = FLOAT+"<u>Kernel</u>:"+UNFLOAT;
				if (j==m+7 && i==0) td.innerHTML = FLOAT+"<canvas id='graphK2' class='graph'></canvas>"+UNFLOAT;
			}
		}
	}
/*
total cells = (m+1)*(m+1)*3 - (m+1)*3 + 1
	b1						b2						b3
0			0						2*						0		
1		0		1				2*		2*				1		0	
2	0		1		2*		2*		2*		2*		2*		1		0
3		1		2*				2*		2*				2*		1	
4	0		2*		2*		1		2*		1		2*		2*		0
5		1		2*				1		1				2*		1	
6	0		2*		2*		0		1		0		2*		2*		0
7		1		2*				0		0				2*		1	
8			2*						0						2*		
i
	j						s						d				
			2*						0						0		
		1		3				0		1				1		0	
	0		2*		4		0		1		2*		2*		1		0
		1		3				1		2*				2*		1	
	0		2*		4		1		2*		3		3		2*		1
		1		3				2*		3				3		2*	
	0		2*		4		2*		3		4		4		3		2*
		1		3				3		4				4		3	
			2*						4						4		
*/
}

function RemoveClass(c) {
	var list = document.getElementsByClassName(c);
	while (list && list.length)
		list[0].classList.remove(c);
}

function AddClass(id, c) {
	var cell = document.getElementById("cube"+id);
	if (cell) cell.classList.add(c);
	cell = document.getElementById("cube"+id+"u");
	if (cell) cell.classList.add(c);
}

function RefreshCube() {
	RemoveClass("cubeExist");
	RemoveClass("cubeNonExist");
	for (var id in cubeList)
		AddClass(id, cubeList[id]!=MISSING_MARK ? "cubeExist" : "cubeNonExist");
}

function ClickCube(e) {
	var item = e.target;
	var id = item.id.substring(4, 7);
	ChangeLayer(2);
	kernel_B[0] = parseInt(id.charAt(0), 16);
	kernel_B[1] = parseInt(id.charAt(1), 16);
	kernel_B[2] = parseInt(id.charAt(2), 16);
	var st = cubeList[id];
	if (st && st!=MISSING_MARK) {
		SetRuleAndAddCells(st, false, false);
		status = lastZoomName = "Pasted cube<br>Rule: " + lastRuleSt.replace("<","&lt;").replace(">","&gt;");
		DrawAllPanels();
		lastAdd = Add_PASTE;
	}
	PositionCube();
	ShowCalculation();
	CalcKernel();
}

function PositionCube() {
	RemoveClass("cubeSel");
	var id = GetCubeID(kernel_B);
	AddClass(id, "cubeSel");
	DrawAllGraphs();
}

function HoverCube(e) {
	RemoveClass("cubeHover");
	if (e.type=="mouseover") {
		var id = e.target.id.substring(4, 7);
		AddClass(id, "cubeHover");
	}
}

function PopulateAnimalList() {
	if (!animalArr) return;
	var list = document.getElementById("AnimalList");
	if (!list) return;
	list.innerHTML = "";
	var lastCode = "";
	var lastEng0 = "";
	var lastChi0 = "";
	var node = list;
	var currLevel = 0;
	for (var i=0; i<animalArr.length; i++) {
		var a = animalArr[i];
		if (a.length >= 3) {
			var codeSt = a[0].split("(")[0];
			var engSt = a[1].split(" ");
			var chiSt = a[2].split("(");
			if (codeSt.startsWith("~")) codeSt = codeSt.substring(1);
			if (codeSt.startsWith("*")) codeSt = "";
			var sameCode = codeSt != "" && (codeSt == lastCode);
			var sameEng0 = engSt[0] != "" && (engSt[0] == lastEng0);
			var sameChi0 = chiSt[0] != "" && (chiSt[0] == lastChi0);
			lastCode = codeSt;
			lastEng0 = engSt[0];
			lastChi0 = chiSt[0];
			if (a.length >= 4) {
				var cellSt = a[3];
				var st = cellSt.split(";");
				var ruleSt = "";
				if (st.length >= 3) ruleSt = st[0].trim()+";"+st[1].trim()+";"+st[2].trim();
				var li = node.appendChild(document.createElement("LI"));
				li.classList.add("action");
				li.title = a[0] + " " + engSt.join(" ") + " " + chiSt.join("(") + "\n" + ruleSt;
				if (sameCode) codeSt = "−".repeat(codeSt.length);
				if (sameEng0) engSt[0] = lastEng0.substring(0, 1) + ".";
				if (sameChi0) chiSt[0] = "~";
				li.innerHTML = codeSt + " " + engSt.join(" ") + " " + chiSt.join("(") + " " + GetLayerSt(ruleSt);
				li.dataset["animalid"] = i;
				li.addEventListener("click", SelectAnimalItem);
				//node.parentElement.classList.add("closed");
			} else if (a.length == 3) {
				var nextLevel = parseInt(codeSt.substring(1));
				var diffLevel = nextLevel - currLevel;
				var backNum = (diffLevel<=0) ? -diffLevel+1 : 0;
				var foreNum = (diffLevel>0) ? diffLevel : 1;
				for (var k=0; k<backNum; k++) {
					node = node.parentElement;
					if (node.tagName == "LI") node = node.parentElement;
				}
				node = node.appendChild(document.createElement("LI"));
				node.classList.add("group");
				var div = node.appendChild(document.createElement("DIV"));
				div.title = engSt.join(" ") + " " + chiSt.join("(");
				div.innerHTML = engSt.join(" ") + " " + chiSt[0].split(" ")[0];
				div.addEventListener("click", function (e) { this.parentElement.classList.toggle("closed"); });
				//div.style.backgroundColor = RGBSt(COLORS[GREEN][nextLevel-1]);
				/*
				if (engSt[0]=="order:") {
					var img = div.appendChild(document.createElement("IMG"));
					img.src = "catalogue/" + engSt[1] + ".png";
					img.className = "catImg";
					img.style.float = "right";
					var wall = div.appendChild(document.createElement("DIV"));
					wall.style.clear = "both";
				}
				*/
				for (var k=0; k<foreNum; k++) {
					node = node.appendChild(document.createElement("UL"));
				}
				currLevel = nextLevel;
			}
		}
	}
}

function PopulateCatalogue() {
	if (!catalogueArr) return;
	var list = document.getElementById("Catalogue");
	if (!list) return;
	list.innerHTML = "";
	for (var i=0; i<catalogueArr.length; i++) {
		var row = catalogueArr[i];
		var tr = list.appendChild(document.createElement("TR"));
		for (var j=0; j<row.length; j++) {
			var a = row[j];
			var srcSt = a[0];
			var codeSt = a[1];
			var engSt = a[2];
			var chiSt = a[3];
			var td = tr.appendChild(document.createElement("TD"));
			var img = td.appendChild(document.createElement("IMG"));
			img.src = srcSt;
			img.title = engSt + " " + chiSt;
			img.className = "catImg";
			img.dataset["code"] = codeSt;
			img.addEventListener("click", function (e) { DefaultGroups(); SelectAnimalCode(this.dataset["code"]); });
		}
	}
}

function OpenAllGroups(isOpen, prefix) {
	var groups = document.getElementsByClassName("group");
	for (var i=0; i<groups.length; i++) {
		if (prefix == null || groups[i].innerText.startsWith(prefix)) {
			if (isOpen)
				groups[i].classList.remove("closed");
			else
				groups[i].classList.add("closed");
		}
	}
}

function DefaultGroups() {
	OpenAllGroups(false);
	OpenAllGroups(true, "class:");
	OpenAllGroups(true, "order:");
	OpenAllGroups(true, "subfamily:");
}

function HighlightItem(item, isHideOthers) {
	if (lastAnimalItem) lastAnimalItem.classList.remove("selected");
	if (item) {
		item.classList.add("selected");
		var node = item;
		while (node = node.parentElement) {
			if (node.classList.contains("closed"))
				node.classList.remove("closed");
		}
		var e = item, t = item.offsetTop;
		while (e = e.offsetParent)
			t += e.offsetTop;
		var div = document.getElementById("AnimalWindow");
		var divTop = (window.pageYOffset || document.documentElement.scrollTop) + div.getBoundingClientRect().top;
		div.scrollTop = t - divTop - div.clientHeight/2;
	}
	lastAnimalItem = item;
}

function SelectAnimalNext(isRandom, d, n) {
	if (d==null) d = 0;
	if (n==null) n = 0;
	var id = lastAnimalID;
	var id0 = id;
	for (var i=0; i<n; i++) {
		id += d;
		while (id >= 0 && id < animalArr.length && animalArr[id].length < 4)
			id += d;
		if (id >= 0 && id < animalArr.length)
			id0 = id;
	}
	if (id0 >= 0 && id0 < animalArr.length)
		SelectAnimalID(id0, isRandom);
}

function SelectAnimalItem(e) {
	var item = e.target;
	if (!item.dataset["animalid"]) item = item.parentElement;
	var id = parseInt(item.dataset["animalid"]);
	SelectAnimalID(id);
	//HighlightItem(item);
	ShowCounters();
}

function GetItemByID(id) {
	var items = document.querySelectorAll("[data-animalid='"+id+"']");
	return (items.length >= 1) ? items[0] : null;
}

function SelectAnimalID(id, isRandom) {
	lastAnimalID = id;
	HighlightItem(GetItemByID(id));
	var isZoom = true;
	var a = animalArr[id];
	var codeSt = a[0].split("(")[0];
	var engSt = a[1];
	var chiSt = a[2];
	if (codeSt.startsWith("~")) {
		isZoom = false;
		codeSt = codeSt.substring(1);
	} else if (codeSt.startsWith("[")) {
		ResizeField(8, 0);
		isZoom = false;
	}
	var v = a[3];
	if (v != "") SetRuleAndAddCells(v, isZoom, isRandom);
	status = lastZoomName = 
		"Selected soliton<br><span class='codeName'>" + codeSt + "</span> " +
		"<span class='engName'><b>" + engSt.replace(" "," </b>") + "</b></span> " +
		"<span class='chiName'><b>" + chiSt.replace("("," </b>(") + "</b></span><br>" +
		(layerID==0 ? "Unimodal" : layerID==1 ? "Bimodal" : layerID==2 ? "Trimodal" : layerID==3 ? "Tetramodal" : "") + " kernel" +
		" " + GetLayerSt(lastRuleSt);
	lastAdd = Add_SOL;
}

function SelectAnimalCode(code, isRandom) {
	for (var i=0; i<animalArr.length; i++) {
		var code1 = animalArr[i][0];
		if (code1 == code || code1.split("(")[0] == code) {
			SelectAnimalID(i, isRandom);
			break;
		}
	}
}

function GetLayerSt(st) {
	if (!st) return "";
	var s = st.split(/\=|\;|\*/);
	var s3 = s[3].split(/\(|\)|\,/);
	var l = Math.max(s3.length - 3, 0);
	return "[" + (l+1) + "]";
	/*
	var st = "";
	if (l == 0) 
		st = "&#9607;";
	else {
		if (l >= 1) st += "&#960" + (Math.floor(UnSimpFrac(s3[1])/2)+1) + ";";
		if (l >= 1) st += "&#960" + (Math.floor(UnSimpFrac(s3[2])/2)+1) + ";";
		if (l >= 2) st += "&#960" + (Math.floor(UnSimpFrac(s3[3])/2)+1) + ";";
		if (l >= 3) st += "&#960" + (Math.floor(UnSimpFrac(s3[4])/2)+1) + ";";
	}
	*/
	return "<span class='layerSt'>" + (l+1) + " " + st + "</span>";
}

function ChangeLayer(n) {
	var oldL = layerID;
	layerID = Mod(n, layerName.length);
	if (oldL != layerID) {
		var dS = (layerID+1) / (oldL+1);
		NS = Math.round(Bound(NS * dS, 2, F_SIZE));
	} 
	CalcKernel();
}

function ParseRule(st) {
	//0 1  2 3                                   4 5                  6 
	//      (0     1 2   3   4  5 6 7   8   9  10)(0    1     2     3)
	//R=13;k=bump4(1,2/3,1/3,2/3)(1,1/6,1/6,1/6);d=gaus(0.282,0.0358)*0.1
	//  NS   C     kernel_B  {L}  kernel_E         D    delta_c, w    TS
	var s = st.split(/\=|\;|\*/);
	var r = {};
	
	if (s.length > 1) {
		r.NS = parseInt(s[1]);
	}
	if (s.length > 6) {
		r.LV = !s[6].endsWith("+");
		r.TS = Round(1/parseFloat(s[6]));
	}

	if (s.length > 3) {
		var s3 = s[3].split(/\(|\)|\,/);
		var s3p = s3[0].substring(4).split("/");
		var hasE = s[3].indexOf(")(") >= 0;
		r.ruleD = -1; r.ruleL = -1; r.ruleC = -1;
		r.kernel_B = new Array(4).fill(0);
		r.kernel_E = new Array(4).fill(0);

		//*** backward compatibility
		if (s3[0]=="bimo4") { s3[0] = "qua4d"; }
		else if (s3[0]=="bist4") { s3[0] = "stpz1/4"; }
		else if (s3[0]=="trmo4") { s3[0] = "quad4"; ruleL = 2; }
		if (s3p.length >= 3) { r.kernel_B[0] = UnSimpFrac(s3p[1]+"/"+s3p[2]); r.ruleL = 1; }
		//***

		r.ruleC = coreName.indexOf(s3[0]);
		if (r.ruleL == -1) {
			if (hasE)
				r.ruleL = Math.max((s3.length - 5) / 2, 0); // 5,7,9,11 = 0,1,2,3
			else
				r.ruleL = Math.max(s3.length - 3, 0); // 3,4,5,6 = 0,1,2,3
		}
		for (var i=0; i<4; i++) {
			r.kernel_B[i] = UnSimpFrac(s3[i+1]);
			if (hasE) r.kernel_E[i] = UnSimpFrac(s3[i+3+r.ruleL]);
		}
		if (r.ruleL <= 1 && r.kernel_B[0] == 0) r.kernel_B[0] = B_DIV;
	}

	if (s.length > 5) {
		var s5 = s[5].split(/\(|\)|\,/);
		r.ruleD = deltaName.indexOf(s5[0]);
		r.delta_c = parseFloat(s5[1]);
		r.delta_w = parseFloat(s5[2]);
	}

	return r;
}

function SetRule(st) {
	var oldNS = NS;
	lastRuleSt = st;
	var r = ParseRule(st);
	
	NS = r.NS;
	TS = r.TS;
	isLimitValue = r.LV;
	if (!isLimitValue && optFormat == Format_ZIP) optFormat = Format_TEXT;
	kernel_B = r.kernel_B;
	kernel_E = r.kernel_E;
	delta_c = r.delta_c;
	delta_w = r.delta_w;
	if (r.ruleD != -1 && r.ruleL != -1 && r.ruleC != -1 && 
			(deltaID != r.ruleD || layerID != r.ruleL || coreID != r.ruleC)) {
		deltaID = r.ruleD;
		layerID = r.ruleL;
		coreID = r.ruleC;
		if (oldNS != 2 && NS == 2)
			ResizeAll(6);
		else if (oldNS == 2 && NS != 2)
			ResizeAll(7);
	}
	
	ResetStat();
	PositionCube();
	CalcKernel();
	RefreshPlot();
}

function SetRuleAndAddCells(st, isZoom, isRandom) {
	//0    1            2                        3
	//R=13;k=bump4(r<R);d=gaus(0.282,0.0358)*0.1;cells=
	var s = st.split(";");

	var cellSt = "";
	if (s.length == 1) {
		cellSt = st;
	} else if (s.length >= 4) {
		cellSt = s[3].split("=")[1];
		var ruleSt = s.slice(0, -1).join(";");
		SetRule(ruleSt);
	}
	if (cellSt) {
		if (NS == 2) isZoom = false;
		var scale = isZoom ? 1<<optAnimalX : 1;
		if (scale!=1) {
			NS = Math.round(Bound(NS * scale, 2, F_SIZE));
			CalcKernel();
		}
		AddCells(cellSt, isZoom, isRandom);
	}
	DrawAllPanels();
	ShowCalculation();
	DrawAllGraphs();
}

//*** copy
function TrimCells(cells, j1, j2) {
	var h = cells.length;
	var w = cells[0].length;
	var isChop = true;
	var col = 0;
	for (var i=0; i<h; i++)
		col += cells[i][j1];
	if (col == 0) {
		for (var i=0; i<h; i++)
			cells[i] = cells[i].slice(j2, w-1 + j2);
		isChop = true;
	} else {
		isChop = false;
	}
	return isChop;
}

function ToZip(v) {
	return v==0 ? "0" : v==100 ? "1" : /* 1-99 */ String.fromCharCode(v + (ZIP_START-1));
}

function FromZip(c) {
	return c=="0" ? 0 : c=="1" ? 100 : /* 1-99 */ (c.charCodeAt(0) - (ZIP_START-1));
}

function IsZip(c) {
	return (c.charCodeAt(0) >= ZIP_START);
}

function ToRepeatSt(d) {
	return (d==1) ? "" : (d<100) ? ToZip(d) : ToZip(Math.floor(d/100))+ToZip(d%100);
}

function FromRepeatSt(st) {
	return (st=="") ? 1 : IsZip(st) 
		? ( (st.length==1) ? FromZip(st) : FromZip(st[0])*100+FromZip(st[1]) )
		: parseInt(st);
}

/*
function TextToZip(cellSt, format, shrink) {
	if (!cellSt.startsWith(ZIP_HEADER)) {
		var cells = ToCellArray(cellSt);
		return CopyField(cells.arr, cells.w, cells.h, format, shrink);
	} else
		return cellSt;
}
*/

function CopyField(arr, width, height, format, isShrink) {
	var isZip1 = format==Format_ZIP;
	var isZip2 = format==Format_ZIP2;
	var isZip = isZip1 || isZip2;
	var isTable = format==Format_TABLE;
	var isMatlab = format==Format_MATLAB;
	var colSep = isTable ? "\t" : isZip ? "" : ",";
	var rowSep = isTable ? "\n" : isMatlab ? ";" : "/";
	var D = !isZip2 ? 1 : 2;
	var H = Math.floor(height/D);
	var W = Math.floor(width/D);
	var cells = new Array(H);
	var rowSum = new Array(H).fill(0);
	var colSum = new Array(W).fill(0);
	for (var i=0; i<H; i++) {
		cells[i] = new Array(W);
		for (var j=0; j<W; j++) {
			var v;
			var row = arr[i*D];
			v = (j*D<row.length) ? Round(row[j*D]) : 0;
			if (isZip2) {
				var v2 = (j*D<arr[i*D].length) ? Round(arr[i*D][j*D]) : 0;
				var v3 = (j*D<arr[i*D+1].length) ? Round(arr[i*D][j*D]) : 0;
				var v4 = (j*D<arr[i*D+1].length) ? Round(arr[i*D][j*D]) : 0;
				v = (v+v2+v3+v4) / 4;
			}
			cells[i][j] =
				//(format==Format_TEXT) ? (v==0 ? "" : v==1 ? "1" : v.toString().substr(1)) :
				//(format==Format_ZIP) ? (v==0 ? "" : v==1 ? "1" : v.toFixed(2).substr(1)) : v;
				isZip ? ToZip(Math.floor(v*100)) : v;
			rowSum[i] += v;
			colSum[j] += v;
		}
	}

	//*** shrink area to non-zero values
	var st = [];
	if (isShrink) {
		var i1 = 0, i2 = H-1, j1 = 0, j2 = W-1;
		while (IsZero(rowSum[i1])) i1++;
		while (IsZero(rowSum[i2])) i2--;
		if (i1 > i2) return "";
		while (IsZero(colSum[j1])) j1++;
		while (IsZero(colSum[j2])) j2--;

		var zero = isZip ? ToZip(0) : "";
		for (var i=i1; i<=i2; i++) {
			var row = cells[i];
			if (isZip) {
				var j1a = j1, j2a = j2;
				if (!isTable) {
					while (row[j1a]==zero) j1a++;
					while (row[j2a]==zero) j2a--;
				}
				if (j1a <= j2a) {
					var j1d = j1a - j1;
					var leading = (j1d==0) ? "" : ToRepeatSt(j1d) + ".";
					st.push(leading + row.slice(j1a, j2a+1).join(colSep));
				} else {
					st.push("");
				}
			} else {
				st.push(row.slice(j1, j2+1).join(colSep));
			}
		}
	} else {
		for (var i=0; i<=H-1; i++)
			st.push(cells[i].join(colSep));
	}
	st = st.join(rowSep);

	if (isZip1) st = ZIP_HEADER + st;
	if (isZip2) st = ZIP2_HEADER + st;

	return st;
}

function CopyToClipboard(text) {
	var textArea = document.getElementById("copyArea");
	textArea.value = text;
	textArea.select();
	document.execCommand("cut");
	textArea.blur();
}

function CenterField(isRotate) {
	if (optRepeat==Repeat_COLLIDE && collideStage==Collide_ADJ) {
		accumX = 50; accumY = 50; accumA = 180; accumF = 0;
	} else if (optRepeat==Repeat_MIRROR) {
		accumX = accumY = accumA = accumF = 0;
	}
	if (optCalc == Calc_NONE)
		CalcMoments(true);
	TransformFieldOrAdjust(-mX+F_SIZE/2, -mY+F_SIZE/2, 1, isRotate?-aM-90:0, 0);
	if (isRotate) aM = -90;
}

//*** canvas
function InitCanvas(id) {
	var canvas = document.getElementById(id);
	canvas.width = canvas.height = P_SIZE;
	canvas.style.width = canvas.width+"px"; canvas.style.height = canvas.height+"px";
	var ctx = canvas.getContext("2d");
	var img = ctx.createImageData(canvas.width, canvas.height);
	var rect = canvas.getBoundingClientRect();

	return { can: canvas, ctx: ctx, img: img, left: rect.left, top: rect.top };
}

function DrawCircle(canvas, x, y, size, color) {
	if (color!=null) canvas.ctx.strokeStyle = RGBSt(COLORS[optColor][color]);
	canvas.ctx.beginPath();
	canvas.ctx.arc(x, y, size, 0, π*2);
	canvas.ctx.stroke();
}

function FillCircle(canvas, x, y, size, color) {
	if (color!=null) canvas.ctx.fillStyle = RGBSt(COLORS[optColor][color]);
	canvas.ctx.beginPath();
	canvas.ctx.arc(x, y, size, 0, π*2);
	canvas.ctx.fill();
}

function DrawLine(canvas, x1, y1, x2, y2, isArrow, color) {
	if (color!=null) canvas.ctx.strokeStyle = RGBSt(COLORS[optColor][color]);
	canvas.ctx.lineWidth = 1;
	canvas.ctx.beginPath();
	canvas.ctx.moveTo(x1, y1);
	canvas.ctx.lineTo(x2, y2);
	if (isArrow) {
		const head = 10;  // length of arrow head
		var angle = Math.atan2(y2-y1, x2-x1);
		canvas.ctx.lineTo(x2-head*Math.cos(angle-π/6), y2-head*Math.sin(angle-π/6));
		canvas.ctx.moveTo(x2, y2);
		canvas.ctx.lineTo(x2-head*Math.cos(angle+π/6), y2-head*Math.sin(angle+π/6));
	}
	canvas.ctx.stroke();
}

function DrawDirection(canvas, x1, y1, x2, y2, scale, isArrow, color) {
	var x2 = x1 + (x2-x1)*scale;
	var y2 = y1 + (y2-y1)*scale;
	//debugSt = Num(x1) + " - " + Num(x2) + ", " + Num(y1) + " - " + Num(y2) + "<br>";
	for (var i=-1; i<=1; i++)
		for (var j=-1; j<=1; j++)
			DrawLine(canvas, x1+i*P_SIZE, y1+j*P_SIZE, x2+i*P_SIZE, y2+j*P_SIZE, isArrow, color);
}

function DrawArray(canvas, arr, scale, isHalf) {
	var isAutoCenter = (optCenter!=Center_OFF) && !isHalf && (NS != 2);
	var isKernel = (arr == kernel);
	var buf = isAutoCenter ? canvasT.img.data : canvas.img.data;
	var p = 0, S, S1, S2, isSmooth = false;
	for (var i=0; i<P_SIZE; i++) {
		var i2 = Math.floor(i/PIXEL);
		var ii = !isHalf ? i2 : F_SIZE - ((i2 + F_SIZE/2) % F_SIZE) - 1;

		for (var j=0; j<P_SIZE; j++) {
			var j2 = Math.floor(j/PIXEL);
			var jj = !isHalf ? j2 : ((j2 + F_SIZE/2) % F_SIZE);
			var v = arr[ii][jj] * scale;
			var c = Math.floor(v*10);
			if (v >= 1) S = COLORS[optColor][10];
			else if (IsPos(v)) S = COLORS[optColor][c];
			else if (v<NEG_THRES) S = COLORS[GREY][Math.floor((-v+NEG_THRES)/(1+NEG_THRES)*NEG_COLORS)];  // Math.min(10,)
			else S = COLOR_ZERO;
			//*** draw delta contours
			if (optAid==Aid_TRACE && !isHalf && !isKernel) {
				var d = delta[i2][j2];
				if (d >= 0.99) S = COLORS[optColor][LMARK];
				else if (d <= 0.1 && d >= -0.1) S = COLORS[optColor][MMARK];
				else if (d <= -0.998 && d >= -0.999) S = COLORS[optColor][DMARK];  // && (i+j)%3==0
			}
			for (var n=0; n<3; n++)
				buf[p++] = S[n]*0x11;
			/*
			isSmooth = false;
			if (IsPos(v)) {
				if (v >= 1) S = COLORS[optColor][10];
				else {
					isSmooth = true;
					S1 = COLORS[optColor][c];
					S2 = COLORS[optColor][c+1];
				}
			} else {
				if (i % 20 == 0 && j % 20 == 0) S = COLOR_DOT;
				else if (IsNeg(v)) S = COLOR_NEG;
				else S = COLOR_ZERO;
			}
			for (var n=0; n<3; n++)
				if (!isSmooth)
					buf[p++] = S[n]*0x11;
				else
					buf[p++] = Math.floor(S1[n]*0x11 + (S2[n]*0x11-S1[n]*0x11) * (v*10-c));
			*/
			buf[p++] = 255;
		}
	}

	var canvas0 = isAutoCenter ? canvasT : canvas;
	canvas0.ctx.putImageData(canvas0.img, 0, 0);

	if (optCalc!=Calc_NONE && IsPos(m00) && !isHalf && !isKernel) {
		//*** draw arrows
		if (optAid==Aid_TRACE && isShowArrow) {
			const ARROW_TIME = 2;  // length of arrow (estimated distance travelled within time period)
			FillCircle(canvas0, oldmX*PIXEL, oldmY*PIXEL, 2, DMARK);
			DrawDirection(canvas0, oldmX*PIXEL, oldmY*PIXEL, oldmX*PIXEL+Math.cos(th/180*π)*PIXEL, oldmY*PIXEL+Math.sin(th/180*π)*PIXEL, NS*PIXEL, true, DMARK);
			if (dM/NS*PIXEL*TS>=0.01) {
				DrawDirection(canvas0, oldmX*PIXEL, oldmY*PIXEL, arrowX*PIXEL, arrowY*PIXEL, ARROW_TIME*TS*PIXEL, true, MMARK);
			}
			//DrawDirection(canvas0, mX, mY, gX, gY, 1, false, LMARK);
			FillCircle(canvas0, mX*PIXEL, mY*PIXEL, 2, MMARK);
			FillCircle(canvas0, gX*PIXEL, gY*PIXEL, 2, LMARK);
		}
	}

	//*** auto center
	if (isAutoCenter) {
		var tX = oldmX*PIXEL; // P_SIZE/2 oldmX*PIXEL mX*PIXEL
		var tY = oldmY*PIXEL;
		canvas.ctx.save();
		canvas.ctx.fillStyle = RGBSt(COLOR_ZERO);
		canvas.ctx.fillRect(0, 0, P_SIZE, P_SIZE);
		canvas.ctx.translate(P_SIZE/2, P_SIZE/2);
		if (optCenter==Center_AM) canvas.ctx.rotate((-aM-90) * π / 180);
		else if (optCenter==Center_TH) canvas.ctx.rotate(-th * π / 180);
		canvas.ctx.drawImage(canvas0.can, -tX, -tY, P_SIZE, P_SIZE);
		canvas.ctx.restore();
	}

	//*** draw crosshair
	if (optAid==Aid_TRACE && !isHalf && !isKernel) {
		const CROSS_WIDTH = 10;
		canvas.ctx.strokeStyle = RGBSt(COLORS[optColor][DMARK]);
		var a0 = P_SIZE/2, a1 = P_SIZE/2-CROSS_WIDTH, a2 = P_SIZE/2+CROSS_WIDTH;
		DrawLine(canvas, a1, a0, a2, a0, false);
		DrawLine(canvas, a0, a1, a0, a2, false);
	}

	//*** draw ruler
	if (optAid!=Aid_OFF) {
		const TEXT_WIDTH = 35;
		var len = 1; // !isHalf ? 1 : 2;
		var x0 = !isHalf ? P_SIZE-TEXT_WIDTH : P_SIZE/2+NS*PIXEL*len;
		var y0 = !isHalf ? P_SIZE-8 : P_SIZE/2;
		canvas.ctx.fillStyle = RGBSt(COLORS[optColor][MMARK]);
		canvas.ctx.fillText("R", x0+5, y0+5);
		canvas.ctx.fillRect(x0-NS*PIXEL*len, y0+3, NS*PIXEL*len, 2);
		for (var n=0; n<=4*len; n++)
			canvas.ctx.fillRect(x0-NS*PIXEL*n/4, y0, 1, 5);

		if (!isHalf) {
			x0 = P_SIZE-TEXT_WIDTH;
			y0 = P_SIZE-20;
			canvas.ctx.fillStyle = RGBSt(COLORS[optColor][DMARK]);
			canvas.ctx.fillText("10 px", x0+5, y0+5);
			canvas.ctx.fillRect(x0-10*PIXEL, y0, 10*PIXEL, 2);
			for (var n=0; n<=2; n++)
				canvas.ctx.fillRect(x0-10*PIXEL*n/2, y0, 1, 5);
		}
	}

	//*** draw mouse over circles
	if (optAid==Aid_TRACE && !isHalf && !isKernel && mouseX != -1) {
		canvas.ctx.setLineDash([1, 2]);
		DrawCircle(canvas, mouseX*PIXEL, mouseY*PIXEL, NS*PIXEL, DMARK);
		DrawCircle(canvas, mouseX*PIXEL, mouseY*PIXEL, NS*PIXEL/2, MMARK); //LMARK
		DrawCircle(canvas, mouseX*PIXEL, mouseY*PIXEL, NS*PIXEL*3/4);
		DrawCircle(canvas, mouseX*PIXEL, mouseY*PIXEL, NS*PIXEL/4);
		canvas.ctx.setLineDash([]);
		FillCircle(canvas, mouseX*PIXEL, mouseY*PIXEL, 2, LMARK);
	}

}

function DrawAllPanels() {
	DrawArray(canvas1, field, 1);
	if (optWindows==Windows_CALC) {
		DrawArray(canvas2, delta, 1);
		DrawArray(canvas3, neigh, neighLevel/neighDiv/delta_c);
		//DrawArray(canvas3, neigh, 1.1/neighMax, true);
		if (!isKernelDrawn) DrawArray(canvas4, kernel, 1);
		isKernelDrawn = true;
	} else if (optWindows==Windows_STAT) {
		DrawStatsArc(canvas2, Value_PERIOD);
		DrawStatsArc(canvas3, Value_AVG);
		DrawStatsArc(canvas4, Value_VAL);
		isKernelDrawn = false;
	}
	//DrawArray(canvas3, fieldAm, 0.1, true);
	//DrawArray(canvas4, fieldPh, 1, true);
	//DrawArray(canvas4, kernelRe, 1, true);

	if (plottingArr != null)
		Plotly.redraw("PlotDiv");
}

function StatRGBSt(part) {
	var s = 2 - Math.floor(part/STAT_SPLIT);
	var c = (part<STAT_SPLIT) ? part : part-STAT_SPLIT;
	return RGBSt(COLORS[s][c]);
}

function DrawStatsBar() {
	for (var part=0; part<STAT_N; part++) {
		var info = STAT_INFO[part];
		var name = info[0], arcNo = info[1], type = info[2], max = info[3];
		var s = stat[part];
		var p = (s.period.star!=null && s.period.star!=0) ? Round(s.period.star / TS) : null;
		var vals = [statVals[part], s.avg, p];

		var v = statVals[part];
		var x = ( type!=sLOG ? v : Math.log(Math.abs(v)) ) / max * STAT_H;
		var abs = Math.abs(x);

		if (optCalc!=Calc_NONE) {
			var canvas = document.getElementById("stat"+part);
			if (!canvas) return;
			var ctx = canvas.getContext("2d");

			//*** draw stat bar
			ctx.clearRect(((statIndex+1)%STAT_W)*STAT_S, 0, STAT_S, STAT_H);
			if (IsNeg(x)) {
				ctx.fillStyle = RGBSt(COLORS[GREY][1]);
				ctx.fillRect(statIndex*STAT_S, 0, STAT_S, STAT_H-abs);
			}
			ctx.fillStyle = StatRGBSt(part);
			ctx.fillRect(statIndex*STAT_S, STAT_H-abs, STAT_S, abs);

			//*** draw period line
			if (optCalc==Calc_ALL) {
				if (gen >= PERIOD_PRE && (s.index % 2) == 0) {
					var sd1 = s.last1 * STAT_H * STAT_H;
					ctx.fillStyle = RGBSt(COLORS[BLUE][LMARK]);
					ctx.fillRect(statIndex*STAT_S, STAT_H-sd1, STAT_S, sd1);
					/*
					if (type==sANGLE) {
						var sd2 = s.last2 * STAT_H * STAT_H;
						ctx.fillRect((statIndex-1)*STAT_S, STAT_H-sd2, STAT_S, sd2);
					}
					*/
					//*** draw period restart line
					if (s.index == 0) {
						var isSuccess = s.period.curr != null && s.period.curr != 0;
						ctx.fillStyle = RGBSt(COLORS[BLUE][isSuccess ? MMARK : LMARK]);
						ctx.fillRect(statIndex*STAT_S, 0, STAT_S, STAT_H);
					}
				}
			}
		}

		//*** write value
		var div = document.getElementById("val"+part);
		if (!div) return;
		if (optCalc==Calc_ALL || (optCalc==Calc_STAT && optValue!=Value_PERIOD)) {
			if (div) {
				v = vals[optValue];
				var a = Math.abs(v);
				var st = (v==null||isNaN(v)) ? "-" : (v==0) ? "&nbsp;0" : (v > 0 ? "+" : "") + (
					(a < 1000 && a >= 10) ? v.toFixed(2) :
					(a < 10 && a >= 0.001) ? v.toPrecision(3) :
						v.toExponential(2).replace("e","e<sup>") + "</sup>" );
				if (optValue==2) {
					st = st + (s.period.sure==null ? "" : s.period.sure ? "*" : "?");
				} else {
					if (type==sLOG) st = "<u>" + st + "</u>";
				}
				div.innerHTML = st;
			}
		} else {
			div.innerHTML = "";
		}
	}
}

function DrawStatsArc(canvas, which) {
	canvas.ctx.clearRect(0, 0, P_SIZE, P_SIZE);
	if (optCalc==Calc_ALL || (optCalc==Calc_STAT && which!=Value_PERIOD)) {
		//*** draw two circles
		var tr = Round(Math.pow(2, LOG_SIZE+LOG_PIXEL-8));
		canvas.ctx.strokeStyle = which==Value_VAL ? RGBSt(COLORS[BLUE][LMARK]) : which==Value_AVG ? RGBSt(COLORS[BLUE][MMARK]) : RGBSt(COLORS[GREEN][MMARK]);
		DrawCircle(canvas, P_SIZE/2, P_SIZE/2, 50*tr);
		DrawCircle(canvas, P_SIZE/2, P_SIZE/2, 100*tr);

		//*** draw arcs
		for (var part=0; part<STAT_N; part++) {
			var info = STAT_INFO[part];
			var name = info[0], arcNo = info[1], type = info[2], max = info[3];
			var s = stat[part];
			var p = (s.period.star!=null && s.period.star!=0) ? Round(s.period.star / TS) : null;
			var vals = [statVals[part], s.avg, p];

			var tr = Round(Math.pow(2, LOG_SIZE+LOG_PIXEL-8));
			var a1 = 2*π / STAT_N;
			var a0 = π/2;
			var v = vals[which];
			var x = (which==Value_PERIOD) ? v * 20 : ( type!=sLOG ? v : Math.log(Math.abs(v)) ) / max * 100;
			//var x = val / (1 + Math.abs(val)) * 100;
			canvas.ctx.strokeStyle = canvas.ctx.fillStyle = StatRGBSt(part);
			canvas.ctx.lineWidth = 1;
			canvas.ctx.beginPath();
			canvas.ctx.moveTo(P_SIZE/2, P_SIZE/2);
			canvas.ctx.arc(P_SIZE/2, P_SIZE/2, Math.abs(x)*tr, (arcNo+1/10)*a1-a0, (arcNo+1)*a1-a0);
			canvas.ctx.closePath();
			if (which==Value_PERIOD) {
				if (s.period.sure) canvas.ctx.fill(); else canvas.ctx.stroke();
			} else {
				if (IsPos(x)) canvas.ctx.fill(); else canvas.ctx.stroke();
			}

			var ta = (arcNo+1/2)*a1-a0;
			var tx = Math.cos(ta)*tr*100 + P_SIZE/2;
			var ty = Math.sin(ta)*tr*100 + P_SIZE/2;
			canvas.ctx.font = "italic 12px serif";
			canvas.ctx.fillStyle = "#333";
			canvas.ctx.textBaseline = "middle";
			if (name.includes("_")) {
				var names = name.split("_");
				canvas.ctx.textAlign = "right";  canvas.ctx.fillText(names[0], tx, ty);
				canvas.ctx.textAlign = "left";   canvas.ctx.fillText(names[1], tx, ty+3);
			} else {
				canvas.ctx.textAlign = "center"; canvas.ctx.fillText(name, tx, ty);
			}
			canvas.ctx.font = "10px sans-serif";
		}
		
		
		//*** draw overall period line
		if (optCalc==Calc_ALL && which==Value_PERIOD && period0.star!=null && period0.star!=0) {
			var p = Round(period0.star / TS);
			canvas.ctx.strokeStyle = RGBSt(COLORS[GREEN][period0.sure?DMARK:MMARK]);
			DrawLine(canvas, P_SIZE/2, P_SIZE/2, P_SIZE/2, P_SIZE/2-p*20*tr, true);
		}
	}

	/*
	//*** draw value type
	canvas.ctx.textAlign = "right";
	var type = which==Value_VAL ? "" : which==Value_AVG ? "Average" : which==Value_PERIOD ? "Period" : "??";
	canvas.ctx.fillText(type, P_SIZE-10, 10);
	canvas.ctx.textAlign = "left";
	*/
}

function DrawSwatches() {
	var canvas = document.getElementById("swatches1");
	canvas.width = 145; canvas.height = 20;
	canvas.style.width = canvas.width+"px"; canvas.style.height = canvas.height+"px";
	var ctx = canvas.getContext("2d");
	ctx.clearRect(0, 0, canvas.width, canvas.height);

	for (var c=-1; c<=10; c++) {
		var S = (c>-1 ? COLORS[optColor][c] : COLOR_ZERO);
		ctx.fillStyle = RGBSt(S);
		ctx.fillRect(c*12+15, 0, 12, 10);
		if (c==-1) ctx.fillStyle = "#d99";
		ctx.fillText(c==-1?"0":c==10?" 1":"."+c, c*12+15, 20);
	}

	canvas = document.getElementById("swatches2");
	canvas.width = 35; canvas.height = 20;
	canvas.style.width = canvas.width+"px"; canvas.style.height = canvas.height+"px";
	ctx = canvas.getContext("2d");
	ctx.clearRect(0, 0, canvas.width, canvas.height);

	for (var d=0; d<=2; d++) {
		var S = COLORS[optColor][d+LMARK];
		ctx.fillStyle = RGBSt(S);
		ctx.fillRect(d*12, 0, 12, 10);
		ctx.fillStyle = "#d99";
		ctx.fillText(1-d, d*12, 20);
	}
}

//*** info
function ResetCounters() {
	time = 0;
	oldth = dth = th180 = oldaM = shiftX = shiftY = 0;
	mX = mY = oldmX = oldmY = F_SIZE/2;
	//tht = ths1 = ts2 = 0;
	transA = transS = null;
	ResetStat();
}

function InitAllStat() {
	statIndex = -1;
	statVals = [];
	period0 = { arr: {}, star: null, sure: null };
	stat = [];
	for (var k=0; k<STAT_N; k++) {
		stat[k] = { avg: null, 
			arr: new Array(PERIOD_W).fill(null), index: -1, max1: 0, last1: 0, max2: 0, last2: 0, 
			period: { curr: null, arr: {}, star: null, sure: null } };
	}
	document.getElementById("cb0").checked = true;
	document.getElementById("cb6").checked = true;
	document.getElementById("cb10").checked = true;
	StatCheckBox();
}

function StatCheckBox() {
	plotStat = [];
	for (var k=0; k<STAT_N; k++) {
		var cb = document.getElementById("cb"+k);
		if (cb && cb.checked)
			plotStat.push(k);
	}
	if (plotStat.length < 3) plotStat.push(0, 0, 0);
	if (plottingArr == stat)
		PlotLine(false);
}

function ResetStat() {
	gen = 0;
	statIndex = -1;
	statVals = [];
	period0.arr = {};
	period0.star = null;
	period0.sure = null;
	for (var k=0; k<STAT_N; k++) {
		var s = stat[k];
		s.avg = null; 
		s.arr.fill(null);
		s.index = -1;
		s.max1 = 0; s.last1 = 0;
		s.max2 = 0; s.last2 = 0; 
		s.period.curr = null;
		s.period.arr = {};
		s.period.star = null;
		s.period.sure = null;
		var canvas = document.getElementById("stat"+k);
		if (canvas) {
			canvas.width = STAT_W; canvas.height = STAT_H;
			canvas.style.width = canvas.width+"px"; canvas.style.height = canvas.height+"px";
		}
	}
}

function ShowCounters() {
	document.getElementById("txt_t").innerHTML = time.toFixed(3);
	var p = period0.star;
	document.getElementById("txt_period").innerHTML = (p==null||p==0) ? "−" : Round(p / TS) + (period0.sure==null ? "" : period0.sure ? "*" : "?");
	document.getElementById("txt_fps").innerHTML = (fps!=0) ? fps.toFixed(1) : "−";
	//*** write period candidates
	if (optCalc==Calc_ALL) {
		var pSt = [];
		var j = 0;
		SortedKeys(period0.arr).forEach(function(p, i) {
			if (j < 5) { pSt.push("<span class='num'>" + Round(p / TS) + "</span><sup>" + period0.arr[p] + "</sup>"); j++; }
		});
		pSt = (pSt.length==0) ? "-" : pSt.join(" ");
		document.getElementById("txt_periods").innerHTML = pSt;
	}

	var statusSt = status;
	if (optRepeat==Repeat_COLLIDE) {
		statusSt += "<br><span class='colliderStatus'>"
			+ (collideStage==Collide_1ST ? "Paste/select 1st sample ..." : collideStage==Collide_2ND ? "Paste/select 2nd sample ..." : collideStage==Collide_ADJ ? "" : collideStage==Collide_ERR ? "R mismatch between samples!" : "")
			+ "</span>";
	}
	document.getElementById("txt_status").innerHTML = statusSt;

	var st = "";
	st += debugSt + (debugSt!=""?"<br>":"");
	st += debug.join("<br>");
	/*
	tht += 1;
	if (tht < 50) { ths1 += th; ths2 += th*th; }
	else { thc = ths1/(tht-1); thw = Math.sqrt((ths2 - ths1*ths1/(tht-1)) / (tht-2)); tht = ths1 = ths2 = 0; }
	st += "φ=<code>(1)"+Num2(φ1)+", (4)"+Num2(φ4)+", (5)"+Num2(φ5)+", (6)"+Num2(φ6)+", (9)"+Num2(φ9)+"</code><br>";
	st += "<code>(2)"+Num2(φ2)+", (3)"+Num2(φ3)+", (12)"+Num2(φ12)+"</code><br>";
	st += "<code>(10)"+Num2(φ10)+", (11)"+Num2(φ11)+", (7)"+Num2(φ7)+", (8)"+Num2(φ8)+", (13)"+Num2(φ13)+"</code><br>";
	st += "m="+Num(m00)+", v="+Num(vol,0)+"<br>";
	st += "m="+Num(m00/NS/NS)+", v="+Num(vol/NS/NS)+", &rho;="+Num(m00/vol)+", g00="+Num(g00/NS/NS)+", k="+Num(rog/NS)+"<br>";
	st += "s=" + Num(dM/NS*TS) + ", &omega;=" + Num(daM*TS) + " ("+Num(oldaM)+"-"+Num(aM)+")<br>";
	st += "seed=" + lastSeed + "<br>";
	st += "shift=" + shiftX + ", " + shiftY + "<br>";
	st += "a=" + Num(axisA) + ", b=" + Num(axisB) + ", th=" + Num(th) + ", ε=" + Num(ec) + "<br>";
	st += "θ= (&plusmn;" + Num(thw) + ") " + Num(th) + "<br>";
	st += "μ=<code>(22)"+Num2(μ22)+", (31)"+Num2(μ31)+", (13)"+Num2(μ13)+", (40)"+Num2(μ40)+", (04)"+Num2(μ04)+"</code><br>";
	st += "η=<code>(22)"+Num2(η22)+", (31)"+Num2(η31)+", (13)"+Num2(η13)+", (40)"+Num2(η40)+", (04)"+Num2(η04)+"</code><br>";
	//st += "μ11=" + Num(μ11) + "<br>μ20-μ02=" + Num(μ20-μ02) + "<br>";
	//st += "η11=" + η11 + "<br>η20-η02=" + (η20-η02) + "<br>";
	*/
	document.getElementById("txt_debug").innerHTML = st;
}

function ShowEquation(pre, info, id) {
	for (var k=0; k<info.length; k++) {
		var div = document.getElementById(pre + info[k]);
		if (div) div.style.display = (k==id) ? "" : "none";
	}
}

function ShowCalculation() {
	ShowEquation("eqD_", deltaName, deltaID);
	ShowEquation("eqL_", layerName, layerID);
	ShowEquation("eqC_", coreName, coreID);
	ShowEquation("nameD_", deltaName, deltaID);
	ShowEquation("nameL_", layerName, layerID);
	ShowEquation("nameC_", coreName, coreID);

	document.getElementById("txt_c").innerHTML = delta_c;
	document.getElementById("txt_w").innerHTML = delta_w;
	document.getElementById("txt_adj").innerHTML = delta_adj;
	document.getElementById("txt_R").innerHTML = NS;
	document.getElementById("txt_dt").innerHTML = Round(1/TS).toFixed(3);
	document.getElementById("txt_ts").innerHTML = TS;
	document.getElementById("txt_t").innerHTML = time.toFixed(3);
	document.getElementById("txt_A").innerHTML = kernel_A;
	document.getElementById("txt_B").innerHTML = (layerID==0) ? "−" : [SimpFrac2(kernel_B[0]), SimpFrac2(kernel_B[1]), SimpFrac2(kernel_B[2]), SimpFrac2(kernel_B[3])].slice(0, layerID+1).join(",") + " <span class='cubeCell'>" + GetCubeID(kernel_B) + "</span>";
	document.getElementById("txt_E").innerHTML = (layerID==0) ? "−" : [SimpFrac2(kernel_E[0]), SimpFrac2(kernel_E[1]), SimpFrac2(kernel_E[2]), SimpFrac2(kernel_E[3])].slice(0, layerID+1).join(",");
	document.getElementById("txt_size").innerHTML = F_SIZE + "&sup2;";
	document.getElementById("txt_pixel").innerHTML = PIXEL + "&sup2;"; //+LOG_SIZE+";"+LOG_PIXEL;
	document.getElementById("txt_NCap").innerHTML = neighDiv +"/"+ neighLevel;
	document.getElementById("txt_value1").style.display = isLimitValue ? "" : "none";
	document.getElementById("txt_value2").style.display = !isLimitValue ? "" : "none";
}

function DrawAxis(ctx, x1, y1, x2, y2, minX, minY, scaleX, scaleY, height) {
	ctx.beginPath();
	ctx.moveTo((x1-minX)*scaleX, height-(y1-minY)*scaleY);
	ctx.lineTo((x2-minX)*scaleX, height-(y2-minY)*scaleY);
	ctx.stroke();
}

function DrawGraph(func, id, minX, maxX, scaleX, minY, maxY, scaleY) {
	var width = (maxX-minX) * scaleX;
	var height = (maxY-minY) * scaleY;
	var canvas = document.getElementById(id);
	var ctx = canvas.getContext("2d");
	canvas.width = width;
	canvas.height = height;
	canvas.style.width = (width*1)+"px";
	canvas.style.height = (height*1)+"px";;

	ctx.lineWidth = 1;
	ctx.strokeStyle = "#55ff55";
	DrawAxis(ctx, minX, 0, maxX, 0, minX, minY, scaleX, scaleY, canvas.height);
	DrawAxis(ctx, 0, minY, 0, maxY, minX, minY, scaleX, scaleY, canvas.height);
	ctx.setLineDash([2, 2]);
	ctx.strokeStyle = "#bbbbbb";
	if (func == DeltaFunc) {
		DrawAxis(ctx, 0.5, minY, 0.5, maxY, minX, minY, scaleX, scaleY, canvas.height);
		DrawAxis(ctx, delta_c, minY, delta_c, maxY, minX, minY, scaleX, scaleY, canvas.height);
		DrawAxis(ctx, delta_c+delta_w, minY, delta_c+delta_w, maxY, minX, minY, scaleX, scaleY, canvas.height);
		DrawAxis(ctx, delta_c-delta_w, minY, delta_c-delta_w, maxY, minX, minY, scaleX, scaleY, canvas.height);
		var neighCap = 1 / (neighLevel/neighDiv/delta_c);
		DrawAxis(ctx, neighCap, minY, neighCap, maxY, minX, minY, scaleX, scaleY, canvas.height);
	} else if (func == KernelFunc) {
		var N = layerID==2 ? 2 : 3;
		var D = layerID==2 ? 3 : 4;
		for (var n=1; n<=N; n++) {
			DrawAxis(ctx, n/D, minY, n/D, maxY, minX, minY, scaleX, scaleY, canvas.height);
			DrawAxis(ctx, -n/D, minY, -n/D, maxY, minX, minY, scaleX, scaleY, canvas.height);
		}
	}
	ctx.setLineDash([]);

	ctx.lineJoin = "round";
	ctx.lineWidth = 1;
	ctx.strokeStyle = "#00aaff";
	ctx.beginPath();
	ctx.moveTo(0, canvas.height-(func(minX)-minY)*scaleY);
	for (var x=minX+1/scaleX; x<=maxX; x+=1/scaleX) {
		//ctx.beginPath();
		//ctx.moveTo((x-minX)*scaleX, canvas.height-(-minY)*scaleY);
		ctx.lineTo((x-minX)*scaleX, canvas.height-(func(x)-minY)*scaleY);
		//ctx.stroke();
	}
	ctx.stroke();
}

function CopyGraph(id1, id2) {
	var canvas1 = document.getElementById(id1);
	var canvas2 = document.getElementById(id2);
	var ctx2 = canvas2.getContext("2d");
	canvas2.width = canvas1.width;
	canvas2.height = canvas1.height;
	canvas2.style.width = canvas1.style.width;
	canvas2.style.height = canvas1.style.height;
	ctx2.drawImage(canvas1, 0, 0);
}

function DrawAllGraphs() {
	DrawGraph(DeltaFunc, "graphD", -0.005, 1, 400, -1.05, 1.05, 20);
	DrawGraph(KernelFunc, "graphK", -1, 1, 100, -0.05, 1.05, 50);
	CopyGraph("graphK", "graphK2");
}

function PlotSurface(arr) {
	if (arr) {
		SwitchPanel(Panel_PLOT);
		//var colorArr = new Array(11);
		//for (var i=0; i<11; i++)
		//	colorArr[i] = [ Round(i/10.0), RGBSt(COLORS[optColor][i]) ];
		var data = [ { z: arr, type: "surface", showscale: false, colorscale: "Portland" } ];
		var layout = {
			margin: { l: 0, r: 0, b: 0, t: 0, pad: 0 },
			scene: {
				aspectratio: { x: 1.2, y: 1.2, z: 0.3 },
			    camera: { center: { x:0, y:0, z:-0.2 }, eye: { x:0, y:1, z:0.7 }, },
				xaxis: { visible: false }, yaxis: { visible: false }, 
				zaxis: { visible: false, range: [-0.01, 1.01] },
			},
		};
		if (arr==delta) layout.scene.zaxis.range = [-1.01, 1.01];
		else if (arr==neigh) layout.scene.zaxis.range = [-0.01, 1.5/(neighLevel/neighDiv/delta_c)];
		Plotly.newPlot("PlotDiv", data, layout);
	} else {
		Plotly.purge("PlotDiv");
	}
	plottingArr = arr;
}

function PlotLine(isSwitch=true) {
	if (isSwitch) SwitchPanel(Panel_PLOT);
	var arr1 = stat[plotStat[0]].arr;
	var arr2 = stat[plotStat[1]].arr;
	var arr3 = stat[plotStat[2]].arr;
	var name1 = STAT_INFO[plotStat[0]][0];
	var name2 = STAT_INFO[plotStat[1]][0];
	var name3 = STAT_INFO[plotStat[2]][0];
	var data = [ { 
		x: arr1, y: arr2, z: arr3, 
		type: "scatter3d", mode: "lines+markers", showscale: false, 
	 	line: { width: 1, color: "rgb(0,50,200)" }, marker: { size: 2, color: "rgb(50,200,0)" } } ];
	var layout = {
		margin: { l: 0, r: 0, b: 0, t: 0, pad: 0 },
		scene: {
			aspectratio: { x: 1, y: 1, z: 1 },
		    camera: { center: { x:0, y:0, z:-0.2 }, eye: { x:1, y:1, z:1 }, },
			xaxis: { title: name1 }, yaxis: { title: name2 }, zaxis: { title: name3 }, 
		},
	};
	Plotly.newPlot("PlotDiv", data, layout);
	plottingArr = stat;
}

function RefreshPlot() {
	if (plottingArr!=neigh) return;

	var layout = PlotDiv.layout;
	layout.scene.zaxis.range = [-0.01, 1.5/(neighLevel/neighDiv/delta_c)];
	Plotly.relayout("PlotDiv", layout);
}

//*** GIF
function DoGIF() {
	if (optGIF==GIF_ON) {
		gif.setRepeat(0);
		gif.setDelay(50);
		gif.setQuality(1);
		gif.start();
		gif.addFrame(canvas1.ctx);
	} else {
		gif.finish();
		gif.download("download.gif");
	}
}

//*** keyboard
function RepeatAdd(isForce) {
	if (optRepeat==Repeat_ADD || isForce) {
		if (lastAdd==Add_PASTE || lastAdd==Add_SOL) {
			if (optRepeat!=Repeat_COLLIDE) {
				AddCells(lastCellSt, lastZoom, false);
				status = lastZoomName;
			} else if (optRepeat==Repeat_COLLIDE && collideStage==Collide_ADJ) {
				ShowCollide();
			}
		} else if (lastAdd==Add_RAND) {
			ClearField(0);
			UseSeed(GetNewSeed());
			AddRandom(lastFill?0.5:1, lastFill);
		}
	} else if (optRepeat==Repeat_MIRROR) {
		AddCells(lastCellSt, lastZoom, false);
		TransformField(accumX, accumY, 1, accumA, accumF);
		TransformField(0, 0, 1, 0, lastMirror);
	}
}

function ChangeClass(on, off, cond) {
	var from = cond ? off : on;
	var to = cond ? on : off;
	var btns = document.getElementsByClassName(from);
	while (btns && btns.length)
		btns[0].className = btns[0].className.replace(from, to);
}

function ShowOptions() {
	document.getElementById("optAnimalX").value = "x" + (1 << optAnimalX);
	document.getElementById("optCenter").value = optCenter==Center_OFF ? "none" : optCenter==Center_POS ? "Center" : optCenter==Center_AM ? "Rotate(ω​)" : optCenter==Center_TH ? "Rotate(θ)" : "??";
	document.getElementById("optColor").value = optColor==GREY ? "Gray" : optColor==BLUE ? "Blue" : optColor==GREEN ? "Green" : optColor==RAINBOW ? "Rainbow" : "??";
	document.getElementById("optAid").value = optAid==Aid_OFF ? "none" : optAid==Aid_RULER ? "Ruler" : optAid==Aid_TRACE ? "Trace" : "??";
	document.getElementById("optWindows").value = optWindows==Windows_FIELD ? "Field only" : optWindows==Windows_CALC ? "Calculations" : optWindows==Windows_STAT ? "Statistics" : "??";
	document.getElementById("optRepeat").value = optRepeat==Repeat_OFF ? "none" : 
		optRepeat==Repeat_ADD ? (lastAdd==Add_RAND ? "Random" : lastAdd==Add_PASTE ? "Paste" : lastAdd==Add_SOL ? "Soliton" : "??") : 
		optRepeat==Repeat_MIRROR ? "Mirror" : optRepeat==Repeat_COLLIDE ? "Collide" : "??";
	document.getElementById("optCopy").value = optCopy==Copy_FIELD_S ? "Field" : optCopy==Copy_FIELD ? "Field (full)" : optCopy==Copy_DELTA ? "Delta (full)" : optCopy==Copy_NEIGH ? "Neighbor (full)" : optCopy==Copy_SEED ? "Seed" : "??";
	document.getElementById("optFormat").value = optFormat==Format_ZIP ? "Text" : optFormat==Format_TABLE ? "Table" : optFormat==Format_TEXT ? "Text (unzip)" : optFormat==Format_MATLAB ? "Matlab" : "??";
	document.getElementById("optGIF").value = optGIF==GIF_OFF ? "off" : optGIF==GIF_ON ? "Recording..." : "??";
	document.getElementById("optCalc").value = optCalc==Calc_ALL ? "Stat+Period" : optCalc==Calc_STAT ? "Stat" : optCalc==Calc_NONE ? "off" : "??";
	document.getElementById("optValue").value = optValue==Value_VAL ? "Value" : optValue==Value_AVG ? "Average" : optValue==Value_PERIOD ? "Period" : "??";
	document.getElementById("panel2_1").style.display = document.getElementById("panel3_1").style.display = document.getElementById("panel4_1").style.display = optWindows==Windows_CALC ? "" : "none";
	document.getElementById("panel2_2").style.display = document.getElementById("panel3_2").style.display = document.getElementById("panel4_2").style.display = optWindows==Windows_STAT ? "" : "none";
	document.getElementById("canvas2td1").style.display = document.getElementById("canvas3td1").style.display = document.getElementById("canvas4td1").style.display = optWindows!=Windows_FIELD ? "" : "none";
	document.getElementById("canvas2td2").style.display = document.getElementById("canvas3td2").style.display = document.getElementById("canvas4td2").style.display = optWindows!=Windows_FIELD ? "" : "none";
	ChangeClass("btnCenterOn", "btnCenterOff", optCenter!=Center_OFF);
	ChangeClass("btnAddOn", "btnAddOff", optRepeat==Repeat_ADD);
	ChangeClass("btnMirrorOn", "btnMirrorOff", optRepeat==Repeat_MIRROR);
	ChangeClass("btnGIFOn", "btnGIFOff", optGIF==GIF_ON);
	ChangeClass("btnCollideOn", "btnCollideOff", optRepeat==Repeat_COLLIDE && collideStage==Collide_ADJ);
}

function ShowPanel(name, isOn) {
	document.getElementById(name+"Btn").className = isOn ? "btnHigh" : "";
	document.getElementById(name+"Panel").style.display = isOn ? "" : "none";
}

function SwitchPanel(i) {
	var isChanged = (optPanel != i);
	ShowPanel("Info", i==Panel_INFO); ShowPanel("Plot", i==Panel_PLOT); ShowPanel("Ctrl", i==Panel_CTRL); ShowPanel("Calc", i==Panel_CALC); ShowPanel("Cube", i==Panel_CUBE); ShowPanel("Stat", i==Panel_STAT);
	if (i==Panel_CTRL) HighlightItem(lastAnimalItem);
	optPanel = i;
	return isChanged;
}

function PresetRule({fsize, psize, ns, ts, dfunc, lfunc, cfunc, A, C, W, isFill}) {
	if (psize)
		ResizeField(fsize, psize);
	else
		ResizeAll(fsize);
	optCenter = Center_OFF; optAnimalX = 0;
	NS = ns; TS = ts;
	deltaID = dfunc; delta_c = C; delta_w = W;
	layerID = lfunc, coreID = cfunc; kernel_A = A;
	CalcKernel();
	ResetStat();
	ClearField(0);
	UseSeed(GetNewSeed());
	AddRandom(isFill?0.5:1, isFill);
}

function PresetAnimal({fsize, psize, asize, animalCode}) {
	if (psize)
		ResizeField(fsize, psize);
	else
		ResizeAll(fsize);
	optCenter = Center_OFF; optAnimalX = (asize!=null) ? asize : 0;
	deltaID = 0; coreID = 0; layerID = 0; kernel_A = 4;
	ResetStat();
	InitAllCanvas();
	SelectAnimalCode(animalCode, false);
}

function ChangeBeta(b0, b1, b2, d0, shift, b, d) {
	if (optPanel==Panel_CUBE) {
		if (d0!=0) {
			if ((d0==-1 && (kernel_B[b0]==B_DIV || kernel_B[b1]==B_DIV)) ||
				(d0==+1 && kernel_B[b2]<B_DIV))
				kernel_B[b2] = Bound(kernel_B[b2]+d0, 0, B_DIV);
			else {
				kernel_B[b0] = Bound(kernel_B[b0]-d0, 0, B_DIV);
				kernel_B[b1] = Bound(kernel_B[b1]-d0, 0, B_DIV);
			}
			if (shift) {
				var id = GetCubeID(kernel_B);
				var st = cubeList[id];
				if (st && st!=MISSING_MARK) {
					SetRuleAndAddCells(st, false, false);
					status = lastZoomName = "Pasted cube<br>Rule: " + lastRuleSt.replace("<","&lt;").replace(">","&gt;");
					DrawAllPanels();
					lastAdd = Add_PASTE;
				}
				PositionCube();
				ShowCalculation();
				CalcKernel();
			}
		}
	} else {
		if (layerID >= Math.max(b, 1)) {
			if (shift) {
				kernel_E[b] = Bound(kernel_E[b]+d, 0, B_DIV);
			} else {
				kernel_B[b] = Bound(kernel_B[b]+d, 0, B_DIV);
			}
		} 
	}
	PositionCube();
	CalcKernel();
	RepeatAdd();
}

function DoAction(key, shift) {
	var D = shift ? 10 : 1;
	var X = shift ? 1 : 10;
	var A = shift ? 1 : 45;
	var K = shift ? -1 : +1;
	var adj = shift ? NextDeltaAdj(-1) : delta_adj;
	
	switch (key) {
		//*** running
		case SPACE: isRunOnce = true; isRunning = false; break;
		case ENTER: if (shift) runGen = period0.star; isRunning = !isRunning; break;

		//*** param
		case "Q": case PAD+7: delta_c = Bound(delta_c + adj*10, 0, 1); RepeatAdd(); RefreshPlot(); break;
		case "A": case PAD+4: delta_c = Bound(delta_c - adj*10, 0, 1); RepeatAdd(); RefreshPlot(); break;
		case "W": case PAD+8: delta_w = Bound(delta_w + adj, 0, 1); RepeatAdd(); break;
		case "S": case PAD+5: delta_w = Bound(delta_w - adj, 0, 1); RepeatAdd(); break;
		case "E": case PAD+9: delta_adj = NextDeltaAdj(+1); break;
		case "D": case PAD+6: delta_adj = NextDeltaAdj(-1); break;

		case "T": case MUL: DiluteTime(); break;
		case "G": case DIV: ConcentrateTime(); break;
		case "R": case ADD: SetKernelSize(Bound(NS + 1, 2, F_SIZE), !shift); break;
		case "F": case SUB: SetKernelSize(Bound(NS - 1, 2, F_SIZE), !shift); break;

		//*** soliton
		case "Z": SelectAnimalNext(false); break;
		case "X": SelectAnimalNext(true); break;
		case "C": case PAD+1: SelectAnimalNext(false, -1, D); break;
		case "V": case PAD+2: SelectAnimalNext(false, +1, D); break;
		case "B": case PAD+3: optAnimalX = Mod(optAnimalX+K, 4); break;
		case PAD+0: RepeatAdd(true); break;
		case INT+3: OpenAllGroups(true); break;
		case INT+4: DefaultGroups(); break;

		//*** random field
		case "N": ClearField(0); UseSeed(GetNewSeed()); AddRandom(shift?0.5:1, shift); break;
		case "M": case DEC: ClearField(0); UseSeed(lastSeed); AddRandom(shift?0.5:1, lastFill); break;
		case DEL: ClearField(0); break;
		case QUOTE: CenterField(shift); break;

		//*** transform field
		case LEFT:  TransformFieldOrAdjust(-X, 0, 1, 0, 0); break;
		case RIGHT: TransformFieldOrAdjust(+X, 0, 1, 0, 0); break;
		case UP:    TransformFieldOrAdjust(0, -X, 1, 0, 0); break;
		case DOWN:  TransformFieldOrAdjust(0, +X, 1, 0, 0); break;
		case HOME:  TransformFieldOrAdjust(0, 0, 1, -A, 0); break;
		case END:   TransformFieldOrAdjust(0, 0, 1, +A, 0); break;
		case PGUP:  TransformFieldOrAdjust(0, 0, 1, 0, Flip_H); break;
		case PGDN:  TransformFieldOrAdjust(0, 0, 1, 0, Flip_V); break;
		case SLASH: TransformField(0, 0, shift ? 1/2 : 2, 0, 0); break;
		case ALT+SLASH: TransformField(0, 0, shift ? 2/3 : 3/2, 0, 0); break;
		case COMMA: TransformField(0, 0, 1, 0, shift ? Mirror_D : Mirror_H); break;
		case DOT:   TransformField(0, 0, 1, 0, Mirror_X); break;

		//*** field size
		case LBRA: if (shift) ResizeField(LOG_SIZE, Bound(LOG_PIXEL-1, -2, 3)); else ResizeAll(Bound(LOG_SIZE-1, 5, 10)); break;  // ResizeField(Bound(LOG_SIZE-1, 5, 10), LOG_PIXEL);
		case RBRA: if (shift) ResizeField(LOG_SIZE, Bound(LOG_PIXEL+1, -2, 3)); else ResizeAll(Bound(LOG_SIZE+1, 5, 10)); break;
		case DASH:  ResizeAll(Bound(LOG_SIZE-1, 5, 10), true); break;
		case EQUAL: ResizeAll(Bound(LOG_SIZE+1, 5, 10), true); break;
		case BKSLASH: if (shift) ResizeField(LOG_SIZE, 0); else ResizeAll(8); break;

		//*** panels
		case "1": SwitchPanel(Panel_CTRL); break; // { optCalc==Calc_NONE; optCenter = Center_OFF; optWindows = 0; ResizeAll(7); ResetStat(); } break;
		case "2": SwitchPanel(Panel_CALC); break;
		case "3": SwitchPanel(Panel_STAT); break;
		case "4": SwitchPanel(Panel_PLOT); break;
		case "5": SwitchPanel(Panel_CUBE); break;
		case GRAVE: SwitchPanel(Panel_INFO); break;
		// deltaID = coreID = Mod(deltaID+K, deltaName.length); CalcKernel();

		//*** presets
		case "6": PresetRule({fsize:8, ns:18, ts:10, dfunc:0, lfunc:0, cfunc:0, A:4, C:0.14, W:0.015, isFill:false}); break;
		case "7": PresetRule({fsize:8, ns:13, ts:1,  dfunc:0, lfunc:0, cfunc:3, A:4, C:0.31, W:0.049, isFill:true}); break;
		case "8": PresetRule({fsize:6, ns:2,  ts:1,  dfunc:3, lfunc:0, cfunc:4, A:4, C:0.35, W:0.07,  isFill:false}); break;
		case "9": PresetAnimal({fsize:7, animalCode:DEFAULT_ANIMAL}); break;
		case "0": optCalc = Calc_NONE; optCenter = Center_OFF; optWindows = Windows_FIELD; ResizeField(9, 0, true); break;

		//*** options
		case INT+1: kernel_A = Bound(kernel_A + 1, 1, 99); CalcKernel(); break;
		case INT+2: kernel_A = Bound(kernel_A - 1, 1, 99); CalcKernel(); break;
		case "Y": ChangeBeta(0, 0, 0,  0, shift,  0, +1); break;
		case "U": ChangeBeta(1, 2, 0, -1, shift,  1, +1); break;
		case "I": ChangeBeta(2, 0, 1, +1, shift,  2, +1); break;
		case "O": ChangeBeta(0, 1, 2, -1, shift,  3, +1); break;
		case "H": ChangeBeta(0, 0, 0,  0, shift,  0, -1); break;
		case "J": ChangeBeta(0, 1, 2, +1, shift,  1, -1); break;
		case "K": ChangeBeta(2, 0, 1, -1, shift,  2, -1); break;
		case "L": ChangeBeta(1, 2, 0, +1, shift,  3, -1); break;
		case SEMICOLON:
			for (var i=0; i<=layerID; i++)
				kernel_B[i] = RandomInt(0, B_DIV);
			for (var i=0; i<=layerID; i++)
				kernel_E[i] = shift ? RandomInt(0, B_DIV) : 0;
			kernel_B[RandomInt(0, layerID)] = B_DIV;
			PositionCube();
			CalcKernel();
			RepeatAdd();
			break;

		case "P":
			var id = GetCubeID(kernel_B);
			if (!shift) {
				CenterField();
				var cellSt = CopyField(field, F_SIZE, F_SIZE, Format_ZIP, true);
				lastCellSt = cellSt;
				cubeList[id] = GetRuleSt() + ";cells=" + cellSt;
			} else {
				cubeList[id] = MISSING_MARK;
			}
			SwitchPanel(Panel_CUBE);
			RefreshCube();
			break;
		case ALT+"P":
			var id = GetCubeID(kernel_B);
			delete cubeList[id];
			SwitchPanel(Panel_CUBE);
			RefreshCube();
			break;
		case ALT+"Y": cubeList = []; SwitchPanel(Panel_CUBE); RefreshCube(); break;
		case ALT+"H": 
			for (var b0=0; b0<=B_DIV; b0++) {
				for (var b1=0; b1<=B_DIV; b1++) {
					for (var b2=0; b2<=B_DIV; b2++) {
						if (b0<B_DIV && b1<B_DIV && b2<B_DIV) continue;
						var id = GetCubeID([b0, b1, b2]);
						var bList = [SimpFrac(b0), SimpFrac(b1), SimpFrac(b2)].join(",");
						if (!cubeList[id]) cubeList[id] = MISSING_MARK;
					}
				}
			}
			SwitchPanel(Panel_CUBE);
			RefreshCube();
			break;

		case ALT+"U": PlotSurface(null); break;
		case ALT+"I": PlotSurface(field); break;
		case ALT+"O": PlotSurface(delta); break;
		case ALT+"K": PlotSurface(neigh); break;
		case ALT+"L": PlotSurface(kernel); break;
		case ALT+"J": PlotLine(); break;

		case ALT+"Q": deltaID = Mod(deltaID+K, deltaName.length); break;
		case ALT+"W": ChangeLayer(layerID+K); break;
		case ALT+"E": coreID = Mod(coreID+K, coreName.length-1); CalcKernel(); break;
		case ALT+"R": neighLevel = Bound(neighLevel - 1, 1, neighDiv*2); RefreshPlot(); break;
		case ALT+"T": neighLevel = Bound(neighLevel + 1, 1, neighDiv*2); RefreshPlot(); break;

		case ALT+"A": optCenter = Mod(optCenter+K, 4); if (optCalc==Calc_NONE && optCenter!=Center_OFF) optCalc = Calc_STAT; break;
		case ALT+"S": optColor = Mod(optColor+K, 3); isKernelDrawn = false; break;
		case ALT+"D": optAid = Mod(optAid+K, 3); isKernelDrawn = false; break;
		case ALT+"F": optWindows = Mod(optWindows+K, 3); isKernelDrawn = false; break;
		case ALT+"G": isLimitValue = !isLimitValue; break;

		case ALT+"Z": optRepeat = Mod(optRepeat+K, 4); accumX = accumY = accumA = accumF = 0; collideStage = Collide_2ND; collideNS = NS; break;
		case ALT+"X": optFormat = Mod(optFormat+K, 4); break;
		case ALT+"C": optCopy = Mod(optCopy+K, 5); break;
		case ALT+"V": optGIF = Mod(optGIF+K, 2); DoGIF(); break;

		case ALT+"B": ResetStat(); break;
		case ALT+"N": optCalc = Mod(optCalc+K, 3); if (optCalc==Calc_NONE) optCenter = Center_OFF; else if (optCalc==Calc_ALL) ResetStat(); break;
		case ALT+"M": optValue = Mod(optValue+K, 3); break;

		/*
		case COMMA:
			optCenter = Center_POS; optAnimalX = 1;
			optWindows = 3;
			TransformField(0, 0, 1, -aM-90+45, 0); aM = -90+45; break;
			isRunning = true;
			break;
		case DOT:
			optCenter = Center_OFF;
			isRunning = false;
			break;
		*/
		/*
		case ALT+"G":
			var layout = PlotDiv.layout;
			layout.scene.camera.center.z += 0.1*K;
			Plotly.relayout("PlotDiv", layout);
			break;
		case ALT+"J":
			var layout = PlotDiv.layout;
			layout.scene.camera.eye.z += 0.1*K;
			Plotly.relayout("PlotDiv", layout);
			break;
		*/
	}

	DrawAllPanels();
	DrawAllGraphs();
	DrawSwatches();
	ShowCalculation();
	ShowOptions();
	ShowCounters();
}
