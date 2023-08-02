typedef struct {
	double r,g,b,a;
} COLOUR;

void GetColour(double v,double vmin,double vmax, COLOUR *c);
void drawCoords();

void drawText( char *cadena, float x, float y, float z);
