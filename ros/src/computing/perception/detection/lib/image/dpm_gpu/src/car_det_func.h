/////////////////////////Car tracking project with laser_radar_data_fusion/////////////////////////////////////////
//////////////////////////////////////////////////////////////////////Copyright 2009-10 Akihiro Takeuchi///////////

/////detect_func.h functions about car-detection (to extend main.cc) //////////////////////////////////////////////

#include <cstdio>
#include "MODEL_info.h"		//File information

#include "switch_float.h"

#ifndef INCLUDED_MFunctions_
#define INCLUDED_MFunctions_

//load_model.cpp

extern MODEL *load_model(FLOAT ratio);						//load MODEL(filter)
extern void free_model(MODEL *MO);							//release model

//detect.cpp

extern RESULT *car_detection(IplImage *IM,MODEL *MO,FLOAT thresh,int *D_NUMS,FLOAT *A_SCORE,FLOAT overlap);//car detection
extern IplImage *ipl_cre_resize(IplImage *IM,int width,int height);									//create and resize Iplimage
extern IplImage *ipl_resize(IplImage *IM,FLOAT ratio);												//resize image (IplImage)
extern FLOAT *ini_ac_score(IplImage *IM);															//initialize accumulated score
extern IplImage *load_suc_image(int fnum);															//load image_pictures

//showboxes.cpp
//extern void showboxes(IplImage *Image,MODEL *MO,FLOAT *boxes,int *NUM);
extern void show_rects(IplImage *Image,RESULT *CUR,FLOAT ratio);									//show rectangle-boxes
//extern void show_array(IplImage *Image,RESULT *LR,int *PP);										//show integer array(for debug)
//extern int *show_particles(IplImage *Image,RESULT *CUR,PINFO *P_I);								//show particles (for debug)
//extern void show_det_score(IplImage *Image,FLOAT *ac_score,RESULT *CUR);							//show detector score (for debug)
extern void show_vector(IplImage *Image,IplImage *TMAP,RESULT *CUR,PINFO *P_I,FLOAT ratio);;		//show vector of velocity
extern void print_information(void);
extern void save_result(IplImage *Image,int fnum);													//save result imagee
//extern void ovw_det_result(IplImage *OR,IplImage *DE, FLOAT ratio);								//over-write detection result


// tracking.cpp
extern RESULT *create_result(int num);
extern void release_result(RESULT *LR);																//release last_result data space
extern void update_result(RESULT *LR,RESULT *CUR);													//update result
extern RESULT *get_new_rects(IplImage *Image,MODEL *MO,FLOAT *boxes,int *NUM);
extern int finalization(RESULT *CUR,RESULT *LR,PINFO *P_I,FLOAT *A_SCORE,IplImage* Image,int THL);	//finalize tracking

#endif
