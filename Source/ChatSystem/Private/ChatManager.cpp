// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatManager.h"
#include "WebSocketsModule.h"
#include "CoreMinimal.h"
#include "Json.h"
#include "JsonUtilities.h"




// Sets default values
AChatManager::AChatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void AChatManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChatManager::ConnectChatServer(FString URL)
{
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	FString Protocol = "ws";

	//createing websocket pointer
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(URL, Protocol);

	//Assigning the functions
	//WebSocket->OnConnected().AddUFunction(this, FName("MessageRecieved"));
	WebSocket->OnMessage().AddUObject(this, &AChatManager::RecievedWebSocketMessage);
	WebSocket->OnMessageSent().AddUObject(this, &AChatManager::MessageSent);
	//WebSocket->OnClosed().AddUFunction(this, FName("ChatServerClosed"));

	WebSocket->Connect();
	//WebSocket->Send("40");

}

void AChatManager::RecievedWebSocketMessage(const FString& Message)
{

	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, "Message Receieved :" + Message);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// Successfully parsed JSON
		UE_LOG(LogTemp, Warning, TEXT("JSON Deserialized successfully"));

		FString MessageType = JsonObject->GetStringField(TEXT("type"));
		FString Message = "";

		if (MessageType.Equals("private"))
		{
			MessageReceivedFromServer.Content = JsonObject->GetStringField(TEXT("context"));
			MessageReceivedFromServer.MessageType = EMessageType::Private;
			MessageReceivedFromServer.SenderId = JsonObject->GetStringField(TEXT("sender"));

			OnMessageRecieved.Broadcast(MessageReceivedFromServer);
			
		}
		else if (MessageType.Equals("party"))
		{
			MessageReceivedFromServer.Content = JsonObject->GetStringField(TEXT("context"));
			MessageReceivedFromServer.MessageType = EMessageType::Party;
			MessageReceivedFromServer.SenderId = JsonObject->GetStringField(TEXT("sender"));

			OnMessageRecieved.Broadcast(MessageReceivedFromServer);
		}
		else if (MessageType.Equals("lobby"))
		{
			MessageReceivedFromServer.Content = JsonObject->GetStringField(TEXT("context"));
			MessageReceivedFromServer.MessageType = EMessageType::Lobby;
			MessageReceivedFromServer.SenderId = JsonObject->GetStringField(TEXT("sender"));

			OnMessageRecieved.Broadcast(MessageReceivedFromServer);
		}
		else if (MessageType.Equals("global"))
		{
			MessageReceivedFromServer.Content = JsonObject->GetStringField(TEXT("context"));
			MessageReceivedFromServer.MessageType = EMessageType::Global;
			MessageReceivedFromServer.SenderId = JsonObject->GetStringField(TEXT("sender"));

			OnMessageRecieved.Broadcast(MessageReceivedFromServer);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid Message"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON"));
	}
}

void AChatManager::SendWebSocketMessage(const FString& Message)
{
	if (WebSocket.IsValid())
	{
		if (WebSocket->IsConnected())
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, "Message Sent :" + Message);
			WebSocket->Send(Message);
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, "WebSocketNotConnected");
	}
}

void AChatManager::RegisterPlayer(FString PlayerID)
{
	FString JsonString = TEXT("{");
	JsonString += TEXT("\"type\": \"register\", ");
	JsonString += FString::Printf(TEXT("\"playerId\": \"%s\""), *PlayerID);
	JsonString += TEXT("}");
	
	SendWebSocketMessage(JsonString);
}

void AChatManager::MessageRecieved()
{
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, "Message Recieved!!");
}

void AChatManager::MessageSent(const FString& Message)
{
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, "Message Sent :" + Message);
}
void AChatManager::SendChatMessage(FString Message, EMessageType MessageType)
{

}



