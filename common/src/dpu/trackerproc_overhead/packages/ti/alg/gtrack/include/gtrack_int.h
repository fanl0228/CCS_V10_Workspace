/**
 *   @file  gtrack_int.h
 *
 *   @brief
 *      This is the internal header file for GTRACK Algorithm
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef GTRACK_INT_H__
#define GTRACK_INT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "gtrack_listlib.h"

#define GTRACK_POS_DIMENSION (0U)
#define GTRACK_VEL_DIMENSION (1U)
#define GTRACK_ACC_DIMENSION (2U)

#define GTRACK_SPREAD_ALPHA     	                (0.05f) /* 5 percent */
#define GTRACK_CONFIDENCE_ALPHA     	            (0.1f)  /* 10 percent */
#define GTRACK_MIN_DISPERSION_ALPHA	                (0.1f)  /* 10 percent */
#define GTRACK_MIN_POINTS_TO_UPDATE_DISPERSION      (3)     /* 3 points */

#define GTRACK_NOMINAL_ALLOCATION_RANGE     	    (6.0f)  /* Range for allocation SNR scaling */

#define GTRACK_STATIC_MIN_CONIDENCE_LEVEL           (0.8f)  /* MInimal confidence level for static state */
/**
 * @brief 
 *  3x3 Matrix Definition
 *
 * @details
 *  The structure describes the 3x3 matrix that can be viewed as an row-wise array OR addressed by individual element.
 */
typedef union {
	float a[9];
	struct
	{
		float e11; float e12; float e13;
		float e21; float e22; float e23;
		float e31; float e32; float e33;
	};
} MATRIX3x3; 

/**
 * @brief 
 *  4x4 Matrix Definition
 *
 * @details
 *  The structure describes the 4x4 matrix that can be viewed as an row-wise array OR addressed by individual element.
 */
typedef union {
	float a[16];
	struct
	{
		float e11; float e12; float e13; float e14;
		float e21; float e22; float e23; float e24;
		float e31; float e32; float e33; float e34;
		float e41; float e42; float e43; float e44;
	};
} MATRIX4x4;

/**
 * @brief 
 *  6x6 Matrix Definition
 *
 * @details
 *  The structure describes the 6x6 matrix that can be viewed as an row-wise array OR addressed by individual element.
 */
typedef union {
	float a[36];
	struct
	{
		float e11; float e12; float e13; float e14; float e15; float e16;
		float e21; float e22; float e23; float e24; float e25; float e26;
		float e31; float e32; float e33; float e34; float e35; float e36;
		float e41; float e42; float e43; float e44; float e45; float e46;
		float e51; float e52; float e53; float e54; float e55; float e56;
		float e61; float e62; float e63; float e64; float e65; float e66;
	};
} MATRIX6x6;

/**
 * @brief 
 *  GTRACK rotation sincos
 * @details
 *  The structure defines coefficients needed for rotation along X axes
 */
typedef struct
{
    /**  @brief   sine of rotation angle */
    float sinTheta;
    /**  @brief   cosine of rotation angle */
    float cosTheta;
} GTRACK_rotation_sincos;

/**
 * @brief 
 *  GTRACK World Transformation
 * @details
 *  The structure defines the transformation from tracking spece to world coordinates
 *  in 3D configurations, this transformation is rotation along X axis follwed by move along Z axis
 */
typedef struct {
    /**  @brief   boolean to indicate whether transformation to world coordinates space is required */
    bool transformationRequired;
    /**  @brief   boolean to indicate sensor azimuth tilt angle not equal to zero */
    bool isAzimuthTilted;
    /**  @brief   structure to hold space rotation angle sine and cosine values */
    GTRACK_rotation_sincos rotX;
    /**  @brief   space vertical move value */
    float offsetZ;
    /**  @brief   structure to hold space rotation angle sine and cosine values */
    MATRIX3x3 rotZX;
} GTRACK_worldTransformParams;

/**
 * @brief 
 *  GTRACK Unrolling Parameters
 *
 * @details
 *  The structure describes the filtering parameters used to switch unrolling states
 */
