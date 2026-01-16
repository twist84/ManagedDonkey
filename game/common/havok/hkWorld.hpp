#pragma once

#include "hkArray.hpp"
#include "hkBase.hpp"
#include "hkMath.hpp"

enum hkStepResult
{
	HK_STEP_RESULT_SUCCESS = 0x0,
	HK_STEP_RESULT_MEMORY_FAILURE_BEFORE_INTEGRATION = 0x1,
	HK_STEP_RESULT_MEMORY_FAILURE_DURING_COLLIDE = 0x2,
	HK_STEP_RESULT_MEMORY_FAILURE_DURING_TOI_SOLVE = 0x3,
};

struct hkSimulation;
struct hkSimulationIsland;
struct hkRigidBody;
struct hkWorldMaintenanceMgr;
struct hkWorldMemoryWatchDog;
struct hkBroadPhase;
struct hkTypedBroadPhaseDispatcher;
struct hkPhantomBroadPhaseListener;
struct hkEntityEntityBroadPhaseListener;
struct hkBroadPhaseBorderListener;
struct hkProcessCollisionInput;
struct hkCollisionFilter;
struct hkCollisionDispatcher;
struct hkConvexListFilter;
struct hkWorldOperationQueue;
struct hkDebugInfoOnPendingOperationQueues;
struct hkCriticalSection;
struct hkPhantom;
struct hkActionListener;
struct hkEntity;
struct hkBroadPhaseBorder;

struct hkMultiThreadLock
{
	unsigned int m_threadId;
	int16 m_lockCount;
	uns16 m_lockBitStack;
};

struct hkStepInfo
{
	hkPadSpu<float> m_startTime;
	hkPadSpu<float> m_endTime;
	hkPadSpu<float> m_deltaTime;
	hkPadSpu<float> m_invDeltaTime;
};

struct hkSolverInfo
{
	struct DeactivationInfo
	{
		float m_linearVelocityThresholdInv;
		float m_angularVelocityThresholdInv;
		float m_slowObjectVelocityMultiplier;
		float m_relativeSleepVelocityThreshold;
		uns16 m_stepsToDeactivate;
		float m_maxDistSqrd[2];
		hkHalf m_maxRotSqrd[2];
	};

	float m_one;
	float m_tau;
	float m_damping;
	float m_frictionTau;
	hkVector4 m_globalAccelerationPerSubStep;
	hkVector4 m_globalAccelerationPerStep;
	hkVector4 m_integrateVelocityFactor;
	hkVector4 m_invIntegrateVelocityFactor;
	float m_dampDivTau;
	float m_tauDivDamp;
	float m_dampDivFrictionTau;
	float m_frictionTauDivDamp;
	float m_contactRestingVelocity;
	hkSolverInfo::DeactivationInfo m_deactivationInfo[6];
	float m_deltaTime;
	float m_invDeltaTime;
	int32 m_numSteps;
	int32 m_numMicroSteps;
	float m_invNumMicroSteps;
	float m_invNumSteps;
	hkBool m_forceCoherentConstraintOrderingInSolver;
	unsigned char m_deactivationNumInactiveFramesSelectFlag[2];
	unsigned char m_deactivationIntegrateCounter;
};

struct hkWorldDynamicsStepInfo
{
	hkStepInfo m_stepInfo;
	hkSolverInfo m_solverInfo;
};

