// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "IWebSocketsManager.h"
#include "ChatManager.generated.h"

UENUM(BlueprintType)
enum class EMessageType : uint8
{
	Private UMETA(DisplayName = "Private"),
	Party UMETA(DisplayName = "Party"),
	Global UMETA(DisplayName = "Global"),
	Lobby UMETA(DisplayName = "Lobby"),
	SentMsg UMETA(DisplayName = "SentMsg")
};

USTRUCT(BlueprintType)
struct FReceivedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Message")
	FString SenderId;

	UPROPERTY(BlueprintReadWrite, Category = "Message")
	EMessageType MessageType;

	UPROPERTY(BlueprintReadWrite, Category = "Message")
	FString Content;

	// Constructor
	FReceivedMessage()
	{
		SenderId = TEXT("");
		MessageType = EMessageType::SentMsg; // Set default message type
		Content = TEXT("");
	}

	// Constructor with parameters
	FReceivedMessage(const FString& InSenderId, const EMessageType& InMessageType, const FString& InContent)
		: SenderId(InSenderId), MessageType(InMessageType), Content(InContent)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKFOnMessageRecieved, FReceivedMessage, ReceivedMessage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKFOnPartyMembersUpdated);

UCLASS()
class CHATSYSTEM_API AChatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChatManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSharedPtr<IWebSocket> WebSocket;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ConnectChatServer(FString URL);

	void RecievedWebSocketMessage(const FString& Message);

	void SendWebSocketMessage(const FString& Message);

	UFUNCTION(BlueprintCallable)
	void RegisterPlayer(FString PlayerID);

	UPROPERTY(BlueprintAssignable)
	FKFOnMessageRecieved OnMessageRecieved;

	UPROPERTY(BlueprintAssignable)
	FKFOnPartyMembersUpdated OnPartyMembersUpdated;

	FReceivedMessage MessageReceivedFromServer;

	void MessageRecieved();

	void MessageSent(const FString& Message);

	UFUNCTION(BlueprintCallable)
	void SendPrivateMessage(FString Message, FString SenderId, FString ReceiverId);

	UFUNCTION(BlueprintCallable)
	void SendPartyMessage(FString Message, FString SenderId, FString PartyId);

	UFUNCTION(BlueprintCallable)
	void SendGlobalMessage(FString Message, FString SenderId);

	UFUNCTION(BlueprintCallable)
	void SendLobbyMessage(FString Message, FString SenderId);

	UFUNCTION(BlueprintCallable)
	void CreateParty(FString PartyID, FString PlayerID);

	UFUNCTION(BlueprintCallable)
	void JoinParty(FString PartyID, FString PlayerID);

	UFUNCTION(BlueprintCallable)
	void GetPartyMembers(FString PartyID);

	UFUNCTION(BlueprintCallable)
	void EmptyParty(FString PartyID);

	UFUNCTION(BlueprintCallable)
	void MutePlayer(FString PartyID);

	UFUNCTION(BlueprintCallable)
	void connectionSuccess(bool isSuccess);

	UPROPERTY(BlueprintReadOnly)
	bool isConnectionSuccess;

	UPROPERTY(BlueprintReadOnly)
	FString SID;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> PartyMembers;


};