typedef struct {
    /**  @brief  Range rate filtering alpha */
	float alpha;
    /**  @brief  Range rate filtering confidence */
	float confidence;
} GTRACK_unrollingParams;

/**
 * @brief 
 *  GTRACK Unit Parameters structure
 *
 * @details
 *  The structure describes the parameters used to instantiate GTRACK unit instance
 */
typedef struct {
	
    /**  @brief  Track Unit Identifier (UID) */
	uint8_t uid;
    /**  @brief  State vector type to use */
	GTRACK_STATE_VECTOR_TYPE stateVectorType;
    /**  @brief  Verbosenes mask to use */
    uint64_t verbose;
    
	/**  @brief Configured target maximum acceleration, in m/s2 */
	float   maxAcceleration[3];    
    /**  @brief Expected target radial velocity at the moment of detection, m/s */
    float   initialRadialVelocity;
    /**  @brief Absolute value of maximum radial velocity, m/s */
    float   maxRadialVelocity;
    /**  @brief Radial velocity resolution, m/s */
    float   radialVelocityResolution;
    /**  @brief Frame rate */
    float   deltaT;

    /**  @brief  Gating parameters */    
    GTRACK_gatingParams gatingParams;
    /**  @brief  Allocation parameters */
    GTRACK_allocationParams allocationParams;
    /**  @brief  Unrolling parameters */
    GTRACK_unrollingParams unrollingParams;
    /**  @brief  State parameters */
    GTRACK_stateParams stateParams;   
    /**  @brief  Scenery parameters */
    GTRACK_sceneryParams   sceneryParams;
    /**  @brief  Presence Detection parameters */
    GTRACK_presenceParams   presenceParams;
    
    /**  @brief  Transformation to world coordinates */
    GTRACK_worldTransformParams   transormParams;

	/**  @brief Pointer to current Transition matrix */
    float   *F;
	/**  @brief Pointer to current Process Noise matrix */
    float   *Q;

} TrackingParams;

/**
 * @brief    Macros that define values to use for \ref TrackingParams::verbose when specifying the required verboseness
 */
#define VERBOSE_ERROR_INFO			0x00000001      /*!< Report Errors */
#define VERBOSE_WARNING_INFO		0x00000002      /*!< Report Warnings */
#define VERBOSE_DEBUG_INFO			0x00000004      /*!< Report Debuging information */
#define VERBOSE_MATRIX_INFO			0x00000008      /*!< Report Matrix math computations */
#define VERBOSE_UNROLL_INFO			0x00000010      /*!< Report velocity unrolling data */
#define VERBOSE_STATE_INFO			0x00000020      /*!< Report state transitions */
#define VERBOSE_ASSOSIATION_INFO	0x00000040      /*!< Report association data */
#define VERBOSE_GATEXY_INFO			0x00000080      /*!< Report gating in XY space */
#define VERBOSE_GATERA_INFO			0x00000100      /*!< Report gating in range/angle space */
#define VERBOSE_GATEG1_INFO			0x00000200      /*!< Report unitary gating */

/**
 * @brief 
 *  GTRACK Unit State
 *
 * @details
 *  The structure describes GTRACK unit states
 */
typedef enum {
    /**  @brief Free (not allocated) */
    TRACK_STATE_FREE = 0,
    /**  @brief INIT */
    TRACK_STATE_INIT,
    /**  @brief DETECTION State */
    TRACK_STATE_DETECTION,
    /**  @brief ACTIVE State */
    TRACK_STATE_ACTIVE
} TrackState;

/**
 * @brief 
 *  GTRACK Unit Velocity Handling State
 *
 * @details
 *  The structure describes GTRACK velocity handling states
 */
typedef enum {
    /**  @brief INIT */
    VELOCITY_INIT = 0,
    /**  @brief Range Rate filtering */
    VELOCITY_RATE_FILTER,
    /**  @brief Stabilizing Velocity Estimation */
    VELOCITY_TRACKING,
    /**  @brief Locked */
    VELOCITY_LOCKED
} VelocityHandlingState;


/** @addtogroup GTRACK_ALG_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief 
 *  GTRACK Unit instance structure
 *
 * @details
 *  The structure describes the individual GTRACK unit instance
 */