struct hkWorld :
	public hkReferencedObject
{
public:
	hkStepResult __thiscall stepDeltaTime(float physicsDeltaTime);

	struct PropertyLock
	{
		unsigned int m_key;
		hkMultiThreadLock m_multiThreadLock;
	};

	hkSimulation* m_simulation;
	hkVector4 m_gravity;
	hkSimulationIsland* m_fixedIsland;
	hkRigidBody* m_fixedRigidBody;
	hkArray<hkSimulationIsland> m_activeSimulationIslands;
	hkArray<hkSimulationIsland> m_inactiveSimulationIslands;
	hkArray<hkSimulationIsland> m_dirtySimulationIslands;
	hkWorldMaintenanceMgr* m_maintenanceMgr;
	hkWorldMemoryWatchDog* m_memoryWatchDog;
	hkBroadPhase* m_broadPhase;
	hkTypedBroadPhaseDispatcher* m_broadPhaseDispatcher;
	hkPhantomBroadPhaseListener* m_phantomBroadPhaseListener;
	hkEntityEntityBroadPhaseListener* m_entityEntityBroadPhaseListener;
	hkBroadPhaseBorderListener* m_broadPhaseBorderListener;
	hkProcessCollisionInput* m_collisionInput;
	hkCollisionFilter* m_collisionFilter;
	hkCollisionDispatcher* m_collisionDispatcher;
	hkConvexListFilter* m_convexListFilter;
	hkWorldOperationQueue* m_pendingOperations;
	int32 m_pendingOperationsCount;
	int32 m_criticalOperationsLockCount;
	int32 m_criticalOperationsLockCountForPhantoms;
	hkBool m_blockExecutingPendingOperations;
	hkBool m_criticalOperationsAllowed;
	hkDebugInfoOnPendingOperationQueues* m_pendingOperationQueues;
	int32 m_pendingOperationQueueCount;
	hkMultiThreadLock m_multiThreadLock;
	hkBool m_processActionsInSingleThread;
	unsigned int m_minDesiredIslandSize;
	hkCriticalSection* m_modifyConstraintCriticalSection;
	hkCriticalSection* m_worldLock;
	hkCriticalSection* m_islandDirtyListCriticalSection;
	hkCriticalSection* m_propertyMasterLock;
	hkArray<PropertyLock> m_propertyLocks;
	hkBool m_wantSimulationIslands;
	float m_snapCollisionToConvexEdgeThreshold;
	float m_snapCollisionToConcaveEdgeThreshold;
	hkBool m_enableToiWeldRejection;
	hkBool m_wantDeactivation;
	hkBool m_wantOldStyleDeactivation;
	hkBool m_shouldActivateOnRigidBodyTransformChange;
	float m_highFrequencyDeactivationPeriod;
	float m_lowFrequencyDeactivationPeriod;
	float m_deactivationReferenceDistance;
	float m_toiCollisionResponseRotateNormal;
	int32 m_simulationType;
	unsigned int m_lastEntityUid;
	hkArray<hkPhantom> m_phantoms;
	hkArray<hkActionListener> m_actionListeners;
	hkArray<hkEntity> m_entityListeners;
	unsigned char m_phantomListeners[12];
	unsigned char m_constraintListeners[12];
	unsigned char m_worldDeletionListeners[12];
	unsigned char m_islandActivationListeners[12];
	unsigned char m_worldPostSimulationListeners[12];
	unsigned char m_worldPostIntegrateListeners[12];
	unsigned char m_worldPostCollideListeners[12];
	unsigned char m_islandPostIntegrateListeners[12];
	unsigned char m_islandPostCollideListeners[12];
	unsigned char m_collisionListeners[12];
	hkBroadPhaseBorder* m_broadPhaseBorder;
	hkWorldDynamicsStepInfo m_dynamicsStepInfo;
	hkVector4 m_broadPhaseExtents[2];
	int32 m_broadPhaseNumMarkers;
	int32 m_sizeOfToiEventQueue;
	int32 m_broadPhaseQuerySize;
	int32 m_broadPhaseUpdateSize;
	signed char m_contactPointGeneration;
};
COMPILE_ASSERT(sizeof(hkWorld) == 0x320);
COMPILE_ASSERT(0x008 == __builtin_offsetof(hkWorld, m_simulation));
COMPILE_ASSERT(0x010 == __builtin_offsetof(hkWorld, m_gravity));
COMPILE_ASSERT(0x020 == __builtin_offsetof(hkWorld, m_fixedIsland));
COMPILE_ASSERT(0x024 == __builtin_offsetof(hkWorld, m_fixedRigidBody));
COMPILE_ASSERT(0x028 == __builtin_offsetof(hkWorld, m_activeSimulationIslands));
COMPILE_ASSERT(0x034 == __builtin_offsetof(hkWorld, m_inactiveSimulationIslands));
COMPILE_ASSERT(0x040 == __builtin_offsetof(hkWorld, m_dirtySimulationIslands));
COMPILE_ASSERT(0x04C == __builtin_offsetof(hkWorld, m_maintenanceMgr));
COMPILE_ASSERT(0x050 == __builtin_offsetof(hkWorld, m_memoryWatchDog));
COMPILE_ASSERT(0x054 == __builtin_offsetof(hkWorld, m_broadPhase));
COMPILE_ASSERT(0x058 == __builtin_offsetof(hkWorld, m_broadPhaseDispatcher));
COMPILE_ASSERT(0x05C == __builtin_offsetof(hkWorld, m_phantomBroadPhaseListener));
COMPILE_ASSERT(0x060 == __builtin_offsetof(hkWorld, m_entityEntityBroadPhaseListener));
COMPILE_ASSERT(0x064 == __builtin_offsetof(hkWorld, m_broadPhaseBorderListener));
COMPILE_ASSERT(0x068 == __builtin_offsetof(hkWorld, m_collisionInput));
COMPILE_ASSERT(0x06C == __builtin_offsetof(hkWorld, m_collisionFilter));
COMPILE_ASSERT(0x070 == __builtin_offsetof(hkWorld, m_collisionDispatcher));
COMPILE_ASSERT(0x074 == __builtin_offsetof(hkWorld, m_convexListFilter));
COMPILE_ASSERT(0x078 == __builtin_offsetof(hkWorld, m_pendingOperations));
COMPILE_ASSERT(0x07C == __builtin_offsetof(hkWorld, m_pendingOperationsCount));
COMPILE_ASSERT(0x080 == __builtin_offsetof(hkWorld, m_criticalOperationsLockCount));
COMPILE_ASSERT(0x084 == __builtin_offsetof(hkWorld, m_criticalOperationsLockCountForPhantoms));
COMPILE_ASSERT(0x088 == __builtin_offsetof(hkWorld, m_blockExecutingPendingOperations));
COMPILE_ASSERT(0x089 == __builtin_offsetof(hkWorld, m_criticalOperationsAllowed));
COMPILE_ASSERT(0x08C == __builtin_offsetof(hkWorld, m_pendingOperationQueues));
COMPILE_ASSERT(0x090 == __builtin_offsetof(hkWorld, m_pendingOperationQueueCount));
COMPILE_ASSERT(0x094 == __builtin_offsetof(hkWorld, m_multiThreadLock));
COMPILE_ASSERT(0x09C == __builtin_offsetof(hkWorld, m_processActionsInSingleThread));
COMPILE_ASSERT(0x0A0 == __builtin_offsetof(hkWorld, m_minDesiredIslandSize));
COMPILE_ASSERT(0x0A4 == __builtin_offsetof(hkWorld, m_modifyConstraintCriticalSection));
COMPILE_ASSERT(0x0A8 == __builtin_offsetof(hkWorld, m_worldLock));
COMPILE_ASSERT(0x0AC == __builtin_offsetof(hkWorld, m_islandDirtyListCriticalSection));
COMPILE_ASSERT(0x0B0 == __builtin_offsetof(hkWorld, m_propertyMasterLock));
COMPILE_ASSERT(0x0B4 == __builtin_offsetof(hkWorld, m_propertyLocks));
COMPILE_ASSERT(0x0C0 == __builtin_offsetof(hkWorld, m_wantSimulationIslands));
COMPILE_ASSERT(0x0C4 == __builtin_offsetof(hkWorld, m_snapCollisionToConvexEdgeThreshold));
COMPILE_ASSERT(0x0C8 == __builtin_offsetof(hkWorld, m_snapCollisionToConcaveEdgeThreshold));
COMPILE_ASSERT(0x0CC == __builtin_offsetof(hkWorld, m_enableToiWeldRejection));
COMPILE_ASSERT(0x0CD == __builtin_offsetof(hkWorld, m_wantDeactivation));
COMPILE_ASSERT(0x0CE == __builtin_offsetof(hkWorld, m_wantOldStyleDeactivation));
COMPILE_ASSERT(0x0CF == __builtin_offsetof(hkWorld, m_shouldActivateOnRigidBodyTransformChange));
COMPILE_ASSERT(0x0D0 == __builtin_offsetof(hkWorld, m_highFrequencyDeactivationPeriod));
COMPILE_ASSERT(0x0D4 == __builtin_offsetof(hkWorld, m_lowFrequencyDeactivationPeriod));
COMPILE_ASSERT(0x0D8 == __builtin_offsetof(hkWorld, m_deactivationReferenceDistance));
COMPILE_ASSERT(0x0DC == __builtin_offsetof(hkWorld, m_toiCollisionResponseRotateNormal));
COMPILE_ASSERT(0x0E0 == __builtin_offsetof(hkWorld, m_simulationType));
COMPILE_ASSERT(0x0E4 == __builtin_offsetof(hkWorld, m_lastEntityUid));
COMPILE_ASSERT(0x0E8 == __builtin_offsetof(hkWorld, m_phantoms));
COMPILE_ASSERT(0x0F4 == __builtin_offsetof(hkWorld, m_actionListeners));
COMPILE_ASSERT(0x100 == __builtin_offsetof(hkWorld, m_entityListeners));
COMPILE_ASSERT(0x10C == __builtin_offsetof(hkWorld, m_phantomListeners));
COMPILE_ASSERT(0x118 == __builtin_offsetof(hkWorld, m_constraintListeners));
COMPILE_ASSERT(0x124 == __builtin_offsetof(hkWorld, m_worldDeletionListeners));
COMPILE_ASSERT(0x130 == __builtin_offsetof(hkWorld, m_islandActivationListeners));
COMPILE_ASSERT(0x13C == __builtin_offsetof(hkWorld, m_worldPostSimulationListeners));
COMPILE_ASSERT(0x148 == __builtin_offsetof(hkWorld, m_worldPostIntegrateListeners));
COMPILE_ASSERT(0x154 == __builtin_offsetof(hkWorld, m_worldPostCollideListeners));
COMPILE_ASSERT(0x160 == __builtin_offsetof(hkWorld, m_islandPostIntegrateListeners));
COMPILE_ASSERT(0x16C == __builtin_offsetof(hkWorld, m_islandPostCollideListeners));
COMPILE_ASSERT(0x178 == __builtin_offsetof(hkWorld, m_collisionListeners));
COMPILE_ASSERT(0x184 == __builtin_offsetof(hkWorld, m_broadPhaseBorder));
COMPILE_ASSERT(0x190 == __builtin_offsetof(hkWorld, m_dynamicsStepInfo));
COMPILE_ASSERT(0x2E0 == __builtin_offsetof(hkWorld, m_broadPhaseExtents));
COMPILE_ASSERT(0x300 == __builtin_offsetof(hkWorld, m_broadPhaseNumMarkers));
COMPILE_ASSERT(0x304 == __builtin_offsetof(hkWorld, m_sizeOfToiEventQueue));
COMPILE_ASSERT(0x308 == __builtin_offsetof(hkWorld, m_broadPhaseQuerySize));
COMPILE_ASSERT(0x30C == __builtin_offsetof(hkWorld, m_broadPhaseUpdateSize));
COMPILE_ASSERT(0x310 == __builtin_offsetof(hkWorld, m_contactPointGeneration));

