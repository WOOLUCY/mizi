// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "GameFramework/Actor.h"

class FActorPool : public FGCObject
{
public:
	virtual void ReturnActorToPool(class APooledActor* Actor) {}
};

template<typename T>
class FCircularActorPool : public FActorPool
{
	static_assert(TIsDerivedFrom<T, APooledActor>::IsDerived, "T must be derived from Actor");

public:
	void InitializePool(UWorld* World, int32 InPoolSize)
	{
		checkf(PoolSize == 0, TEXT("이미 초기화 됨"));
		PoolSize = InPoolSize;
		ActorPool.Reserve(InPoolSize);
		ActiveActors.Reserve(InPoolSize);

		for (int32 i = 0; i < PoolSize; ++i)
		{
			FActorSpawnParameters ActorSpawnParameters;
			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			T* NewActor = World->SpawnActor<T>(ActorSpawnParameters);
			NewActor->SetOwnerPool(this);
			DeactivateActor(static_cast<T*>(NewActor));
			ActorPool.Add(NewActor);
		}
	}

	T* GetActorFromPool()
	{
		if (ActorPool.IsEmpty())
		{
			TObjectPtr<T> ActiveActor = ActiveActors[0];
			ActiveActors.RemoveAt(0, EAllowShrinking::No);
			ReturnActorToPool(ActiveActor);
		}

		TObjectPtr<T> Actor = ActorPool.Pop(false);
		ActiveActors.Add(Actor);
		ActivateActor(Actor);
		return Actor;
	}

	virtual void ReturnActorToPool(APooledActor* Actor) override
	{
		DeactivateActor(static_cast<T*>(Actor));
		ActorPool.Add(static_cast<T*>(Actor));
	}

protected:
	void ActivateActor(T* Actor)
	{
		Actor->SetActorEnableCollision(true);
		Actor->SetActorHiddenInGame(false);

		if (Actor->IsTickable())
		{
			Actor->SetActorTickEnabled(true);
		}

		for (UActorComponent* Component : Actor->GetComponents())
		{
			Component->SetActive(true);
		}
	}

	void DeactivateActor(T* Actor)
	{
		Actor->SetActorEnableCollision(false);
		Actor->SetActorHiddenInGame(true);
		Actor->SetActorTickEnabled(false);

		for (UActorComponent* Component : Actor->GetComponents())
		{
			Component->SetActive(false);
		}
	}

	/**
	 * Pure virtual that must be overloaded by the inheriting class. Use this
	 * method to serialize any UObjects contained that you wish to keep around.
	 *
	 * @param Collector The collector of referenced objects.
	 */
	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		Collector.AddReferencedObjects(ActorPool);
		Collector.AddReferencedObjects(ActiveActors);
	}

	/** Overload this method to report a name for your referencer */
	virtual FString GetReferencerName() const
	{
		return TEXT("FCircularActorPool");
	}

private:
	TArray<TObjectPtr<T>> ActorPool;
	TArray<TObjectPtr<T>> ActiveActors; // 활성화되어 월드에 배치된 Actor
	int32 PoolSize = 0;
};

//template<typename T>
//class FActorPool : public FGCObject
//{
//	static_assert(TIsDerivedFrom<T, AActor>::IsDerived, "T must be derived from Actor");
//
//public:
//	void InitializePool(UWorld* World, int32 InPoolSize)
//	{
//		checkf(PoolSize == 0, TEXT("이미 초기화 됨"));
//		PoolSize = InPoolSize;
//		ActorPool.Reserve(InPoolSize);
//
//		for (int32 i = 0; i < PoolSize; ++i)
//		{
//			FActorSpawnParameters ActorSpawnParameters;
//			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//			T* NewActor = World->SpawnActor<T>(ActorSpawnParameters);
//			DeactivateActor(NewActor);
//			ActorPool.Add(NewActor);
//		}
//	}
//
//	T* GetActorFromPool()
//	{
//		if (ActorPool.IsEmpty())
//		{
//			return nullptr;
//		}
//
//		TObjectPtr<T> Actor = ActorPool.Pop(false);
//		ActivateActor(Actor);
//		return Actor;
//	}
//
//	void ReturnActorToPool(T* Actor)
//	{
//		DeactivateActor(Actor);
//		ActorPool.Add(Actor);
//	}
//
//protected:
//	void ActivateActor(T* Actor)
//	{
//		Actor->SetActorEnableCollision(true);
//		Actor->SetActorHiddenInGame(false);
//		Actor->SetActorTickEnabled(true);
//
//		for (UActorComponent* Component : Actor->GetComponents())
//		{
//			Component->SetActive(true);
//		}
//	}
//
//	void DeactivateActor(T* Actor)
//	{
//		Actor->SetActorEnableCollision(false);
//		Actor->SetActorHiddenInGame(true);
//		Actor->SetActorTickEnabled(false);
//
//		for (UActorComponent* Component : Actor->GetComponents())
//		{
//			Component->SetActive(false);
//		}
//	}
//
//	/**
//	 * Pure virtual that must be overloaded by the inheriting class. Use this
//	 * method to serialize any UObjects contained that you wish to keep around.
//	 *
//	 * @param Collector The collector of referenced objects.
//	 */
//	virtual void AddReferencedObjects(FReferenceCollector& Collector)
//	{
//		//Collector.AddReferencedObjects(ActorPool);
//	}
//
//	/** Overload this method to report a name for your referencer */
//	virtual FString GetReferencerName() const
//	{
//		return TEXT("FActorPool");
//	}
//private:
//	TArray<TObjectPtr<T>> ActorPool;
//	int32 PoolSize = 0;
//};