typedef struct
{
    /**  @brief Tracking Unit identifier */
    uint8_t	    uid;
    /**  @brief Target identifier */
    uint32_t	tid;

    /**  @brief TimeStamp */
    uint64_t	heartBeatCount;
    /**  @brief Allocation Time */
    uint64_t	allocationTime;
    /**  @brief Allocation Range */
	float		allocationRange;
    /**  @brief Allocation Radial Velocity */
	float		allocationVelocity;
    /**  @brief Estimated Number of Points */
    float		estNumOfPoints;

    /**  @brief Current State */
    TrackState	state;
    /**  @brief Target confidence Level */
    float       confidenceLevel;
    /**  @brief Confidence Level threshold to determine sleep condition limit */
    float       confidenceLevelThreshold;
    /**  @brief Indicates whether to ignore height dimension during 3D association or not (used in 3D ceiling mount configuration) */
    bool        isAssociationHeightIgnore;
    /**  @brief Indicates whether we mark ghosts behind the target (used in 2D case) */
    bool        isAssociationGhostMarking;
    /**  @brief Indicates whether we estimate expected number of points */
    bool        isEnablePointNumberEstimation;

    /**  @brief Indicates whether target is currently static */
    bool        isTargetStatic;
    /**  @brief Skip Prediction (when not enough reliable reflection points) */
    bool        skipPrediction;
    /**  @brief Skip Process Noise estimation (when not enough reliable reflection points) */
    bool        skipProcessNoiseEst;
    /**  @brief Indicates whether SNR-based weighting or simple averaging is used to compute centroid center */
    bool        isSnrWeighting;

    /**  @brief Requested State Vector type */
	GTRACK_STATE_VECTOR_TYPE	stateVectorType;
    /**  @brief Current State Vector type */
	GTRACK_STATE_VECTOR_TYPE	currentStateVectorType;
    /**  @brief Number of Dimensions in State Vector, ex. 2 for constant velocity, 3 for constnat acceleration */
    uint16_t	stateVectorDimNum;
    /**  @brief Length of each dimensions in State Vector, ex. 2 for XY, 3 for XYZ */
    uint16_t	stateVectorDimLength;
    /**  @brief Length of State Vector */
    uint16_t	stateVectorLength;
    /**  @brief Length of Measurement Vector */
    uint16_t	measurementVectorLength;
    /**  @brief Veboseness Mask */
	uint64_t	verbose;    

    /**  @brief Gating Parameters */
	GTRACK_gatingParams		*gatingParams;
    /**  @brief State Changing Parameters */
	GTRACK_stateParams	        *stateParams;
    /**  @brief Allocation Parameters */
	GTRACK_allocationParams	*allocationParams;
    /**  @brief Unrolling Parameters */
	GTRACK_unrollingParams		*unrollingParams;
    /**  @brief Scenery Parameters */
    GTRACK_sceneryParams       *sceneryParams;
    /**  @brief Presence Parameters Parameters */
    GTRACK_presenceParams       *presenceParams;
    /**  @brief  Transformation to world coordinates */
    GTRACK_worldTransformParams   *transormParams;
	
    /**  @brief Current velocity handling State */
	VelocityHandlingState	velocityHandling;
	
    /**  @brief Expected target radial velocity at the moment of detection, m/s */
    float       initialRadialVelocity;	
    /**  @brief Absolute value of maximum radial velocity measure by the sensor, m/s */
	float		maxRadialVelocity;
    /**  @brief Radial velocity resolution of the sensor, m/s */
	float		radialVelocityResolution;
    /**  @brief Current Range Rate value*/
    float		rangeRate;

    /**  @brief when no points available: velocity threshold to transition to static state */
    float		minVelocityStopNoPoints;
    /**  @brief when no dynamic points available: velocity threshold to transition to static state */
    float		minVelocityStopNoDyn;
    /**  @brief when no dynamic points availabe: minimal velocity threshold to start slowing down */
    float		minStaticVelocitySlow;
    /**  @brief maximum mean velocity of the dynamic points to transition to dynamic state */
    float		maxVelocityMoveToDynamic;
    /**  @brief filter coefficient for spread estimation */
    float		estSpreadAlpha;
    /**  @brief Number of assocuated dynamic points */
    uint32_t    numAssosiatedPoints;

    /**  @brief Detection state count to active */
    uint16_t	detect2activeCount;
    /**  @brief Detection state count to free */
    uint16_t	detect2freeCount;
    /**  @brief Active state count to free */
    uint16_t	active2freeCount;
    /**  @brief Active state static condition count to free */
    uint16_t	sleep2freeCount;
    /**  @brief Outside boundary box count to free */
    uint16_t	outside2freeCount;
    /**  @brief Maintains a running average (based on alpha filtering) of the number of static points */
    uint16_t	numStaticPtsHistory;

	/**  @brief Configured target maximum acceleration */
	float		maxAcceleration[3];
	/**  @brief Configured Frame rate */
    float		dt;

	/**  @brief Pointer to current Transition matrix */
    float		*F;
	/**  @brief Pointer to current Process Noise matrix */
    float		*Q;

	/**  @brief State matrix, estimated */
    float		S_hat[GTRACK_STATE_VECTOR_SIZE];
	/**  @brief State matrix, predicted */
    float		S_apriori_hat[GTRACK_STATE_VECTOR_SIZE];
	/**  @brief Process matrix, estimated */
    float		P_hat[GTRACK_STATE_VECTOR_SIZE*GTRACK_STATE_VECTOR_SIZE];
	/**  @brief Process matrix, predicted */
    float		P_apriori_hat[GTRACK_STATE_VECTOR_SIZE*GTRACK_STATE_VECTOR_SIZE];
	/**  @brief Expected Measurement matrix */
    GTRACK_measurementUnion H_s;
	/**  @brief Calculated measurement centroid */
    GTRACK_measurementUnion uCenter;
	/**  @brief Calculated measurement centroid */
    GTRACK_cartesian_position uPos;
	/**  @brief Limits for associated measurments */
    GTRACK_measurementUnion H_limits;
	/**  @brief Estimated spread of the measurements */
    GTRACK_measurementUnion estSpread;
	/**  @brief Estimated physical dimensions of the target */
    float       estDim[GTRACK_MEASUREMENT_VECTOR_SIZE];
	/**  @brief Group Dispersion matrix */
    float		gD[GTRACK_MEASUREMENT_VECTOR_SIZE*GTRACK_MEASUREMENT_VECTOR_SIZE];
	/**  @brief Group Member Covariance matrix (between a member in measurment group and group centroid) */
    float		gC[GTRACK_MEASUREMENT_VECTOR_SIZE*GTRACK_MEASUREMENT_VECTOR_SIZE];
	/**  @brief Inverse of Group Covariance matrix */
    float		gC_inv[GTRACK_MEASUREMENT_VECTOR_SIZE*GTRACK_MEASUREMENT_VECTOR_SIZE];
	/**  @brief DEBUG, previous tick ec to report */
    float		ec[GTRACK_MEASUREMENT_VECTOR_SIZE*GTRACK_MEASUREMENT_VECTOR_SIZE];
    /**  @brief determinant of Group Covariance matrix */
	float		gC_det;
	/**  @brief Gain used in association function */
	float		G;

} GtrackUnitInstance;