struct hkSimulation :
	public hkReferencedObject
{
	enum LastProcessingStep
	{
		INTEGRATE = 0,
		COLLIDE = 1,
	};
	hkWorld* m_world;
	LastProcessingStep m_lastProcessingStep;
	float m_currentTime;
	float m_currentPsiTime;
	float m_physicsDeltaTime;
	float m_simulateUntilTime;
	float m_frameMarkerPsiSnap;
	hkStepResult m_previousStepResult;
};
COMPILE_ASSERT(sizeof(hkSimulation) == 0x28);

struct hkConstraintInfo
{
	int32 m_maxSizeOfSchema;
	int32 m_sizeOfSchemas;
	int32 m_numSolverResults;
	int32 m_numSolverElemTemps;
};
COMPILE_ASSERT(sizeof(hkConstraintInfo) == 0x10);

struct hkConstraintOwner :
	public hkReferencedObject
{
	hkConstraintInfo m_constraintInfo;
};
COMPILE_ASSERT(sizeof(hkConstraintOwner) == 0x18);

struct hkAction :
	public hkReferencedObject
{
	hkWorld* m_world;
	hkSimulationIsland* m_island;
	unsigned int m_userData;
	const char* m_name;
};
COMPILE_ASSERT(sizeof(hkAction) == 0x18);

