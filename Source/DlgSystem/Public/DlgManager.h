// Copyright 2017-2018 Csaba Molnar, Daniel Butum
#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObjectIterator.h"
#include "EngineUtils.h"

#include "DlgDialogue.h"
#include "DlgDialogueParticipant.h"

#include "DlgManager.generated.h"

class UDlgContext;
class UDlgDialogue;

// Default comparison function
static bool PredicateSortFNameAlphabeticallyAscending(const FName& A, const FName& B)
{
	return A.Compare(B) < 0;
}

/**
 *  Class providing a collection of static functions to start a conversation and work with Dialogues.
 */
UCLASS()
class DLGSYSTEM_API UDlgManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Starts a Dialogue with the provided Dialogue and Participants array
	 * This method can fail in the following situations:
	 *  - The Participants number does not match the number of participants from the Dialogue.
	 *  - Any UObject in the Participant array does not implement the Participant Interface
	 *  - Participant->GetParticipantName() does not exist in the Dialogue
	 *
	 * @returns The dialogue context object or nullptr if something wrong happened
	 */
	UFUNCTION(BlueprintCallable, Category = DialogueLaunch)
	static UDlgContext* StartDialogue(UDlgDialogue* Dialogue, UPARAM(ref)const TArray<UObject*>& Participants);

	/**
	 * Helper methods, same as StartDialogue but with fixed amount of participant(s)
	 */
	UFUNCTION(BlueprintCallable, Category = DialogueLaunch)
	static UDlgContext* StartMonologue(UDlgDialogue* Dialogue, UObject* Participant);

	UFUNCTION(BlueprintCallable, Category = DialogueLaunch)
	static UDlgContext* StartDialogue2(UDlgDialogue* Dialogue, UObject* Participant0, UObject* Participant1);

	UFUNCTION(BlueprintCallable, Category = DialogueLaunch)
	static UDlgContext* StartDialogue3(UDlgDialogue* Dialogue, UObject* Participant0, UObject* Participant1, UObject* Participant2);

	UFUNCTION(BlueprintCallable, Category = DialogueLaunch)
	static UDlgContext* StartDialogue4(UDlgDialogue* Dialogue, UObject* Participant0, UObject* Participant1, UObject* Participant2, UObject* Participant3);

	/**
	 * Loads all dialogues from the filesystem into memory
	 */
	static void LoadAllDialoguesIntoMemory();

	/** Gets all loaded dialogues from memory. LoadAllDialoguesIntoMemory must be called before this */
	static TArray<UDlgDialogue*> GetAllDialoguesFromMemory()
	{
		TArray<UDlgDialogue*> Array;
		for (TObjectIterator<UDlgDialogue> Itr; Itr; ++Itr)
		{
			UDlgDialogue* Dialogue = *Itr;
			if (Dialogue != nullptr && !Dialogue->IsPendingKill())
			{
				Array.Add(Dialogue);
			}
		}
		return Array;
	}

	/** Gets all the actors from the provided World that implement the Dialogue Participant Interface */
	static TArray<AActor*> GetAllActorsImplementingDialogueParticipantInterface(UWorld* World)
	{
		TArray<AActor*> Array;
		for (TActorIterator<AActor> Itr(World); Itr; ++Itr)
		{
			AActor* Actor = *Itr;
			if (Actor != nullptr && !Actor->IsPendingKill() &&
				Actor->GetClass()->ImplementsInterface(UDlgDialogueParticipant::StaticClass()))
			{
				Array.Add(Actor);
			}
		}
		return Array;
	}

	/** Gets all the dialogues that have a duplicate GUID, should not happen, like ever. */
	static TArray<UDlgDialogue*> GetDialoguesWithDuplicateGuid();

	/** Gets all the loaded dialogues from memory that have the ParticipantName included insided them. */
	static TArray<UDlgDialogue*> GetAllDialoguesForParticipantName(const FName& ParticipantName);

	/** Default sorting function used by all the Dialogue related methods. Sorts alphabetically ascending. */
	static void SortDefault(TArray<FName>& OutArray)
    {
		OutArray.Sort(PredicateSortFNameAlphabeticallyAscending);
    }
	static void SortDefault(TSet<FName>& OutSet)
	{
		OutSet.Sort(PredicateSortFNameAlphabeticallyAscending);
	}

	template<typename ValueType>
	static void SortDefault(TMap<FName, ValueType>& Map)
	{
		Map.KeySort(PredicateSortFNameAlphabeticallyAscending);
	}

	/** Sets the UDlgMemory Dialogue history. */
	UFUNCTION(BlueprintCallable, Category = DialogueData)
	static void SetDialogueHistory(const TMap<FGuid, FDlgHistory>& DlgHistory);

	/** Gets the Dialogue History from the UDlgMemory. */
	UFUNCTION(BlueprintPure, Category = DialogueData)
	static const TMap<FGuid, FDlgHistory>& GetDialogueHistory();

	/** Does the Object implement the Dialogue Participant Interface? */
	UFUNCTION(BlueprintPure, Category = DialogueData)
	static bool DoesObjectImplementDialogueParticipantInterface(UObject* Object);

	/** Gets all the unique participant names sorted alphabetically from all the Dialogues loaded into memory. */
	UFUNCTION(BlueprintPure, Category = DialogueData)
	static void GetAllDialoguesParticipantNames(TArray<FName>& OutArray);

	/** Gets all the unique int variable names sorted alphabetically for the specified ParticipantName from the loaded Dialogues */
	UFUNCTION(BlueprintPure, Category = DialogueData)
	static void GetAllDialoguesIntNames(const FName& ParticipantName, TArray<FName>& OutArray);

	/** Gets all the unique float variable names sorted alphabetically for the specified ParticipantName from the loaded Dialogues */
	UFUNCTION(BlueprintPure, Category = DialogueData)
	static void GetAllDialoguesFloatNames(const FName& ParticipantName, TArray<FName>& OutArray);

	/** Gets all the unique bool variable names sorted alphabetically for the specified ParticipantName from the loaded Dialogues */
	UFUNCTION(BlueprintPure, Category = DialogueData)
	static void GetAllDialoguesBoolNames(const FName& ParticipantName, TArray<FName>& OutArray);

	/** Gets all the unique name variable names sorted alphabetically for the specified ParticipantName from the loaded Dialogues */
	UFUNCTION(BlueprintPure, Category = DialogueData)
	static void GetAllDialoguesNameNames(const FName& ParticipantName, TArray<FName>& OutArray);

	/** Gets all the unique condition names sorted alphabetically for the specified ParticipantName from the loaded Dialogues */
	UFUNCTION(BlueprintPure, Category = DialogueData)
	static void GetAllDialoguesConditionNames(const FName& ParticipantName, TArray<FName>& OutArray);

	/** Gets all the unique event names sorted alphabetically for the specified ParticipantName from the loaded Dialogues */
	UFUNCTION(BlueprintPure, Category = DialogueData)
	static void GetAllDialoguesEventNames(const FName& ParticipantName, TArray<FName>& OutArray);

private:

	/** Helper method, used to append a set to an array. Also sort. */
	static void AppendSetToArray(const TSet<FName>& InSet, TArray<FName>& OutArray)
	{
		TArray<FName> UniqueNamesArray = InSet.Array();
		SortDefault(UniqueNamesArray);
		OutArray.Append(UniqueNamesArray);
	}
};