/**
 * @brief 
 *  GTRACK Module instance structure
 *
 * @details
 *  The structure describes the GTRACK module instance
 */
typedef struct {

	/**  @brief Maximum number of measurement points per frame */
	uint16_t maxNumPoints;
	/**  @brief Maximum number of Tracking objects */
	uint16_t maxNumTracks;
	/**  @brief Tracking Unit Parameters  */
	TrackingParams params;

	/**  @brief TimeStamp  */
	uint64_t	heartBeat;

    /**  @brief verboseness Mask */
	uint64_t	verbose;
    
    /**  @brief ceiling mount option */
    bool        isCeilingMounted;

    /**  @brief boresight detection removal option */
    bool        isBoresightFilteringEnabled;

	/**  @brief Array of best scores     */
	float		*bestScore;
	/**  @brief Array of best score authors (UIDs)  */
	uint8_t	    *bestIndex;
	/**  @brief Bit Array of indicators whether best score author is unique */
	uint8_t	    *isUniqueIndex;
    /**  @brief Bit Array of indicators whether measurement point is associated with the static track */
	uint8_t	    *isStaticIndex;
	/**  @brief Temporary Array of measurement indices for current set-under-construction */
	uint16_t	*allocIndexCurrent;
	/**  @brief Temporary Array of measurement indices for stored set-under-construction */
	uint16_t	*allocIndexStored;
	/**  @brief Array of Tracking Unit handles */
	void		**hTrack;
	/**  @brief List of currently active Tracking Units (UIDs) */
	GTrack_ListObj activeList;
	/**  @brief List of Free Tracking Units (UIDs) */
	GTrack_ListObj freeList;
	/**  @brief Array of UID elements */
	GTrack_ListElem *uidElem;

	/**  @brief Array of Target descriptors */
	GTRACK_targetDesc	*targetDesc;
    /**  @brief Total number of tracked targets */
    uint32_t	targetNumTotal;
	/**  @brief Number of currently tracked Targets */
	uint16_t    targetNumCurrent;

	/**  @brief presence detection enable */
    bool        isPresenceDetectionEnabled;
	/**  @brief presence detection indication based on existing targets within the ocupance box */
    bool        presenceDetectionRaw;
	/**  @brief The flag to indicate presence detection power up condition (no prior knowledge available) */
    bool        isPresenceDetectionInitial;
	/**  @brief The counter for transaction presence ON => presence 0FF */
    uint16_t    presenceDetectionCounter;
	/**  @brief Presence detection output */
    uint8_t     presenceDetectionOutput;

} GtrackModuleInstance;