struct hkAgentNnSector
{
	char m_data[512];
};
COMPILE_ASSERT(sizeof(hkAgentNnSector) == 0x200);

struct hkAgentNnTrack
{
	unsigned int m_bytesUsedInLastSector;
	hkInplaceArray<hkAgentNnSector*, 1> m_sectors;
};

struct hkSimulationIsland :
	public hkConstraintOwner
{
	hkWorld* m_world;
	int32 m_numConstraints;
	uns16 m_storageIndex;
	uns16 m_dirtyListIndex;
	unsigned char m_splitCheckFrameCounter;
	unsigned char m_highFrequencyDeactivationCounter;
	unsigned char m_lowFrequencyDeactivationCounter;
	char m_splitCheckRequested : 2;
	char m_sparseEnabled : 2;
	char m_actionListCleanupNeeded : 2;
	char m_allowIslandLocking : 2;
	char m_isInActiveIslandsArray : 2;
	char m_active : 2;
	char m_inIntegrateJob : 2;
	hkMultiThreadLock m_multiThreadLock;
	float m_timeSinceLastHighFrequencyCheck;
	float m_timeSinceLastLowFrequencyCheck;
	hkArray<hkAction*> m_actions;
	float m_timeOfDeactivation;
	hkInplaceArray<hkEntity*, 1> m_entities;
	hkAgentNnTrack m_agentTrack;
};
COMPILE_ASSERT(sizeof(hkSimulationIsland) == 0x70);
COMPILE_ASSERT(0x18 == __builtin_offsetof(hkSimulationIsland, m_world));
COMPILE_ASSERT(0x1C == __builtin_offsetof(hkSimulationIsland, m_numConstraints));
COMPILE_ASSERT(0x20 == __builtin_offsetof(hkSimulationIsland, m_storageIndex));
COMPILE_ASSERT(0x22 == __builtin_offsetof(hkSimulationIsland, m_dirtyListIndex));
COMPILE_ASSERT(0x24 == __builtin_offsetof(hkSimulationIsland, m_splitCheckFrameCounter));
COMPILE_ASSERT(0x25 == __builtin_offsetof(hkSimulationIsland, m_highFrequencyDeactivationCounter));
COMPILE_ASSERT(0x26 == __builtin_offsetof(hkSimulationIsland, m_lowFrequencyDeactivationCounter));
//COMPILE_ASSERT(0x27 == __builtin_offsetof(hkSimulationIsland, m_splitCheckRequested));
//COMPILE_ASSERT(0x27 == __builtin_offsetof(hkSimulationIsland, m_sparseEnabled));
//COMPILE_ASSERT(0x27 == __builtin_offsetof(hkSimulationIsland, m_actionListCleanupNeeded));
//COMPILE_ASSERT(0x27 == __builtin_offsetof(hkSimulationIsland, m_allowIslandLocking));
//COMPILE_ASSERT(0x28 == __builtin_offsetof(hkSimulationIsland, m_isInActiveIslandsArray));
//COMPILE_ASSERT(0x28 == __builtin_offsetof(hkSimulationIsland, m_active));
//COMPILE_ASSERT(0x28 == __builtin_offsetof(hkSimulationIsland, m_inIntegrateJob));
COMPILE_ASSERT(0x2C == __builtin_offsetof(hkSimulationIsland, m_multiThreadLock));
COMPILE_ASSERT(0x34 == __builtin_offsetof(hkSimulationIsland, m_timeSinceLastHighFrequencyCheck));
COMPILE_ASSERT(0x38 == __builtin_offsetof(hkSimulationIsland, m_timeSinceLastLowFrequencyCheck));
COMPILE_ASSERT(0x3C == __builtin_offsetof(hkSimulationIsland, m_actions));
COMPILE_ASSERT(0x48 == __builtin_offsetof(hkSimulationIsland, m_timeOfDeactivation));
COMPILE_ASSERT(0x4C == __builtin_offsetof(hkSimulationIsland, m_entities));
COMPILE_ASSERT(0x5C == __builtin_offsetof(hkSimulationIsland, m_agentTrack));

