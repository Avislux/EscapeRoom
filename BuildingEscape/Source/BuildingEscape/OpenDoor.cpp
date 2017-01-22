// Copyright Ben Tristem 2016.

#include "BuildingEscape.h"
#include "OpenDoor.h"
#include <string>


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	Owner = GetOwner();
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Pressure plate missing "));
	}
	
}


void UOpenDoor::CloseDoor() {

	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}
// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll trigger volume every frame
	if (GetTotalMassOFActorsOnPlate() > TriggerWeight) {

		OnOpen.Broadcast();
		
	}
	else { OnClose.Broadcast(); }

}

float UOpenDoor::GetTotalMassOFActorsOnPlate() 
{
	float totalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate)
	{
		return totalMass;
	}
	PressurePlate->GetOverlappingActors(OverlappingActors); //Initializes OverlappingActors
	for (auto& actor : OverlappingActors)
	{
		float mass = actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		totalMass += mass;
		//FString fTotalMass = FString::SanitizeFloat(totalMass);
		UE_LOG(LogTemp, Warning, TEXT("OverlappingActors: %s"), *actor->GetName());
	}
	UE_LOG(LogTemp, Warning, TEXT("Total mass %f"), totalMass)
	return totalMass;
}