

#define F_SAMPLE 8000. // Sample Frequency in Hertz

typedef struct {
    double old;
    double older;
    double out;
    double f0;
    double bw;
    double b ;
    double c ;

} tfilter ;


void filter_set(tfilter *filter , double f0, double bw );

double filter_compute(tfilter *filter, double in);
