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
	uint16 m_lockBitStack;
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
		uint16 m_stepsToDeactivate;
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
static_assert(sizeof(hkWorld) == 0x320);
static_assert(0x008 == __builtin_offsetof(hkWorld, m_simulation));
static_assert(0x010 == __builtin_offsetof(hkWorld, m_gravity));
static_assert(0x020 == __builtin_offsetof(hkWorld, m_fixedIsland));
static_assert(0x024 == __builtin_offsetof(hkWorld, m_fixedRigidBody));
static_assert(0x028 == __builtin_offsetof(hkWorld, m_activeSimulationIslands));
static_assert(0x034 == __builtin_offsetof(hkWorld, m_inactiveSimulationIslands));
static_assert(0x040 == __builtin_offsetof(hkWorld, m_dirtySimulationIslands));
static_assert(0x04C == __builtin_offsetof(hkWorld, m_maintenanceMgr));
static_assert(0x050 == __builtin_offsetof(hkWorld, m_memoryWatchDog));
static_assert(0x054 == __builtin_offsetof(hkWorld, m_broadPhase));
static_assert(0x058 == __builtin_offsetof(hkWorld, m_broadPhaseDispatcher));
static_assert(0x05C == __builtin_offsetof(hkWorld, m_phantomBroadPhaseListener));
static_assert(0x060 == __builtin_offsetof(hkWorld, m_entityEntityBroadPhaseListener));
static_assert(0x064 == __builtin_offsetof(hkWorld, m_broadPhaseBorderListener));
static_assert(0x068 == __builtin_offsetof(hkWorld, m_collisionInput));
static_assert(0x06C == __builtin_offsetof(hkWorld, m_collisionFilter));
static_assert(0x070 == __builtin_offsetof(hkWorld, m_collisionDispatcher));
static_assert(0x074 == __builtin_offsetof(hkWorld, m_convexListFilter));
static_assert(0x078 == __builtin_offsetof(hkWorld, m_pendingOperations));
static_assert(0x07C == __builtin_offsetof(hkWorld, m_pendingOperationsCount));
static_assert(0x080 == __builtin_offsetof(hkWorld, m_criticalOperationsLockCount));
static_assert(0x084 == __builtin_offsetof(hkWorld, m_criticalOperationsLockCountForPhantoms));
static_assert(0x088 == __builtin_offsetof(hkWorld, m_blockExecutingPendingOperations));
static_assert(0x089 == __builtin_offsetof(hkWorld, m_criticalOperationsAllowed));
static_assert(0x08C == __builtin_offsetof(hkWorld, m_pendingOperationQueues));
static_assert(0x090 == __builtin_offsetof(hkWorld, m_pendingOperationQueueCount));
static_assert(0x094 == __builtin_offsetof(hkWorld, m_multiThreadLock));
static_assert(0x09C == __builtin_offsetof(hkWorld, m_processActionsInSingleThread));
static_assert(0x0A0 == __builtin_offsetof(hkWorld, m_minDesiredIslandSize));
static_assert(0x0A4 == __builtin_offsetof(hkWorld, m_modifyConstraintCriticalSection));
static_assert(0x0A8 == __builtin_offsetof(hkWorld, m_worldLock));
static_assert(0x0AC == __builtin_offsetof(hkWorld, m_islandDirtyListCriticalSection));
static_assert(0x0B0 == __builtin_offsetof(hkWorld, m_propertyMasterLock));
static_assert(0x0B4 == __builtin_offsetof(hkWorld, m_propertyLocks));
static_assert(0x0C0 == __builtin_offsetof(hkWorld, m_wantSimulationIslands));
static_assert(0x0C4 == __builtin_offsetof(hkWorld, m_snapCollisionToConvexEdgeThreshold));
static_assert(0x0C8 == __builtin_offsetof(hkWorld, m_snapCollisionToConcaveEdgeThreshold));
static_assert(0x0CC == __builtin_offsetof(hkWorld, m_enableToiWeldRejection));
static_assert(0x0CD == __builtin_offsetof(hkWorld, m_wantDeactivation));
static_assert(0x0CE == __builtin_offsetof(hkWorld, m_wantOldStyleDeactivation));
static_assert(0x0CF == __builtin_offsetof(hkWorld, m_shouldActivateOnRigidBodyTransformChange));
static_assert(0x0D0 == __builtin_offsetof(hkWorld, m_highFrequencyDeactivationPeriod));
static_assert(0x0D4 == __builtin_offsetof(hkWorld, m_lowFrequencyDeactivationPeriod));
static_assert(0x0D8 == __builtin_offsetof(hkWorld, m_deactivationReferenceDistance));
static_assert(0x0DC == __builtin_offsetof(hkWorld, m_toiCollisionResponseRotateNormal));
static_assert(0x0E0 == __builtin_offsetof(hkWorld, m_simulationType));
static_assert(0x0E4 == __builtin_offsetof(hkWorld, m_lastEntityUid));
static_assert(0x0E8 == __builtin_offsetof(hkWorld, m_phantoms));
static_assert(0x0F4 == __builtin_offsetof(hkWorld, m_actionListeners));
static_assert(0x100 == __builtin_offsetof(hkWorld, m_entityListeners));
static_assert(0x10C == __builtin_offsetof(hkWorld, m_phantomListeners));
static_assert(0x118 == __builtin_offsetof(hkWorld, m_constraintListeners));
static_assert(0x124 == __builtin_offsetof(hkWorld, m_worldDeletionListeners));
static_assert(0x130 == __builtin_offsetof(hkWorld, m_islandActivationListeners));
static_assert(0x13C == __builtin_offsetof(hkWorld, m_worldPostSimulationListeners));
static_assert(0x148 == __builtin_offsetof(hkWorld, m_worldPostIntegrateListeners));
static_assert(0x154 == __builtin_offsetof(hkWorld, m_worldPostCollideListeners));
static_assert(0x160 == __builtin_offsetof(hkWorld, m_islandPostIntegrateListeners));
static_assert(0x16C == __builtin_offsetof(hkWorld, m_islandPostCollideListeners));
static_assert(0x178 == __builtin_offsetof(hkWorld, m_collisionListeners));
static_assert(0x184 == __builtin_offsetof(hkWorld, m_broadPhaseBorder));
static_assert(0x190 == __builtin_offsetof(hkWorld, m_dynamicsStepInfo));
static_assert(0x2E0 == __builtin_offsetof(hkWorld, m_broadPhaseExtents));
static_assert(0x300 == __builtin_offsetof(hkWorld, m_broadPhaseNumMarkers));
static_assert(0x304 == __builtin_offsetof(hkWorld, m_sizeOfToiEventQueue));
static_assert(0x308 == __builtin_offsetof(hkWorld, m_broadPhaseQuerySize));
static_assert(0x30C == __builtin_offsetof(hkWorld, m_broadPhaseUpdateSize));
static_assert(0x310 == __builtin_offsetof(hkWorld, m_contactPointGeneration));

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
static_assert(sizeof(hkSimulation) == 0x28);