/**
 @} */

void gtrack_modulePredict(GtrackModuleInstance *inst);
void gtrack_moduleAssociate(GtrackModuleInstance *inst, GTRACK_measurementPoint *point, uint16_t num);
void gtrack_moduleAllocate(GtrackModuleInstance *inst, GTRACK_measurementPoint *point, uint16_t num);
void gtrack_moduleUpdate(GtrackModuleInstance *inst, GTRACK_measurementPoint *point, GTRACK_measurement_vector *var, uint16_t num);
void gtrack_moduleReport(GtrackModuleInstance *inst, GTRACK_targetDesc *t, uint16_t *tNum);
void gtrack_modulePresence(GtrackModuleInstance *inst, uint8_t *presence);

void *gtrack_unitCreate(TrackingParams *params, int32_t *errCode);
void gtrack_unitDelete(void *handle);
void gtrack_unitStart(void *handle, uint64_t timeStamp, uint32_t tid, GTRACK_measurement_vector *u, uint16_t numPoints, bool isBehind);
void gtrack_unitStop(void *handle);
void gtrack_unitPredict(void *handle);
TrackState gtrack_unitUpdate(void *handle, GTRACK_measurementPoint *point, GTRACK_measurement_vector *var, uint8_t *pInd, uint8_t * isUnique, uint16_t num);
void gtrack_unitScore(void *handle, GTRACK_measurementPoint *point, float *bestScore, uint8_t *bestInd, uint8_t *isUnique, uint8_t *isStatic, uint16_t num);
void gtrack_unitEvent(void *handle, uint16_t num, uint16_t numReliable, uint16_t numDynamic);
void gtrack_unitReport(void *handle, GTRACK_targetDesc *target);
void gtrack_unitGetH(void *handle, float *H);
void gtrack_unitGetC(void *handle, float *C);
void gtrack_unitGetSpread(void *handle, float *S);
void gtrack_unitGetMCenter(void *handle, float *pPos);

void gtrack_velocityStateHandling(void *handle, GTRACK_measurement_vector *v);
void gtrack_setTargetStatic(void *handle);
float gtrack_unrollRadialVelocity(float rvMax, float rvExp, float rvIn);
void gtrack_spherical2cartesian(GTRACK_STATE_VECTOR_TYPE format, float *sph, float *cart);
void gtrack_cartesian2spherical(GTRACK_STATE_VECTOR_TYPE format, float *cart, float *sph);
void gtrack_computeJacobian(GTRACK_STATE_VECTOR_TYPE format, float *cart, float *jac);
float gtrack_gateCreateLim(float volume, float *EC, float range, float *gateLim);
float gtrack_gateGainLim(float gain, float *EC, float range, float *gateLim);