// $TODO find these a better home

struct hkShape;
struct hkCdBody
{
	const hkShape* m_shape;
	unsigned int m_shapeKey;
	const void* m_motion;
	const hkCdBody* m_parent;
};

struct hkBroadPhaseHandle
{
	unsigned int m_id;
};

struct hkTypedBroadPhaseHandle :
	public hkBroadPhaseHandle
{
	char m_type;
	char m_ownerOffset;
	unsigned __int16 m_objectQualityType;
	unsigned int m_collisionFilterInfo;
};

#pragma pack(push, 4)
struct hkCollidable :
	public hkCdBody
{
	char m_ownerOffset;
	unsigned __int8 m_forceCollideOntoPpu;
	unsigned __int16 m_shapeSizeOnSpu;
	hkTypedBroadPhaseHandle m_broadPhaseHandle;
	float m_allowedPenetrationDepth;
};
#pragma pack(pop)

struct hkAgentNnEntry;
struct hkLinkedCollidable :
	public hkCollidable
{
	struct CollisionEntry
	{
		hkAgentNnEntry* m_agentEntry;
		hkLinkedCollidable* m_partner;
	};

	hkArray<CollisionEntry> m_collisionEntries;
};

struct hkPropertyValue
{
	unsigned __int64 m_data;
};