struct hkConstraintInfo
{
	int32 m_maxSizeOfSchema;
	int32 m_sizeOfSchemas;
	int32 m_numSolverResults;
	int32 m_numSolverElemTemps;
};
static_assert(sizeof(hkConstraintInfo) == 0x10);

struct hkConstraintOwner :
	public hkReferencedObject
{
	hkConstraintInfo m_constraintInfo;
};
static_assert(sizeof(hkConstraintOwner) == 0x18);

struct hkAction :
	public hkReferencedObject
{
	hkWorld* m_world;
	hkSimulationIsland* m_island;
	unsigned int m_userData;
	char const* m_name;
};
static_assert(sizeof(hkAction) == 0x18);

struct hkAgentNnSector
{
	char m_data[512];
};
static_assert(sizeof(hkAgentNnSector) == 0x200);

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
	uint16 m_storageIndex;
	uint16 m_dirtyListIndex;
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
static_assert(sizeof(hkSimulationIsland) == 0x70);
static_assert(0x18 == __builtin_offsetof(hkSimulationIsland, m_world));
static_assert(0x1C == __builtin_offsetof(hkSimulationIsland, m_numConstraints));
static_assert(0x20 == __builtin_offsetof(hkSimulationIsland, m_storageIndex));
static_assert(0x22 == __builtin_offsetof(hkSimulationIsland, m_dirtyListIndex));
static_assert(0x24 == __builtin_offsetof(hkSimulationIsland, m_splitCheckFrameCounter));
static_assert(0x25 == __builtin_offsetof(hkSimulationIsland, m_highFrequencyDeactivationCounter));
static_assert(0x26 == __builtin_offsetof(hkSimulationIsland, m_lowFrequencyDeactivationCounter));
//static_assert(0x27 == __builtin_offsetof(hkSimulationIsland, m_splitCheckRequested));
//static_assert(0x27 == __builtin_offsetof(hkSimulationIsland, m_sparseEnabled));
//static_assert(0x27 == __builtin_offsetof(hkSimulationIsland, m_actionListCleanupNeeded));
//static_assert(0x27 == __builtin_offsetof(hkSimulationIsland, m_allowIslandLocking));
//static_assert(0x28 == __builtin_offsetof(hkSimulationIsland, m_isInActiveIslandsArray));
//static_assert(0x28 == __builtin_offsetof(hkSimulationIsland, m_active));
//static_assert(0x28 == __builtin_offsetof(hkSimulationIsland, m_inIntegrateJob));
static_assert(0x2C == __builtin_offsetof(hkSimulationIsland, m_multiThreadLock));
static_assert(0x34 == __builtin_offsetof(hkSimulationIsland, m_timeSinceLastHighFrequencyCheck));
static_assert(0x38 == __builtin_offsetof(hkSimulationIsland, m_timeSinceLastLowFrequencyCheck));
static_assert(0x3C == __builtin_offsetof(hkSimulationIsland, m_actions));
static_assert(0x48 == __builtin_offsetof(hkSimulationIsland, m_timeOfDeactivation));
static_assert(0x4C == __builtin_offsetof(hkSimulationIsland, m_entities));
static_assert(0x5C == __builtin_offsetof(hkSimulationIsland, m_agentTrack));