void gtrack_sph2cart(GTRACK_measurement_vector *v, GTRACK_cartesian_position *c);
void gtrack_cart2sph(GTRACK_cartesian_position *c, GTRACK_measurement_vector *v);
void gtrack_censor2world(GTRACK_cartesian_position *c_in, GTRACK_worldTransformParams *wt, GTRACK_cartesian_position *c_out);

float gtrack_calcDistance(GTRACK_measurement_vector *v1, GTRACK_measurement_vector *v2);
void gtrack_calcDim(float *mSpread, float R, float *tDim);
void gtrack_calcMeasurementSpread(float range, GTRACK_gateLimits *gate_limits, GTRACK_measurement_vector *estSpread);
void gtrack_calcMeasurementLimits(float range, GTRACK_gateLimits *gate_limits, GTRACK_measurement_vector *limits);

uint8_t gtrack_isPointBehindTarget(GTRACK_measurement_vector *v, GTRACK_measurement_vector *hs, GTRACK_measurement_vector *spread);
uint8_t gtrack_isPointInsideBox(GTRACK_cartesian_position *c, GTRACK_boundaryBox *box);
uint8_t gtrack_isInsideSolidAngle(GTRACK_measurement_vector *aError, GTRACK_measurement_vector *spread);
uint8_t gtrack_isInsideBoresightStaticZone(GTRACK_measurement_vector *v);

void gtrack_sincosd(float theta, float * pSinVal, float * pCosVal);
void gtrack_calcRotMatZX(float azimTiltAngle, float elevTiltAngle, MATRIX3x3 *rotZX);

void gtrack_matrixInit(uint16_t rows, uint16_t cols, float value, float *A);
void gtrack_matrixEye(uint16_t size, float *A);
void gtrack_matrixSetDiag(uint16_t size, float *v, float *A);
void gtrack_matrixGetDiag(uint16_t size, float *A, float *v);

void gtrack_vectorInit(uint16_t size, float value, float *A);
void gtrack_vectorAdd(uint16_t size, float *A, float *B, float *C);
void gtrack_vectorSub(uint16_t size, float *A, float *B, float *C);
void gtrack_vectorScalarMul(uint16_t size, float *A, float c, float *B);
void gtrack_vectorScalarMulAcc(uint16_t size, float *A, float c, float *B);
void gtrack_vectorFilter(uint16_t size, float *A, float alpha, float *B);

void gtrack_matrixCovAcc(uint16_t size, float *A, float *v, float *mean);
void gtrack_matrixCovNormalize(uint16_t size, float *A, uint16_t num);
void gtrack_matrixCovFilter(uint16_t size, float *A, float *B, float alpha);

void gtrack_matrixMultiply(uint16_t m1, uint16_t m2, uint16_t m3, float *A, float *B, float *C);
void gtrack_matrixComputePJT(float *P, float *J, float *PJ);
void gtrack_matrixTransposeMultiply(uint16_t rows, uint16_t m, uint16_t cols, float *A, float *B, float *C);
void gtrack_matrixScalarMultiply(uint16_t m1, uint16_t m2, float *A, float c, float *B);
void gtrack_matrixAdd(uint16_t m1, uint16_t m2, float *A, float *B, float *C);
void gtrack_matrixSub(uint16_t m1, uint16_t m2, float *A, float *B, float *C);
void gtrack_matrixInv(const float *A, float *det, float *Ainv);
void gtrack_matrixMakeSymmetrical(uint16_t m, float *A, float *B);
void gtrack_matrixCholesky(uint16_t dim, const float *A, float *G);
void gtrack_computeMahalanobis(float *d, float *S, float *md);
void gtrack_computeMahalanobisPartial(float *d, float *S, float *mdp);

void gtrack_matrixPrint(uint16_t m1, uint16_t m2, float *A);
void gtrack_matrixPrint2(uint16_t rows, uint16_t cols, float *A, float *B);

#ifdef __cplusplus
}
#endif

#endif /* GTRACK_INT_LIB_H__ */