struct hkProperty
{
	unsigned int m_key;
	unsigned int m_alignmentPadding;
	hkPropertyValue m_value;
};

struct hkWorldObject :
	public hkReferencedObject
{
	hkWorld* m_World;
	void* m_userData;
	hkLinkedCollidable m_collidable;
	hkMultiThreadLock m_multithreadLock;
	const char* m_name;
	hkArray<hkProperty> m_properties;
};

struct hkMaterial
{
	signed __int8 m_responseType;
	float m_friction;
	float m_restitution;
};

struct hkConstraintAtom
{
	unsigned __int16 m_type;
};

struct hkConstraintInstance;
struct __declspec(align(8)) hkConstraintInternal
{
	hkConstraintInstance* m_constraint;
	hkEntity* m_entities[2];
	hkConstraintAtom* m_atoms;
	unsigned __int16 m_atomsSize;
	unsigned __int8 m_callbackRequest;
	unsigned __int8 m_priority;
	unsigned __int16 m_sizeOfSchemas;
	unsigned __int16 m_numSolverResults;
	unsigned __int16 m_numSolverElemTemps;
	unsigned __int8 m_whoIsMaster;
	hkBool m_isNormalType;
	void* m_runtime;
	unsigned __int16 m_runtimeSize;
	unsigned __int16 m_slaveIndex;
};

template<typename t_type>
struct __declspec(align(4)) hkSmallArray
{
	t_type* m_data;
	unsigned __int16 m_size;
	unsigned __int16 m_capacityAndFlags;
};
struct hkEntityDeactivator :
	public hkReferencedObject
{
};

template<typename t_type, typename t_storage_type>
struct hkEnum
{
	t_storage_type m_storage;
};

struct hkPhantomOverlapListener
{
	void* __vftable /*VFT*/;
};

struct hkPhantomListener
{
	void* __vftable /*VFT*/;
};

