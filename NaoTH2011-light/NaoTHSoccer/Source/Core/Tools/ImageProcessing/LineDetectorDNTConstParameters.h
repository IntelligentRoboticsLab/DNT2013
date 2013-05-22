/**
* @file LineDetectorDNTConstParameters.h
*
* @author <a href="mailto:giorgosmethe@gmail.com">Georgios Methenitis</a>
* Definition of constants for the LineDetectorDNT
*/

#ifndef __LineDetectorDNTConstParameters_H_
#define __LineDetectorDNTConstParameters_H_

#define MAX_LINE_THICKNESS 80
#define MIN_LINE_THICKNESS 4
#define SCAN_STEP MIN_LINE_THICKNESS - 1
#define MAX_SCAN_RESOLUTION 25
#define MIN_SCAN_RESOLUTION 15
#define MAX_AREA 89600.0
#define QUAL_WHITE_RATIO 0.55
#define CONN_WHITE_RATIO 0.60
#define BEST_CANDIDATE_BUFFER 4
#define CONN_ERROR_SINGLE 3.00
#define CONN_ERROR_COMPLEX 10.00

#endif //__LineDetectorDNTConstParameters_H_