struct hkPhantom :
	public hkWorldObject
{
	hkArray<hkPhantomOverlapListener> m_overlapListeners;
	hkArray<hkPhantomListener> m_phantomListeners;
};

struct hkSweptTransform
{
	hkVector4 m_centerOfMass0;
	hkVector4 m_centerOfMass1;
	hkQuaternion m_rotation0;
	hkQuaternion m_rotation1;
	hkVector4 m_centerOfMassLocal;
};

struct hkMotionState
{
	hkTransform m_transform;
	hkSweptTransform m_sweptTransform;
	hkVector4 m_deltaAngle;
	float m_objectRadius;
	float m_maxLinearVelocity;
	float m_maxAngularVelocity;
	float m_linearDamping;
	float m_angularDamping;
	unsigned __int16 m_deactivationClass;
	unsigned __int16 m_deactivationCounter;
	unsigned int m_deactivationRefOrientation[2];
};

struct hkShapePhantom :
	public hkPhantom
{
	hkMotionState m_motionState;
};

struct hkMotion : 
	public hkReferencedObject
{
	enum MotionType
	{
		MOTION_INVALID = 0,
		MOTION_DYNAMIC = 1,
		MOTION_SPHERE_INERTIA = 2,
		MOTION_STABILIZED_SPHERE_INERTIA = 3,
		MOTION_BOX_INERTIA = 4,
		MOTION_STABILIZED_BOX_INERTIA = 5,
		MOTION_KEYFRAMED = 6,
		MOTION_FIXED = 7,
		MOTION_THIN_BOX_INERTIA = 8,
		MOTION_MAX_ID = 9,
	};

	hkEnum<MotionType, unsigned char> m_type;
	unsigned __int8 m_deactivationIntegrateCounter;
	unsigned __int16 m_deactivationNumInactiveFrames[2];
	hkMotionState m_motionState;
	hkVector4 m_inertiaAndMassInv;
	hkVector4 m_linearVelocity;
	hkVector4 m_angularVelocity;
	hkVector4 m_deactivationRefPosition[2];
};

struct hkMaxSizeMotion;
struct hkKeyframedRigidMotion :
	public hkMotion
{
	hkMaxSizeMotion* m_savedMotion;
	int m_savedQualityTypeIndex;
};

struct hkMaxSizeMotion :
	public hkKeyframedRigidMotion
{
};

struct hkCollisionListener;
struct hkEntityActivationListener;
struct hkEntityListener;

struct hkEntity :
	public hkWorldObject
{
	hkBool isActive()
	{
		hkBool result = { false };
		if (m_simulationIsland)
		{
			result.m_bool = m_simulationIsland->m_active;
		}
		return result;
	}

	hkMaterial m_material;
	unsigned int m_solverData;
	unsigned __int16 m_storageIndex;
	unsigned __int16 m_processContactCallbackDelay;
	hkSmallArray<hkConstraintInternal> m_constraintsMaster;
	hkArray<hkConstraintInstance*> m_constraintsSlave;
	hkArray<unsigned char> m_constraintRuntime;
	hkEntityDeactivator* m_deactivator;
	hkSimulationIsland* m_simulationIsland;
	char m_autoRemoveLevel;
	unsigned int m_uid;
	__declspec(align(16)) hkMaxSizeMotion m_motion;
	hkSmallArray<hkCollisionListener*> m_collisionListeners;
	hkSmallArray<hkEntityActivationListener*> m_activationListeners;
	hkSmallArray<hkEntityListener*> m_entityListeners;
	hkSmallArray<hkAction*> m_actions;
	int m_spuCollisionCallback;
};
COMPILE_ASSERT(sizeof(hkEntity) == 0x200);
COMPILE_ASSERT(0x90 == __builtin_offsetof(hkEntity, m_simulationIsland));

struct hkCollidable;
struct hkSimpleShapePhantom :
	public hkShapePhantom
{
	struct hkCollisionDetail
	{
		hkCollidable* m_collidable;
	};

	hkArray<hkCollisionDetail> m_collisionDetails;
};

struct hkRigidBody :
	public hkEntity
{
};

